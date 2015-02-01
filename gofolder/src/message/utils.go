package message

import (
	"bytes"
	"encoding/binary"
	"log"
)

func Encode(message []byte, name string ) []byte{
        log.Println("Encode total:", 8 + len(name) + len(message))
        return append(append(append(IntToBytes(8 + len(name) + len(message)), IntToBytes(len(name))...), []byte(name)...), message...)
}

func Decode(msg []byte)(string ,int, int){
        total_len := BytesToInt(msg)
        if total_len < 0 {
        }

        name_len := BytesToInt(msg[4:8])
        name := string(msg[8:8 + name_len])

        return name, total_len, name_len
}

func BytesToInt(b []byte) int {
    bytesBuffer := bytes.NewBuffer(b)

    var x int32
    binary.Read(bytesBuffer, binary.BigEndian, &x)

    return int(x)
}

func IntToBytes(n int) []byte {
    x := int32(n)
    bytesBuffer := bytes.NewBuffer([]byte{})
    binary.Write(bytesBuffer, binary.BigEndian, x)

    return bytesBuffer.Bytes()
}
