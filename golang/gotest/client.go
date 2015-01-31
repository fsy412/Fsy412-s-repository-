package main

import (
    "net"
    "fmt"
)

func main() {
    currency := 20 
    count := 10 
    for i:=0;i<currency;i++ {
        go func(){
            for j:=0;j<count;j++ {
                sendMessage()
            }
        }()
    }
    select{}
}

func sendMessage() {
    conn, err := net.Dial("tcp", "127.0.0.1:443")

    if(err != nil) {
        panic("error")
    }

    header := "GET / HTTP/1.0\r\n\r\n"
    fmt.Fprintf(conn, header)
}
