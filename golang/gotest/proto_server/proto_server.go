package main
import proto "github.com/golang/protobuf/proto" 
import (
"bytes"
"encoding/binary"
"net"
"fmt"
"./message"
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
    //talktoclients(conn)
    for {
        var ibuf []byte = make([]byte, maxRead + 1)
        length, err := conn.Read(ibuf[0:maxRead])
        ibuf[maxRead] = 0 // to prevent overflow
    	switch err {
    		case nil:
		    	handleMsg(length, err, ibuf, conn)
 
	   	default:
      		  goto DISCONNECT
    	}		
    }
    DISCONNECT:
    err := conn.Close()
    println("Closed connection:" , connFrom)
    checkError(err, "Close:" )
}
 
func sendMsg(to net.Conn, msg []byte) {
    fmt.Println("sendMsg")	    
    len, err := to.Write(msg)
    fmt.Println("len:" + string(len))
    checkError(err, "sendMsg: " + string(len) + " bytes.")
}

func IntToBytes(n int) []byte {
    x := int32(n)
 
    bytesBuffer := bytes.NewBuffer([]byte{})
    binary.Write(bytesBuffer, binary.BigEndian, x)
    return bytesBuffer.Bytes()
}

func encode(message []byte, name string ) []byte{
	fmt.Println("encode total:", 8 + len(name) + len(message)) 
	return append(append(append(IntToBytes(8 + len(name) + len(message)), IntToBytes(len(name))...), []byte(name)...), message...)
}

func BytesToInt(b []byte) int {
    bytesBuffer := bytes.NewBuffer(b)
 
    var x int32
    binary.Read(bytesBuffer, binary.BigEndian, &x)
 
    return int(x)
}

func getProtoName(msg []byte) string {
	var len int
	len = BytesToInt(msg)
	if len < 0 {
		return "err" 
	}
	
	var name_len int
	name_len = BytesToInt(msg[4:8])
	
	var name string
	name = string(msg[8:8 + name_len])
	
	return name
} 

func handleMsg(length int, err error, msg []byte, conn net.Conn) {
    if length > 0 {
        fmt.Println("handleMsg proto name:", getProtoName(msg))
	protomsg := &message.SessionId{}
	buf, err := proto.Marshal(protomsg)
	sendMsg(conn, encode(buf, getProtoName(msg)))
	checkError(err, "handleMsg")
    }
 
}

func checkError(error error, info string) {
	if error != nil{
		panic("ERROR:" + info + " " + error.Error())
	}
} 
