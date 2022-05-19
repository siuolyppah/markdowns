# 、相关资料

- [视频链接](https://www.bilibili.com/video/BV1ov41187bq)
- [Nginx官网](http://nginx.org/)



# Nginx简介

## 反向代理

- 正向代理：

  ![image-20220518215431656](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220518215431656.png)

- 反向代理：

  ![image-20220518215436494](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220518215436494.png)



## Nginx优点

### (1)速度更快、并发更高

单次请求或者高并发请求的环境下，Nginx都会比其他Web服务器响应的速度更快。一方面在正常情况下，单次请求会得到更快的响应，另一方面，在高峰期(如有数以万计的并发请求)，Nginx比其他Web服务器更快的响应请求。Nginx之所以有这么高的并发处理能力和这么好的性能原因在于Nginx采用了多进程和I/O多路复用(epoll)的底层实现。

### (2)配置简单，扩展性强

Nginx的设计极具扩展性，它本身就是由很多模块组成，这些模块的使用可以通过配置文件的配置来添加。这些模块有官方提供的也有第三方提供的模块，如果需要完全可以开发服务自己业务特性的定制模块。

### (3)高可靠性

Nginx采用的是多进程模式运行，其中有一个master主进程和N多个worker进程，worker进程的数量我们可以手动设置，每个worker进程之间都是相互独立提供服务，并且master主进程可以在某一个worker进程出错时，快速去"拉起"新的worker进程提供服务。

### (4)热部署

现在互联网项目都要求以7*24小时进行服务的提供，针对于这一要求，Nginx也提供了热部署功能，即可以在Nginx不停止的情况下，对Nginx进行文件升级、更新配置和更换日志文件等功能。

### (5)成本低、BSD许可证

BSD是一个开源的许可证，世界上的开源许可证有很多，现在比较流行的有六种分别是GPL、BSD、MIT、Mozilla、Apache、LGPL。这六种的区别是什么，我们可以通过下面一张图来解释下：

![1585139995444](Nginx%E5%9F%BA%E7%A1%80.assets/1585139995444.png)

Nginx本身是开源的，我们不仅可以免费的将Nginx应用在商业领域，而且还可以在项目中直接修改Nginx的源码来定制自己的特殊要求。这些点也都是Nginx为什么能吸引无数开发者继续为Nginx来贡献自己的智慧和青春。

典型的产品：

- OpenRestry [Nginx+Lua]
- Tengine[淘宝]



## Nginx的功能特性及常用功能

Nginx提供的基本功能服务从大体上归纳为"基本HTTP服务"、“高级HTTP服务”和"邮件服务"等三大类。

### 基本HTTP服务

Nginx可以提供基本HTTP服务，可以作为HTTP代理服务器和反向代理服务器，支持通过缓存加速访问，可以完成简单的负载均衡和容错，支持包过滤功能，支持SSL等。

- 处理静态文件、处理索引文件以及支持自动索引；
- 提供反向代理服务器，并可以使用缓存加上反向代理，同时完成负载均衡和容错；
- 提供对FastCGI、memcached等服务的缓存机制，，同时完成负载均衡和容错；
- 使用Nginx的模块化特性提供过滤器功能。Nginx基本过滤器包括gzip压缩、ranges支持、chunked响应、XSLT、SSI以及图像缩放等。其中针对包含多个SSI的页面，经由FastCGI或反向代理，SSI过滤器可以并行处理。
- 支持HTTP下的安全套接层安全协议SSL.
- 支持基于加权和依赖的优先权的HTTP/2

### 高级HTTP服务

- 支持基于名字和IP的虚拟主机设置
- 支持HTTP/1.0中的KEEP-Alive模式和管线(PipeLined)模型连接
- 自定义访问日志格式、带缓存的日志写操作以及快速日志轮转。
- 提供3xx~5xx错误代码重定向功能
- 支持重写（Rewrite)模块扩展
- 支持重新加载配置以及在线升级时无需中断正在处理的请求
- 支持网络监控
- 支持FLV和MP4流媒体传输

### 邮件服务

Nginx提供邮件代理服务也是其基本开发需求之一，主要包含以下特性：

- 支持IMPA/POP3代理服务功能
- 支持内部SMTP代理服务功能

### Nginx常用的功能模块

- 静态资源部署

- Rewrite地址重写

  正则表达式

- 反向代理

- 负载均衡

  轮询、加权轮询、ip_hash、url_hash、fair

- Web缓存

- 环境部署

  如何搭建高可用的环境

- 用户认证模块

- ...



Nginx的核心文件：

- nginx二进制可执行文件
- nginx.conf配置文件
- error.log错误的日志记录
- access.log访问日志记录



# Nginx安装

>- Nginx源码在：/root/nginx/core/nginx-1.16.1
>- Nginx已安装在：/usr/local/nginx/



## 版本介绍

- 最近版本：[nginx: download](http://nginx.org/en/download.html)

- 所有版本：[Index of /download/ (nginx.org)](http://nginx.org/download/)

![image-20220518222032994](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220518222032994.png)



## linux系统相关配置

- linux2.6及以上内核

  > Nginx需要解决高并发压力问题需要使用到epoll

  使用`uname -a`命令来查询linux的内核版本：

- 确保虚拟机能联网：ping www.baidu.com

  ![image-20220518223535899](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220518223535899.png)

- 确认关闭防火墙

  >仅针对于<u>对linux系统的防火墙设置规则不太清楚</u>的，建议大家把防火墙都关闭掉，因为我们此次课程主要的内容是对Nginx的学习，把防火墙关闭掉，可以省掉后续Nginx学习过程中遇到的诸多问题

  关闭的方式有如下两种：

  1. `systemctl stop firewalld`：关闭运行的防火墙，系统重新启动后，防火墙将重新打开
  2. `systemctl disable firewalld`：永久关闭防火墙，，系统重新启动后，防火墙依然关闭

  - `systemctl status firewalld`：查看防火墙状态

- 确认停用selinux：

  selinux(security-enhanced linux),美国安全局对于强制访问控制的实现，在linux2.6内核以后的版本中，selinux已经成功内核中的一部分。可以说selinux是linux史上最杰出的新安全子系统之一。虽然有了selinux，我们的系统会更安全，但是对于我们的学习Nginx的历程中，会**多很多设置**，所以这块建议大家将selinux进行关闭。

  `sestatus`命令查看状态：

  ![image-20220518223137252](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220518223137252.png)

  如果查看不是disabled状态，我们可以通过修改配置文件来进行设置,修改SELINUX=disabled，然后重启下系统即可生效。

  ```
  vim /etc/selinux/config
  ```

  ![image-20220518223159288](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220518223159288.png)



## 源码安装与yum安装

Nginx的安装方式可分为两种：

1. 通过Nginx源码安装
   - 通过Nginx源码的简单安装
   - 通过Nginx源码的复杂安装
2. 通过yum源安装



### 准备安装环境

通过源码安装，需要准备的环境：

1. GCC编译器

   - Nginx是使用C语言编写的程序，因此想要运行Nginx就需要安装一个编译工具。GCC就是一个开源的编译器集合，用于处理各种各样的语言，其中就包含了C语言。
   - 使用命令`yum install -y gcc`来安装
   - 安装成功后，可以通过`gcc --version`来查看gcc是否安装成功

2. PCRE

   - Nginx在编译过程中需要使用到PCRE库（perl Compatible Regular Expressoin 兼容正则表达式库)，因为在Nginx的Rewrite模块和http核心模块都会使用到PCRE正则表达式语法。

   - 可以使用命令`yum install -y pcre pcre-devel`来进行安装
   - 安装成功后，可以通过`rpm -qa pcre pcre-devel`来查看是否安装成功

3. zlib

   - zlib库提供了开发人员的压缩算法，在Nginx的各个模块中需要使用gzip压缩，所以我们也需要提前安装其库及源代码zlib和zlib-devel
   - 可以使用命令`yum install -y zlib zlib-devel`来进行安装
   - 安装成功后，可以通过`rpm -qa zlib zlib-devel`来查看是否安装成功

4. OpenSSL

   - OpenSSL是一个开放源代码的软件库包，应用程序可以使用这个包进行安全通信，并且避免被窃听。
   - SSL:Secure Sockets Layer安全套接协议的缩写，可以在Internet上提供秘密性传输，其目标是保证两个应用间通信的保密性和可靠性。在Nginx中，如果服务器需要提供安全网页时就需要用到OpenSSL库，所以我们需要对OpenSSL的库文件及它的开发安装包进行一个安装。

   - 可以使用命令`yum install -y openssl openssl-devel`来进行安装

   - 安装成功后，可以通过`rpm -qa openssl openssl-devel`来查看是否安装成功

>上述命令，一个个来的话比较麻烦，我们也可以通过一条命令来进行安装
>
>`yum install -y gcc pcre pcre-devel zlib zlib-devel openssl openssl-devel`进行全部安装。



### 通过源码简单安装

1. 进入官网查找需要下载版本（1.16.1）的[链接地址](http://nginx.org/en/download.html)，然后使用wget命令进行下载

   ```sh
   wget http://nginx.org/download/nginx-1.16.1.tar.gz
   ```

2. 将下载的资源进行包管理

   ```sh
   mkdir -p nginx/core
   mv nginx-1.16.1.tar.gz nginx/core
   ```

3. 解压缩

   ```sh
   tar -xzf nginx-1.16.1.tar.gz
   ```

4. 进入资源文件中，执行configure文件

   ```sh
   ./configure
   ```

5. 编译

   ```sh
   make
   ```

6. 安装

   ```sh
   make install
   ```

> 默认安装到：/usr/local/nginx/



### 关于源码的目录说明

执行`tar -zxvf nginx-1.16.1.tar.gz`对下载的资源进行解压缩，进入压缩后的目录，可以看到如下结构：

![image-20220519160850837](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220519160850837.png)

- auto:存放的是编译相关的脚本

- CHANGES:版本变更记录

- CHANGES.ru:俄语的版本变更记录

- conf:nginx默认的配置文件

- configure:nginx软件的自动脚本程序,是一个比较重要的文件，作用如下：

  1. 检测环境及根据环境检测结果生成C代码
  2. 生成编译代码需要的Makefile文件

- contrib:存放的是几个特殊的脚本文件，其中README中对脚本有着详细的说明

- html:存放的是Nginx自带的两个html页面，访问Nginx的首页和错误页面

- LICENSE:许可证的相关描述文件

- man:nginx的man手册

- README:Nginx的阅读指南

- src:Nginx的源代码

  

### 通过yum安装

使用源码进行简单安装，我们会发现安装的过程比较繁琐，需要提前准备GCC编译器、PCRE兼容正则表达式库、zlib压缩库、OpenSSL安全通信的软件库包，然后才能进行Nginx的安装。



1. 安装yum-utils

   ```sh
   sudo yum  install -y yum-utils
   ```

2. 添加yum源文件

   ```sh
   vim /etc/yum.repos.d/nginx.repo
   ```

   ```
   [nginx-stable]
   name=nginx stable repo
   baseurl=http://nginx.org/packages/centos/$releasever/$basearch/
   gpgcheck=1
   enabled=1
   gpgkey=https://nginx.org/keys/nginx_signing.key
   module_hotfixes=true
   
   [nginx-mainline]
   name=nginx mainline repo
   baseurl=http://nginx.org/packages/mainline/centos/$releasever/$basearch/
   gpgcheck=1
   enabled=0
   gpgkey=https://nginx.org/keys/nginx_signing.key
   module_hotfixes=true
   ```

3. 查看是否安装成功

   ```sh
   yum list | grep nginx
   ```

4. 使用yum进行安装

   ```sh
   yun install -y nginx
   ```

5. 查看nginx的安装位置

   ```sh
   whereis nginx
   ```

6. 启动测试



### 简单安装与yun安装的区别

这里先介绍一个命令: `./nginx -V`,通过该命令可以查看到所安装Nginx的版本及相关配置信息。

- 简单安装：

  ![image-20220519155620822](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220519155620822.png)

- yum安装：

  ![image-20220519155626145](Nginx%E5%9F%BA%E7%A1%80.assets/image-20220519155626145.png)



### 通过源码的复杂安装

与简单安装的不同之处在于，通过`./configure`来对编译参数进行设置。

查看所有可选的配置项：

```sh
./configure --help
```



可简单分为：

- PATH相关
- --with开头：用于配置启用模块
- --without开头：用于配置关闭模块



常见的配置：

- --prefix=PATH

  ```
  指向Nginx的安装目录，默认值为/usr/local/nginx   
  ```

- --sbin-path=PATH

  ```
  指向可执行程序文件(nginx)的路径,默认值为<prefix>/sbin/nginx
  ```

- --modules-path=PATH

  ```
  指向Nginx动态模块安装目录，默认值为<prefix>/modules
  ```

- --conf-path=PATH

  ```
  指向配置文件(nginx.conf)的路径,默认值为<prefix>/conf/nginx.conf
  ```

- --error-log-path=PATH 

  ```
  指向错误日志文件的路径,默认值为<prefix>/logs/error.log
  ```

- --http-log-path=PATH  

  ```
  指向访问日志文件的路径,默认值为<prefix>/logs/access.log
  ```

- --pid-path=PATH

  ```
  指向记录Nginx启动后进程ID的文件路径，默认值为<prefix>/logs/nginx.pid
  ```

- --lock-path=PATH

  ```
  指向Nginx锁文件的存放路径,默认值为<prefix>/logs/nginx.lock
  ```



要想使用可以通过如下命令

```
./configure --prefix=/usr/local/nginx \
--sbin-path=/usr/local/nginx/sbin/nginx \
--modules-path=/usr/local/nginx/modules \
--conf-path=/usr/local/nginx/conf/nginx.conf \
--error-log-path=/usr/local/nginx/logs/error.log \
--http-log-path=/usr/local/nginx/logs/access.log \
--pid-path=/usr/local/nginx/logs/nginx.pid \
--lock-path=/usr/local/nginx/logs/nginx.lock
```



> 关于卸载已安装的Nginx：
>
> 1. 将nginx的进程关闭
>
>    `./nginx -s stop`
>
> 2. 将安装的nginx进行删除
>
>    `rm -rf /usr/local/nginx`
>
> 3. 将安装包之前编译的环境清除掉
>
>    `make clean`



# Nginx的目录结构

- 安装tree工具：

  ```sh
  yum install -y tree
  ```

- 使用tree命令：

  ```sh
  tree /usr/local/nginx
  ```

  ```
  /usr/local/nginx
  ├── client_body_temp
  ├── conf
  │   ├── fastcgi.conf
  │   ├── fastcgi.conf.default
  │   ├── fastcgi_params
  │   ├── fastcgi_params.default
  │   ├── koi-utf
  │   ├── koi-win
  │   ├── mime.types
  │   ├── mime.types.default
  │   ├── nginx.conf
  │   ├── nginx.conf.default
  │   ├── scgi_params
  │   ├── scgi_params.default
  │   ├── uwsgi_params
  │   ├── uwsgi_params.default
  │   └── win-utf
  ├── fastcgi_temp
  ├── html
  │   ├── 50x.html
  │   └── index.html
  ├── logs
  │   ├── access.log
  │   ├── error.log
  │   └── nginx.pid
  ├── proxy_temp
  ├── sbin
  │   └── nginx
  ├── scgi_temp
  └── uwsgi_temp
  ```



>  CGI(Common Gateway Interface)通用网关【接口】：
>
>  主要解决的问题是从客户端发送一个请求和数据，服务端获取到请求和数据后可以调用调用CGI【程序】处理及相应结果给客户端的一种标准规范。



- mime.types：记录HTTP协议中，Content-Type的值和文件后缀名的对应关系
- nginx.conf：Nginx的核心配置文件
- koi-utf、koi-win、win-utf：这三个文件都是与编码转换映射相关的配置文件，用来将一种编码转换成另一种编码



# Nginx服务器的启停命令

[黑马程序员Nginx教程，Java进阶从0到1学会Nginx分布式框架_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1ov41187bq?p=15&spm_id_from=pageDriver)