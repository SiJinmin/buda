# buda 社区资讯服务网站

本项目是开发一个提供社区资讯服务的网站，前端用vue3开发，后端用C/C++语言自建http服务器。<br>
buda是「佛」的英文Budda的简写，意味推崇大乘佛法「慈悲喜舍、普度众生」的精神。<br>
为了将来最大限度地满足高并发性访问和大量视频、文件的下载，提高服务器的运行效率和灵活性，用C/C++语言开发了专属了http服务器。<br>


## 目录结构

buda/c: 网站后端，用C/C++语言自建的专用于本站的http服务器，实现了本站需要的极少量的http服务协议，并非http协议的完整实现，以后也不打算成为通用性http服务器。<br>
buda/vue: 网站前端，用Vue3开发。<br>
buda/node: 偶尔使用的一些nodejs脚本小工具，与本站无关，可以忽略。<br>


## 前端

```sh
cd buda/vue
npm i
npm run dev
npm run build
```


## 服务端

服务端位于 buda/c 文件夹中。

### 开发思路说明

主要使用C语言开发，使用了极少量的C++语言特性，绝大部分为纯C语言代码，尽量不用C++的语言特性，为什么要这样呢？
1. 我们希望尽可能清晰地掌握每一句代码的运行原理，以便出现问题时能快速解决。
2. 我们不推崇将函数绑定到一个类的OOP编程思想，而是认为，函数通常都是具有跨类的通用性的，不应该专属于某一个类，因此，我们仅仅使用了C语言的struct来表示一个对象的数据，而让所有的函数以库的形式分类放置。

使用的极少量C++语言特性：
1. namespace
2. 函数设置参数的default value

### 文件目录说明
release: 存放不带debug信息的可执行文件的地方<br>
debug: 存放开发时编译出来的带有debug信息的可执行文件的地方<br>
src: 存放入口源代码的地方<br>
includes: 存放头文件的地方<br>
lib: 存放库文件源代码的地方<br>
play: 随意玩乐的地方，专放乱七八糟可忽略的文件的地方<br>

### 开发和部署环境
开发环境：Ubuntu Linux 24.04 AMD 64桌面版，VSCODE最新版<br>
部署环境：Ubuntu Linux 22/24 不包含xwindow的服务器版<br>

### 如何开发？
```sh
cd buda/c/debug
gcc ../src/socket_server.c ../lib/*.c -g -o socket_server
gdb socket_server -w ../../vue/dist -p 80
```

### 如何部署？
```sh
# 查看防火墙的状态：(国内的云服务器80,443,8080,8443是备案端口，必须备案以后才能从外网访问，注意避免使用)
ufw status
ufw allow 80
# 查看端口是否被其它程序占用
sudo apt lsof
lsof -i:80

cd buda/c/release
gcc ../src/socket_server.c ../lib/*.c -o socket_server
./socket_server -w ../../vue/dist -p 80
```


## 已完成的功能

2024年6月22日<br>
实现http协议的Get /url功能，获取服务端的vue/dist中的静态文件。<br>
当服务端发现异常时，返回给用户vue/dist/index.html文件的内容。<br>
完成网站标题、菜单的现实，招聘信息和租房信息的列表页面。<br>
完成图片显示组件，可全屏观看、上一张、下一张图片。<br>
人物传记整理工具，完成30%<br>


## 即将开发的功能

实现https协议<br>
实现服务端的多线程运行<br>
实现服务端的内容缓存系统，提升运行效率<br>
实现纯素商城<br>
实现公司官网模板<br>
完成人物传记整理工具<br>


## 联系作者、捐助作者

欢迎加我为微信好友，讨论网站的开发、社区资讯服务的运营、捐助投资。<br>
微信：TobeBuda<br>
支付宝 / Paypal: jinmin.si@outlook.com<br>
Email: jinmin.si@outlook.com  si.jinmin@gmail.com<br>
微信公众号：立心成见<br>
CSDN：https://blog.csdn.net/qiuzen<br>
Github: https://github.com/SiJinmin<br>


