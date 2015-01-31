package main
import(
             "fmt"
          )

var a string
var c = make(chan int,3)//此处定义为缓冲的channel
func ff(){
          a ="hello world"
          ca := <-c
          fmt.Println(ca)
        }

func  main(){
           go ff()      //开启一个goroutine
          c<-3
          c<-13
          c<-123
          c<-333
          fmt.Println(a)
         
           }
