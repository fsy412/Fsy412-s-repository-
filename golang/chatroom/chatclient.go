package main

import (
	"bufio"
	. "./chat"
	"log"
	"net"
	"os"
)

func main() {

	conn, err := net.Dial("tcp", "127.0.0.1:12345")

	if err != nil {
		log.Fatal(err)
	}

	defer conn.Close()
	in := bufio.NewReader(os.Stdin)
	out := bufio.NewWriter(os.Stdout)

	client := CreateClient(conn)

	go func() {
		for {
			out.WriteString(client.GetIncoming() + "\n")
			out.Flush()
		}
	}()

	for {
		line, _, _ := in.ReadLine()
		client.PutOutgoing(string(line))
	}

}
