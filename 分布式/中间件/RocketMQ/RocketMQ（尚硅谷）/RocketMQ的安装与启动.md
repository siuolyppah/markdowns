# 单机安装与启动

## 下载RocketMQ与解压

- 直接在linux系统中使用wget

- 先下载，再上传至linux系统 ：

  [Downloading the Apache RocketMQ Releases - Apache RocketMQ](https://rocketmq.apache.org/dowloading/releases/)

  上传目录：/opt/tools

  解压至：/opt/apps

  ```sh
  unzip xxx.zip -d /opt/apps
  ```

  

## 修改初始内存

### 修改runserver.sh

```sh
vim bin/runserver.sh
```

![image-20220629133411298](RocketMQ%E7%9A%84%E5%AE%89%E8%A3%85%E4%B8%8E%E5%90%AF%E5%8A%A8.assets/image-20220629133411298.png)



### 修改runbroker.sh

```sh
vim bin/runbroker.sh
```

![image-20220629133440667](RocketMQ%E7%9A%84%E5%AE%89%E8%A3%85%E4%B8%8E%E5%90%AF%E5%8A%A8.assets/image-20220629133440667.png)



## 启动

### 启动NameServer

```sh
nohup sh bin/mqnamesrv &
tail -f ~/logs/rocketmqlogs/namesrv.log
# 日志内容：  The Name Server boot success...
```



### 启动broker

```sh
nohup sh bin/mqbroker -n localhost:9876 &
tail -f ~/logs/rocketmqlogs/broker.log
# 日志内容：	  The broker[%s, 172.30.30.233:10911] boot success...
```



## 发送/接收消息测试

- [官方Quick Start文档](https://rocketmq.apache.org/docs/quick-start/#:~:text=10911%5D%20boot%20success...-,Send%20%26%20Receive%20Messages,-Before%20sending/receiving)

```sh
export NAMESRV_ADDR=localhost:9876
sh bin/tools.sh org.apache.rocketmq.example.quickstart.Producer
# log: SendResult [sendStatus=SEND_OK, msgId= ...

sh bin/tools.sh org.apache.rocketmq.example.quickstart.Consumer
# log: ConsumeMessageThread_%d Receive New Messages: [MessageExt...
```



## 关闭

### 关闭broker

```sh
sh bin/mqshutdown broker
# log:
# The mqbroker(36695) is running...
# Send shutdown request to mqbroker(36695) OK
```



### 关闭Name Server

```sh
sh bin/mqshutdown namesrv
# log:
# The mqnamesrv(36664) is running...
# Send shutdown request to mqnamesrv(36664) OK
```



# 安装可视化控制台

RocketMQ有一个可视化的dashboard，通过该控制台可以直观的查看到很多数据。



## 下载

下载地址：https://github.com/apache/rocketmq-externals/releases  



## 修改配置

其为Spring Boot工程，修改其src/main/resources中的application.properties配置文件：

- 原来的端口号为8080，修改为一个不常用的（如7000）
- 指定RocketMQ的name server地址



## 添加依赖

在解压目录rocketmq-console的pom.xml中添加如下JAXB依赖：

>JAXB，Java Architechture for Xml Binding，用于XML绑定的Java技术，是一个业界标准，是一项可以根据XML Schema生成Java类的技术。  



```xml
<dependency>
    <groupId>javax.xml.bind</groupId>
    <artifactId>jaxb-api</artifactId>
    <version>2.3.0</version>
</dependency>
<dependency>
    <groupId>com.sun.xml.bind</groupId>
    <artifactId>jaxb-impl</artifactId>
    <version>2.3.0</version>
</dependency>
<dependency>
    <groupId>com.sun.xml.bind</groupId>
    <artifactId>jaxb-core</artifactId>
    <version>2.3.0</version>
</dependency>
<dependency>
    <groupId>javax.activation</groupId>
    <artifactId>activation</artifactId>
    <version>1.1.1</version>
</dependency>
```



## 使用maven打包

```cmd
mvn clean package -Dmaven.test.skip=true
```



## 启动

```sh
java -jar rocketmq-console-ng-1.0.0.jar
```



## 访问

http://localhost:7000/



# 集群搭建理论

## 数据复制与刷盘策略

复制与刷屏的区别：

![image-20220629145049490](RocketMQ%E7%9A%84%E5%AE%89%E8%A3%85%E4%B8%8E%E5%90%AF%E5%8A%A8.assets/image-20220629145049490.png)



### 复制策略

复制策略是Broker的Master与Slave间的数据同步方式。分为：

- 同步复制：消息写入master后，master会等待slave同步数据成功后才向producer返回成功ACK
- 异步复制：消息写入master后，master立即向producer返回成功ACK，无需等待slave同步数据成功



### 刷盘策略

刷盘策略指的是broker中消息的落盘方式，即消息发送到broker内存后消息持久化到磁盘的方式。分为：

- 同步刷盘：当消息持久化到broker的磁盘后才算是消息写入成功。

- 异步刷盘：当消息写入到broker的内存后即表示消息写入成功，无需等待消息持久化到磁盘。

  >- 消息写入到Broker的内存，一般是写入PageCache
  >- 对于异步刷盘策略，消息会写入到PageCache后立即返回成功ACK。但并不会立即做落盘操作，而是当PageCache到达一定量时会自动进行落盘。  



## Broker集群模式

根据Broker集群中各个节点间关系的不同，Broker集群可以分为以下几类：



### 单Master

- 只有一个broker（其本质上就不能称为集群）。

- 这种方式也只能是在测试时使用，生产环境下不能使用，因为存在单点问题。  



### 多Master

- broker集群仅由多个master构成，不存在Slave。
- 同一Topic的各个Queue会平均分布在各个master节点上。  



- 优点：

  配置简单，单个Master宕机或重启维护对应用无影响，在磁盘配置为RAID10时，即使机器宕机不可恢复情况下，由于RAID10磁盘非常可靠，消息也不会丢（异步刷盘丢失少量消息，同步刷盘一条不丢），性能最高；

  >前提是，这些Master都配置了RAID磁盘阵列。如果没有配置，一旦出现某Master宕机，则会发生大量消息丢失的情况。  

- 缺点：

  单台机器宕机期间，这台机器上未被消费的消息在机器恢复之前不可订阅（不可消费），消息实时性会受到影响。  



### 多Master多Slave模式-异步复制

- broker集群由多个master构成，每个master又配置了多个slave（在配置了RAID磁盘阵列的情况下，一个master一般配置一个slave即可）。
- master与slave的关系是主备关系，即master负责处理消息的读写请求，而slave仅负责消息的备份与master宕机后的角色切换。
- 异步复制即前面所讲的复制策略中的异步复制策略，即消息写入master成功后，master立即向producer返回成功ACK，无需等待slave同步数据成功。



- 该模式的最大特点之一是，当master宕机后slave能够自动切换为master。不过由于slave从master的同步具有短暂的延迟（毫秒级），所以当master宕机后，这种异步复制方式可能会存在少量消息的丢失问题。

  >Slave从Master同步的延迟越短，其可能丢失的消息就越少



### 多Master多Slave模式-同步双写

- 该模式是多Master多Slave模式的同步复制实现。
- 所谓同步双写，指的是消息写入master成功后，master会等待slave同步数据成功后才向producer返回成功ACK，即master与slave都要写入成功后才会返回成功ACK，也即双写。  



- 该模式与异步复制模式相比，优点是消息的安全性更高，不存在消息丢失的情况。但单个消息的RT略高，从而导致性能要略低（大约低10%）。  
- 该模式存在一个大的问题：对于目前的版本，Master宕机后，Slave不会自动切换到Master。



### 最佳实践

一般会为Master配置RAID10磁盘阵列，然后再为其配置一个Slave（异步复制）。即利用了RAID10磁盘阵列的高效、安全性，又解决了可能会影响订阅的问题。  

>- 多Master+RAID阵列，其仅仅可以保证数据不丢失，即不影响消息写入，但其可能会影响到消息的订阅。但其执行效率要远高于 多Master多Slave集群  
>- 多Master多Slave集群，其不仅可以保证数据不丢失，也不会影响消息写入。其运行效率要低于多Master+RAID阵列  



# 搭建Broker集群

## 集群架构

搭建一双主双从异步复制的Broker集群。

为了方便，使用两台主机来完成集群的搭建。这两台主机的功能与broker角色分配如下表：

![image-20220629151817515](RocketMQ%E7%9A%84%E5%AE%89%E8%A3%85%E4%B8%8E%E5%90%AF%E5%8A%A8.assets/image-20220629151817515.png)



![image-20220629151639157](RocketMQ%E7%9A%84%E5%AE%89%E8%A3%85%E4%B8%8E%E5%90%AF%E5%8A%A8.assets/image-20220629151639157.png)



## 修改rocketmqOS1的配置文件

1. 进入conf/2m-2s-async目录

2. 修改broker-a.properties：

   ```properties
   # 指定整个broker集群的名称，或者说是RocketMQ集群的名称
   brokerClusterName=DefaultCluster
   # 指定master-slave集群的名称。一个RocketMQ集群可以包含多个master-slave集群
   brokerName=broker-a
   # master的brokerId为0
   brokerId=0# 指定删除消息存储过期文件的时间为凌晨4点
   deleteWhen=04
   # 指定未发生更新的消息存储文件的保留时长为48小时，48小时后过期，将会被删除
   fileReservedTime=48
   # 指定当前broker为异步复制master
   brokerRole=ASYNC_MASTER
   # 指定刷盘策略为异步刷盘
   flushDiskType=ASYNC_FLUSH
   # 指定Name Server的地址
   namesrvAddr=192.168.59.164:9876;192.168.59.165:9876
   # 指定删除消息存储过期文件的时间为凌晨4点
   deleteWhen=04
   # 指定未发生更新的消息存储文件的保留时长为48小时，48小时后过期，将会被删除
   fileReservedTime=48
   # 指定当前broker为异步复制master
   brokerRole=ASYNC_MASTER
   # 指定刷盘策略为异步刷盘
   flushDiskType=ASYNC_FLUSH
   # 指定Name Server的地址
   namesrvAddr=192.168.59.164:9876;192.168.59.165:9876
   ```

3. 修改broker-b-s.properties：

   ```properties
   brokerClusterName=DefaultCluster
   # 指定这是另外一个master-slave集群
   brokerName=broker-b
   # slave的brokerId为非0
   brokerId=1
   deleteWhen=04
   fileReservedTime=48
   # 指定当前broker为slave
   brokerRole=SLAVE
   flushDiskType=ASYNC_FLUSH
   namesrvAddr=192.168.59.164:9876;192.168.59.165:9876
   # 指定Broker对外提供服务的端口，即Broker与producer与consumer通信的端口。默认10911。由于当前主机同时充当着master1与slave2，而前面的master1使用的是默认端口。这里需要将这两个端口加以区分，以区分出master1与slave2
   listenPort=11911
   # 指定消息存储相关的路径。默认路径为~/store目录。由于当前主机同时充当着master1与slave2，master1使用的是默认路径，这里就需要再指定一个不同路径
   storePathRootDir=~/store-s
   storePathCommitLog=~/store-s/commitlog
   storePathConsumeQueue=~/store-s/consumequeue
   storePathIndex=~/store-s/index
   storeCheckpoint=~/store-s/checkpoint
   abortFile=~/store-s/abort
   ```

   

## 修改rocketmqOS2的配置文件

1. broker-b.properties：

   ```properties
   brokerClusterName=DefaultCluster
   brokerName=broker-b
   brokerId=0
   deleteWhen=04
   fileReservedTime=48
   brokerRole=ASYNC_MASTER
   flushDiskType=ASYNC_FLUSH
   namesrvAddr=192.168.59.164:9876;192.168.59.165:9876
   ```

2. broker-a-s.properties：

   ```properties
   brokerClusterName=D
   brokerName=broker-a
   brokerId=1
   deleteWhen=04
   fileReservedTime=48
   brokerRole=SLAVE
   flushDiskType=ASYNC
   namesrvAddr=192.168.59.164:9876;192.168.59.165:9876
   listenPort=11911
   storePathRootDir=~/store-s
   storePathCommitLog=~/store-s/commitlog
   storePathConsumeQueue=~/store-s/consumequeue
   storePathIndex=~/store-s/index
   storeCheckpoint=~/store-s/checkpoint
   abortFile=~/store-s/abort
   ```

   

## 启动服务器

### 启动NameServer集群

分别启动rocketmqOS1与rocketmqOS2两个主机中的NameServer。启动命令完全相同。

```sh
nohup sh bin/mqnamesrv &
tail -f ~/logs/rocketmqlogs/namesrv.log
# 日志内容：  The Name Server boot success...
```



### 启动两个Master

分别启动rocketmqOS1与rocketmqOS2两个主机中的broker master。

注意，它们指定所要加载的配置文件是不同的。  

```sh
nohup sh bin/mqbroker -c conf/2m-2s-async/broker-a.properties &
tail -f ~/logs/rocketmqlogs/broker.log
```

```sh
nohup sh bin/mqbroker -c conf/2m-2s-async/broker-b.properties &
tail -f ~/logs/rocketmqlogs/broker.log
```



### 启动两个Slave

分别启动rocketmqOS1与rocketmqOS2两个主机中的broker slave。注意，它们指定所要加载的配置文件是不同的。  



```sh
nohup sh bin/mqbroker -c conf/2m-2s-async/broker-b-s.properties &
tail -f ~/logs/rocketmqlogs/broker.log
```

```sh
nohup sh bin/mqbroker -c conf/2m-2s-async/broker-a-s.properties &
tail -f ~/logs/rocketmqlogs/broker.log
```



## mqadmin命令

在mq解压目录的bin目录下有一个`mqadmin`命令，该命令是一个运维指令，用于对mq的主题，集群，broker 等信息进行管理。  



### 修改bin/tools.sh

修改mq解压目录下bin/tools.sh配置的JDK的ext目录位置：

![image-20220629162119527](RocketMQ%E7%9A%84%E5%AE%89%E8%A3%85%E4%B8%8E%E5%90%AF%E5%8A%A8.assets/image-20220629162119527.png)



### 该命令的官网详解

https://github.com/apache/rocketmq/blob/master/docs/cn/operation.md  