package main

import (
	. "./chat"
)

func main() {
/*	if len(os.Args) != 2 {
		fmt.Printf("Usage: %s <port>\n", os.Args[0])
		os.Exit(-1)
	}
*/
	server := CreateServer()
	server.Start("localhost:12345")

}
