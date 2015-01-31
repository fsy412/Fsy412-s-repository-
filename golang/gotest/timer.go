package main
 
import (
    "fmt"
    "time"
)

func main() {
	go loopingCall()
	go loop()
}
func loop(){
	for {
		fmt.Println("loop")
	}
}
func loopingCall(){


    pingTicker := time.NewTicker(30 * time.Second) // 定时
    testAfter := time.After(5 * time.Second)       // 延时
 
    for {
        select {
        case <-pingTicker.C:
		fmt.Println(" case <-pingTicker.C:")
        case <-testAfter:
		fmt.Println("case <-testAfter:")
        }
    }

}
