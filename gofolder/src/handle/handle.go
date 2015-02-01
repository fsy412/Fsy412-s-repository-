package handle

import proto "github.com/golang/protobuf/proto"
import (
"message"
"log"
)

var ProtoHandler map[string]func([]byte) []byte

func HandleSeverInfoReq(data []byte) []byte{
	log.Println("HandleSeverInfoReq")

	req := &message.ServerInfoReq{}
	error := proto.Unmarshal(data, req)
	checkError(error,"HandleSeverInfoReq")

	log.Println("ip:", req.GetIp())
	log.Println("param", req.GetParam())

	msg := &message.SessionId{}
	buf, err := proto.Marshal(msg)
	checkError(err, "buildProtoMessage")
	
	return buf
}
 
func HandleDBOperation(data []byte) []byte{
	msg := &message.SessionId{}
	buf, err := proto.Marshal(msg)
	checkError(err, "buildProtoMessage")
	
	return buf
}
 
func init() {
	ProtoHandler = map[string]func([]byte) []byte{
		"ServerInfoReq":   HandleSeverInfoReq,
		"DBOperationReq":   HandleDBOperation,
	}
}

func checkError(error error, info string) {
	if error != nil{
		panic("ERROR:" + info + " " + error.Error())
	}
}
