package main
 
import (
    "log"
)
 
func main() {
    defer func() {
        if r := recover(); r != nil {
            log.Printf("Runtime error caught: %v", r)
        }
    }()
 
    a()
}
 
func a() {
    panic("a内错误")
}
