>《Docker 技术入门与实战第二版》



# 什么是容器

一句话概括容器：容器就是将软件打包成标准化单元，以用于开发、交付和部署。

- **容器镜像是轻量的、可执行的独立软件包** ，包含软件运行所需的所有内容：代码、运行时环境、系统工具、系统库和设置。
- **容器化软件适用于基于 Linux 和 Windows 的应用，在任何环境中都能够始终如一地运行。**
- **容器赋予了软件独立性**，使其免受外在环境差异（例如，开发和预演环境的差异）的影响，从而有助于减少团队间在相同基础设施上运行不同软件时的冲突。



# 容器与虚拟机

-  容器虚拟化的是操作系统而不是硬件，容器之间是共享同一套操作系统资源的。

- 虚拟机技术是虚拟出一套硬件后，在其上运行一个完整操作系统。因此容器的隔离级别会稍低一些。

![img](Docker%E5%9F%BA%E6%9C%AC%E6%A6%82%E5%BF%B5.assets/2e2b95eebf60b6d03f6c1476f4d7c697.png)

>容器内的应用进程直接运行于宿主的内核，容器内没有自己的内核，而且也没有进行硬件虚拟。



# Docker的基本概念

- **镜像（Image）**
- **容器（Container）**
- **仓库（Repository）**



## 镜像

>一个特殊的文件系统

**操作系统分为内核和用户空间**。对于 Linux 而言，内核启动后，会挂载 root 文件系统为其提供用户空间支持。而 Docker 镜像（Image），就相当于是一个 root 文件系统。



**Docker 镜像是一个特殊的文件系统，除了提供容器运行时所需的程序、库、资源、配置等文件外，还包含了一些为运行时准备的一些配置参数（如匿名卷、环境变量、用户等）。** 镜像不包含任何动态数据，其内容在构建之后也不会被改变。



## 容器

> 镜像运行时的实体



镜像（Image）和容器（Container）的关系，就像是面向对象程序设计中的 类 和 实例 一样，镜像是静态的定义，**容器是镜像运行时的实体。容器可以被创建、启动、停止、删除、暂停等** 。



**容器的实质是*进程*，但与直接在宿主执行的进程不同，容器进程运行于属于自己的独立的命名空间**。前面讲过镜像使用的是分层存储，容器也是如此**。**

容器存储层的生存周期和容器一样，容器消亡时，容器存储层也随之消亡。**因此，任何保存于容器存储层的信息都会随容器删除而丢失**。



按照 Docker 最佳实践的要求，**容器不应该向其存储层内写入任何数据** ，容器存储层要保持无状态化。**所有的文件写入操作，都应该使用数据卷（Volume）、或者绑定宿主目录**，在这些位置的读写会跳过容器存储层，直接对宿主(或网络存储)发生读写，其性能和稳定性更高。数据卷的生存周期独立于容器，容器消亡，数据卷不会消亡。因此， **使用数据卷后，容器可以随意删除、重新 run ，数据却不会丢失。**



## 仓库

> 集中存放镜像文件的地方



镜像构建完成后，可以很容易的在当前宿主上运行，但是， **如果需要在其它服务器上使用这个镜像，我们就需要一个集中的存储、分发镜像的服务，Docker Registry 就是这样的服务。**



一个 Docker Registry 中可以包含多个仓库（Repository）；每个仓库可以包含多个标签（Tag）；每个标签对应一个镜像。

通常，**一个仓库会包含同一个软件不同版本的镜像**，而**标签就常用于对应该软件的各个版本** 。

我们可以通过`<仓库名>:<标签>`的格式来指定具体是这个软件哪个版本的镜像。如果不给出标签，将以 latest 作为默认标签.。



> 最常使用的 Registry 公开服务是官方的 **Docker Hub**
>
> [https://hub.docker.com/](https://gitee.com/link?target=https%3A%2F%2Fhub.docker.com%2F) 
>
>  [时速云镜像库](https://gitee.com/link?target=https%3A%2F%2Fwww.tenxcloud.com%2F)、[网易云镜像服务](https://gitee.com/link?target=https%3A%2F%2Fwww.163yun.com%2Fproduct%2Frepo)、[DaoCloud 镜像市场](https://gitee.com/link?target=https%3A%2F%2Fwww.daocloud.io%2F)、[阿里云镜像库](https://gitee.com/link?target=https%3A%2F%2Fwww.aliyun.com%2Fproduct%2Fcontainerservice%3Futm_content%3Dse_1292836)等



# 常见命令

## 基本命令

```sh
docker version # 查看docker版本
docker images # 查看所有已下载镜像，等价于：docker image ls 命令
docker container ls #	查看所有容器
docker ps #查看正在运行的容器
docker image prune # 清理临时的、没有被使用的镜像文件。-a, --all: 删除所有没有用的镜像，而不仅仅是临时文件；
```



## 拉取镜像

```sh
docker search mysql # 查看mysql相关镜像
docker pull mysql:5.7 # 拉取mysql镜像
docker image ls # 查看所有已下载镜像
```



## 删除镜像

1. 确保这个镜像没有被容器引用

   ```sh
   docker ps
   ```

   ```sh
   ➜  ~ docker ps
   CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                               NAMES
   c4cd691d9f80        mysql:5.7           "docker-entrypoint.s…"   7 weeks ago         Up 12 days          0.0.0.0:3306->3306/tcp, 33060/tcp   mysql
   ```

   mysql 正在被 id 为 c4cd691d9f80 的容器引用

2. 通过 `docker stop c4cd691d9f80` 或者 `docker stop mysql`暂停这个容器

3. 查看 mysql 镜像的 id

   ```sh
   ➜  ~ docker images
   REPOSITORY              TAG                 IMAGE ID            CREATED             SIZE
   mysql                   5.7                 f6509bac4980        3 months ago        373MB
   ```

4. 通过 IMAGE ID 或者 REPOSITORY 名字即可删除

   ```sh
   docker rmi f6509bac4980 #  或者 docker rmi mysql 
   ```

   



# Build Ship and Run

- **Build（构建镜像）** ： 镜像就像是集装箱包括文件以及运行环境等等资源。
- **Ship（运输镜像）** ：主机和仓库间运输，这里的仓库就像是超级码头一样。
- **Run （运行镜像）** ：运行的镜像就是一个容器，容器就是运行程序的地方。



# Docker数据卷

[docs/tools/docker/docker-in-action.md · SnailClimb/JavaGuide - Gitee.com](https://gitee.com/SnailClimb/JavaGuide/blob/main/docs/tools/docker/docker-in-action.md#docker-数据卷)