package main

import "fmt"
import "time"

import "os/exec"

import "net"

type A struct {
	Sfds string
	Sftt int
}

var t1 *time.Timer

func dingshi() {
	for {
		select {
		case <-t1.C:
			t1.Reset(2 * time.Minute)
			fmt.Printf("jiang")
			exec.Command("/bin/sh", "/root/run.sh")
		}
	}
}

func fazhan(cn net.Conn) {
	var zhi = make([]byte, 1024)
	for {
		cn.Read(zhi)
		fmt.Println(zhi)
	}
}

const (
	asfd = iota
	asdf
)

func main() {
	t1 = time.NewTimer(2 * time.Minute)
	dingshi()
}
