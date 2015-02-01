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
    
    another_msg := &lm.Test{
	Test: proto.String("test"),
    }

    path := string("./log.txt")
    f, err := os.Create(path)
    if err != nil {
        fmt.Printf("failed: %s\n", err)
        return
    }

    defer f.Close()
    buffer, err := proto.Marshal(msg) //SerializeToOstream
    new_helloworld := &lm.Helloworld{}
    err = proto.Unmarshal(buffer, new_helloworld)
    fmt.Println("get from newhellworld" )
    fmt.Println("Id:" , new_helloworld.GetId())
     
    f.Write(buffer)
    buffer, err = proto.Marshal(another_msg)

} 

 
