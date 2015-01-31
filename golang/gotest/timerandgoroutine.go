package main
import (
"fmt"
"time"
)

func main(){
	go test()
}

func  test(){
	ch := make(bool)

	for i := 0; i < 10; i++ {
    		go func() {
        		println("begin")
	       		 <-ch
	        	println("end")
    		}
	}

	time.Sleep(10 * time.Second)
	close(ch)
}
