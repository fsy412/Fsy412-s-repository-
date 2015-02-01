package main

import (
	"net"
	"log"
	"handle"
	"message"
)

var (
	maxRead = 1100
	msgStop   = []byte("cmdStop")
	msgStart  = []byte("cmdContinue")
)

func main() {
    hostAndPort := "0.0.0.0:443"
    listener := initServer(hostAndPort)
    for {
        conn, err := listener.Accept()
        checkError(err, "Accept: ")
        go connectionHandler(conn)
    }
}
 
func initServer(hostAndPort string) *net.TCPListener {
    serverAddr, err := net.ResolveTCPAddr("tcp", hostAndPort)
    checkError(err, "Resolving address:port failed: '" + hostAndPort + "'")
 
    listener, err := net.ListenTCP("tcp", serverAddr)
    checkError(err, "ListenTCP: ")
 
    println("Listening to: ", listener.Addr().String())
 
    return listener
}
 
func connectionHandler(conn net.Conn) {
    connFrom := conn.RemoteAddr().String()
    println("Connection from: ", connFrom)
   
    ch := make(chan []byte, 10000)
    go StartSession(ch, conn)
   
    for {
        buf := make([]byte, maxRead + 1)
        length, err := conn.Read(buf[0:maxRead])
        //err := conn.Read(buf[0:maxRead])
	log.Println("length", length)
        buf[maxRead] = 0 // to prevent overflow
  
 	switch err {
    		case nil:
			ch <- buf
 
	   	default:
			goto DISCONNECT
    	}		
    }

    DISCONNECT:
    err := conn.Close()
    println("Closed connection:" , connFrom)
    checkError(err, "Close:" )
    close(ch)
}

func handleMsg(length int, err error, msg []byte, conn net.Conn) {
    if length > 0 {
	name, total_len, name_len := message.Decode(msg)
	
	log.Println("handleMsg", name)
	handle := handle.ProtoHandler[name]
	
	if handle == nil{
		log.Println("hanlder not found")
		return 
	}

   	resp := handle(msg[(8 + name_len): total_len])
	sendMsg(conn, message.Encode(resp, "SessionId"))
   }
}

func sendMsg(to net.Conn, msg []byte) {
    len, err := to.Write(msg)
    checkError(err, "sendMsg: " + string(len) + " bytes.")
}

func checkError(error error, info string) {
	if error != nil{
		panic("ERROR:" + info + " " + error.Error())
	}
}

func StartSession(incoming chan []byte, conn net.Conn) {
    defer func(){
	log.Println("StartSession defered")
    }()

    for {
        select {
		case msg, ok := <-incoming:
			if !ok {
				return
			}
		var err error
	    	handleMsg(len(msg), err, msg, conn)
        }
   }
} 
