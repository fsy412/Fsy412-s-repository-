package main

import (
    "fmt"
    "labix.org/v2/mgo"
    "labix.org/v2/mgo/bson"
)

type Mail struct {
    Id bson.ObjectId "_id"
    Name string
    Email string
}

func main() {
    // 连接数据库
    session, err := mgo.Dial("127.0.0.1")
    if err != nil {
        panic(err)
    }
    defer session.Close()

    // 获取数据库,获取集合
    c := session.DB("test").C("mail")

    // 存储数据
    m1 := Mail{bson.NewObjectId(), "user1", "user1@dotcoo.com"}
    m2 := Mail{bson.NewObjectId(), "user1", "user2@dotcoo.com"}
    m3 := Mail{bson.NewObjectId(), "user3", "user3@dotcoo.com"}
    m4 := Mail{bson.NewObjectId(), "user3", "user4@dotcoo.com"}
    err = c.Insert(&m1, &m2, &m3, &m4)
    if err != nil {
        panic(err)
    }

    // 读取数据
    ms := []Mail{}
    err = c.Find(&bson.M{"name":"user3"}).All(&ms)
    if err != nil {
        panic(err)
    }

    // 显示数据
    for i, m := range ms {
        fmt.Printf("%s, %d, %s\n", m.Id.Hex(), i, m.Email)
    }
}
