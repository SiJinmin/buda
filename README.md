# buda小时工资讯网

本项目是开发一个提供小时工资讯服务的网站，前端用vue3开发，后端用C/C++语言自建http服务器。<br>
buda是「佛」的英文Budda的简写，意谓推崇大乘佛法「慈悲喜舍、普度众生」的精神。<br>
为了将来最大限度地满足高并发性访问和大量视频、文件的下载，提高服务器的运行效率和灵活性，用C/C++语言开发了专属的http服务器。<br>


## 本项目的开发运行环境

We develop it on Ubuntu Linux x64 system. 32-bit system or Windows system cannot compile and run it.
We compile it by gcc. Other compilers are not tested.


## 主要功能

### 招聘信息

### human life story edit tool
auto adjust input area height.<br>
auto save user input content into local storage, the red color text means it shall be saved in 5 seconds.<br>
auto get the event time from input content, support time locking.<br>
auto compute the human age for events.<br>

### config file: /etc/buda.conf

Sample buda.conf is buda/doc/buda.conf, you can copy it to /etc.

```sh
cp buda/doc/buda.conf /etc/
```

It's ok if /etc/buda.conf is missing, then the following default values will be used.

```js
{
  pw: "Buda123456",
}
```


## source code arch

buda/c: website backend, including the http server by C/C++. It's not full implementation of http protocol, just some functions we need in this website. <br>
buda/vue: website frontend, by Vue3.<br>
buda/node: some script tools by nodejs, you can ignore it. <br>


## about frontend

```sh
cd buda/vue
npm i
npm run dev
npm run build
```


## about backend

Backend is in buda/c.

### code principles

We try to just use C, but use C++ as little as possilbe. The C++ features we use are:
1. namespace
2. function default value

### source code arch of buda/c/

release: exe file with no debug info<br>
debug: exe file with debug info<br>
src: entry source file<br>
includes: head files<br>
lib: library source files<br>
play: for fun, you can ignore it.<br>

### development

Ubuntu Linux 24.04 AMD 64 Desktop，VSCODE<br>

```sh
sudo cp buda/buda.conf /etc/
# then modify /etc/buda.conf to your settings
cd buda/vue
npm run build
cd buda/c/debug
gcc ../src/socket_server.cpp ../lib/*.cpp -g -o socket_server
sudo ./socket_server -w ../../vue/dist -p 8888
gdb --args socket_server -w ../../vue/dist -p 80
```

in ~/.bash_aliases

```sh
alias gitbuda='gitbuda() { cd ~/code/buda; git add -A; git commit -m "$1"; git push; }; gitbuda'
alias makebudav='cd ~/code/buda/vue && npm run build'
alias runbudav='cd ~/code/buda/vue && npm run dev'
alias makebudac='cd ~/code/buda/c/debug && gcc ../src/socket_server.cpp ../lib/*.cpp -g -o socket_server'
alias runbudac='cd ~/code/buda/c/debug && ./socket_server -w ../../vue/dist -p 8888'
alias debugbudac='cd ~/code/buda/c/debug && gdb --args ./socket_server -w ../../vue/dist -p 8888'
```
commit to git: # gitbuda "commit message"

### deployment

Ubuntu Linux 22 cloud server<br>

```sh
# check firewall(80,443,8080,8443 are 备案 ports in China, do not use them before 备案)
ufw status
ufw allow 80
apt install lsof
lsof -i:80

cp buda/buda.conf /etc/
# then modify /etc/buda.conf to your settings
cd buda/vue
npm run build
cd buda/c/release
gcc ../src/socket_server.cpp ../lib/*.cpp -o socket_server
./socket_server -w ../../vue/dist -p 80
```
For "systemctl start buda" style, please ref buda/doc/buda.service


## 开发计划

id generator by timenano
訪問index.html時寫入cookie
数据库每日备份
网站备案<br>
完成日志查看功能：
log的写入严格检查
每隔一定的时间获取一次内存使用量/剩余量、硬盘使用量/剩余量、CPU占有率
统计和显示任意时间间隔段内的线程的工作时长、空置率、内存使用量（%）、硬盘使用量(%)、CPU占有率、request数量、接收字节数、发送字节数、每个request的平均响应时间、每个字节的平均接收和发送时长<br>
文件上传和下载<br>
使用cmake<br>
内存分配和释放记录<br>
拼车模块、租车模块<br>
Img组件扫动时切换图片<br>
活动详情页面居中显示<br>
实现https协议<br>
实现服务端的多线程运行，开启线程前后检查每个线程消耗的内存数量，线程数量通过CPU核心数决定<br>
实现服务端的内容缓存系统，提升运行效率<br>
实现Connection: keep-alive<br>
内存使用动态释放较长时间不用的内存块<br>
实现共享MemChain Pool<br>
实现纯素商城: 出售娃哈哈、三只松鼠素肉、佛法艺术品、joke咖啡<br>
日租房<br>
实现公司官网模板<br>
全民分红系统<br>
完成人物传记整理工具<br>
实现全民兼职业务员网络系统<br>
添加文章视频发布、常用信息剪贴板<br>
svg文件格式简化，首页comp集成<br>
三套异地的数据库备份和查看系统<br>
报警系统，每日主动发起例行检查任务，发现异常时报警。
1. 三套数据库备份和查看系统是否能正常工作。
2. 网站是否能正常访问
3. 系统性能是否正常
strchr的排序搜索优化版


## 已完成的工作

2024-07-10<br>
write logs<br>
multi-thread test<br>

2024-07-09<br>
添加全民兼职业务员页面<br>

2024-07-08<br>
implement json parse<br>
change buda.conf to json format<br>

2024-07-07<br>
implement memory management by Link across functions<br>
implement json save to file(object and array)<br>

2024-07-06<br>
design json structs, implement json save to file(int and string)<br>

2024-07-05<br>
implement Link (bi directions)<br>

2024-06-30<br>
write invalid user input into individual log file, and stop response
add a job

2024-06-29<br>
In order to share with all the people, change the readme from Chinese to English.<br>
add /ect/buda.conf as config file.<br>
change log files to /var/log/buda/<br>

2024年6月28日<br>
改进创业聚会模块<br>
日志查看功能完成90%<br>

2024年6月27日<br>
添加创业聚会模块<br>
日志查看功能完成80%<br>

2024年6月26日<br>
日志查看功能完成50%<br>

2024年6月25日<br>
使用动态内存分配器MemChain进行http response<br>

2024年6月23日<br>
使用systemctl控制网站的运行，保存日志文件<br>
人物传记整理工具添加事件日期锁定功能<br>

2024年6月22日<br>
实现http协议的GET /url功能，获取服务端的vue/dist中的静态文件。<br>
当服务端发现异常时，返回给用户vue/dist/index.html文件的内容。<br>
完成网站标题、菜单的现实，招聘信息和租房信息的列表页面。<br>
完成图片显示组件，可全屏观看、上一张、下一张图片。<br>
人物传记整理工具，完成30%<br>


## 联系我们，捐助即是投资

欢迎通过以下联系方式与我们联系，探讨本站的发展，捐助就是投资，即为我们的股东。<br>
微信: TobeBuda<br>
Email / 支付宝 / Paypal: jinmin.si@outlook.com<br>
CSDN：https://blog.csdn.net/qiuzen<br>
Github: https://github.com/SiJinmin<br>
微信公众号：立心成见<br>


