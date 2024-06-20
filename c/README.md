# buda 社区资讯服务网站服务端

我们用C语言为buda社区资讯服务网站写了一个专属的http网站服务器，希望能由此提供极限的服务器效率和灵活性。
使用了极少量的C++语言语言特性，比如namespace，函数参数的default value，绝大部分为纯C语言代码，为什么要这样呢？我们希望尽可能多的掌握每一句代码的运行原理，开发的速度慢一点没关系，关键是要尽可能多的清楚知晓、完全掌控。

## 文件目录说明
release: 存放不带debug信息的可执行文件的地方
debug: 存放开发时编译出来的可执行文件的地方
src: 存放入口源代码的地方
includes: 存放头文件的地方
lib: 存放库文件源代码的地方
play: 随意玩乐的地方，专放乱七八糟可忽略的文件的地方

## 开发和运行环境
开发环境：Ubuntu Linux 24.04 AMD 64桌面版，VSCODE最新版
运行环境：Ubuntu Linux 22/24 云服务器

## 如何开发？
```sh
cd buda/c/debug
gcc ../src/socket_server.c ../lib/*.c -g -o socket_server
gdb socket_server -w /path/to/web_root
```

## 如何发布？
```sh
# 查看防火墙的状态：(国内的云服务器80,443,8080,8443是备案端口，必须备案以后才能从外网访问，注意避免使用)
ufw status
ufw allow 8888
# 查看端口是否被其它程序占用
sudo apt lsof
lsof -i:8888

cd buda/c/release
gcc ../src/socket_server.c ../lib/*.c -o socket_server
./socket_server -w /path/to/web_root
```
