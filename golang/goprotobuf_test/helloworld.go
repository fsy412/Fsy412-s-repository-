package main

import proto "github.com/golang/protobuf/proto"
import (
    "./lm"
    "fmt"
    "os"
)

func main() {

    msg := &lm.Helloworld{
        Id:  proto.Int32(101),
        Str: proto.String("hello"),
    } //msg init

    path := string("./log.txt")
    f, err := os.Create(path)
    if err != nil {
        fmt.Printf("failed: %s\n", err)
        return
    }

    defer f.Close()
    buffer, err := proto.Marshal(msg) //SerializeToOstream
    f.Write(buffer)
 
} 

 
