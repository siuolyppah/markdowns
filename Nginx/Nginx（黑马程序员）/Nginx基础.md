# 相关资料

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



# Nginx环境准备

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



## Nginx安装

[黑马程序员Nginx教程，Java进阶从0到1学会Nginx分布式框架_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1ov41187bq?p=8&spm_id_from=pageDriver)