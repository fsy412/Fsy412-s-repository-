package main
import (
"fmt"
"reflect"
)
type User struct {
Id int
Name string
Age int
Title string
}
func(this User)Test()string{
return this.Name
}

func main(){
var o interface {} = &User{1,"Tom",12,"nan"}
v := reflect.ValueOf(o)
fmt.Println(v)
m := v.MethodByName("Test")
rets := m.Call([]reflect.Value{})
fmt.Println(rets)
}

