package chat

import (
	"fmt"
	"log"
	"net"
	"strings"
)

const (
	MAXCLIENTS = 50
)

type Message chan string
type Token chan int
type ClientTable map[net.Conn]*Client

type Server struct {
	listener net.Listener
	clients  ClientTable
	tokens   Token
	pending  chan net.Conn
	quiting  chan net.Conn
	incoming Message
	outgoing Message
}

func (self *Server) generateToken() {
	self.tokens <- 0
}

func (self *Server) takeToken() {
	<-self.tokens
}

func CreateServer() *Server {
	server := &Server{
		clients:  make(ClientTable, MAXCLIENTS),
		tokens:   make(Token, MAXCLIENTS),
		pending:  make(chan net.Conn),
		quiting:  make(chan net.Conn),
		incoming: make(Message),
		outgoing: make(Message),
	}
	server.listen()
	return server
}

func (self *Server) listen() {
	go func() {
		for {
			select {
			case message := <-self.incoming:
				self.broadcast(message)
			case conn := <-self.pending:
				self.join(conn)
			case conn := <-self.quiting:
				self.leave(conn)
			}
		}
	}()
}

func (self *Server) join(conn net.Conn) {
	client := CreateClient(conn)
	name := getUniqName()
	client.SetName(name)
	self.clients[conn] = client

	log.Printf("Auto assigned name for conn %p: %s\n", conn, name)

	go func() {
		for {
			msg := <-client.incoming
			log.Printf("Got message: %s from client %s\n", msg, client.GetName())

			if strings.HasPrefix(msg, ":") {
			}
			// fallthrough to normal message if it is not parsable or executable
			self.incoming <- fmt.Sprintf("%s says: %s", client.GetName(), msg)
		}
	}()

	go func() {
		for {
			conn := <-client.quiting
			log.Printf("Client %s is quiting\n", client.GetName())
			self.quiting <- conn
		}
	}()
}

func (self *Server) leave(conn net.Conn) {
	if conn != nil {
		conn.Close()
		delete(self.clients, conn)
	}

	self.generateToken()
}

func (self *Server) broadcast(message string) {
	log.Printf("Broadcasting message: %s\n", message)
	for _, client := range self.clients {
		client.outgoing <- message
	}
}

func (self *Server) Start(connString string) {
	
	log.Print("connString %p\n", connString)
	serverAddr, err := net.ResolveTCPAddr("tcp", connString)

	self.listener, err = net.ListenTCP("tcp", serverAddr)
	checkError(err, "ListenTCP: ")

	log.Printf("Server %p starts\n", self)

	// filling the tokens
	for i := 0; i < MAXCLIENTS; i++ {
		self.generateToken()
	}

		log.Println("going to for")
	for {
		conn, err := self.listener.Accept()

		log.Println("check err")
		if err != nil {
			log.Println("err accpet %v", err )
			return
		}

		log.Printf("A new connection %v kicks\n", conn)

		self.takeToken()
		self.pending <- conn
	}
}

// FIXME: need to figure out if this is the correct approach to gracefully
// terminate a server.
func (self *Server) Stop() {
	self.listener.Close()
}

func checkError(error error, info string) {
	if error != nil{
		panic("ERROR:" + info + " " + error.Error())
	}
}
