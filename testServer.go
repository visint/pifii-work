package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
)

const (
	upload_path string = "./"
)

func helloHandle(w http.ResponseWriter, r *http.Request) {
	io.WriteString(w, "hello world!")
}

//上传
func uploadHandle(w http.ResponseWriter, r *http.Request) {
	//从请求当中判断方法
	if r.Method == "GET" {
		io.WriteString(w, "<html><head><title>我的第一个页面</title></head><body><form action='' method=\"post\" enctype=\"multipart/form-data\"><label>上传图片</label><input type=\"file\" name='file'  /><br/><label><input type=\"submit\" value=\"上传图片\"/></label></form></body></html>")
	} else {
		//获取文件内容 要这样获取
		file, head, err := r.FormFile("file")
		if err != nil {
			fmt.Println(err)
			return
		}
		defer file.Close()
		//创建文件
		fW, err := os.Create(upload_path + head.Filename)
		if err != nil {
			fmt.Println("文件创建失败")
			return
		}
		defer fW.Close()
		_, err = io.Copy(fW, file)
		if err != nil {
			fmt.Println("文件保存失败")
			return
		}
		fmt.Println("post file ok")
		//io.WriteString(w, head.Filename+" 保存成功")
		http.Redirect(w, r, "/hello", http.StatusFound)
		//io.WriteString(w, head.Filename)
	}
}

// 静态文件处理
func StaticServer(w http.ResponseWriter, req *http.Request) {
	fmt.Println("path:" + req.URL.Path)
//	w.Header().Set("Content-type", "text/html")
	staticHandler.ServeHTTP(w, req)
	fmt.Println("get file ok")

}

var staticHandler http.Handler

func main() {
	//启动一个http 服务器
	staticHandler = http.FileServer(http.Dir("./"))

	http.HandleFunc("/hello", helloHandle)

	//上传
	http.HandleFunc("/image", uploadHandle)

	http.HandleFunc("/", StaticServer)

	err := http.ListenAndServe(":9090", nil)
	if err != nil {
		fmt.Println("服务器启动失败")
		return
	}
	fmt.Println("服务器启动成功")
}
