# 概述

## Netty 是什么？

```
Netty is an asynchronous event-driven network application framework
for rapid development of maintainable high performance protocol servers & clients.
```



Netty 是一个***<u>异步的</u>、<u>基于事件驱动的</u>网络应用框架***，用于快速开发可维护、高性能的网络服务器和客户端

> 异步，指的是多线程异步；而非异步IO



## Netty 的地位

Netty 在 Java 网络应用框架中的地位就好比：Spring 框架在 JavaEE 开发中的地位

以下的框架都使用了 Netty，因为它们有网络通信需求！

* Cassandra - nosql 数据库
* Spark - 大数据分布式计算框架
* Hadoop - 大数据分布式存储框架
* RocketMQ - ali 开源的消息队列
* ElasticSearch - 搜索引擎
* gRPC - rpc 框架
* Dubbo - rpc 框架
* Spring 5.x - flux api 完全抛弃了 tomcat ，使用 netty 作为服务器端



## Netty 的优势

* Netty vs NIO：

  > Netty是基于NI的。但基于原生的NIO进行开发，有以下难点:

  - 工作量大，bug 多

  * 需要自己构建协议
  * 解决 TCP 传输问题，如粘包、半包
  * epoll 空轮询导致 CPU 100%
  * Netty还对 API 进行增强，使之更易用，如 ThreadLocal =>FastThreadLocal，ByteBuffer => ByteBuf 

* Netty vs 其它网络应用框架

  * Mina 由 apache 维护，将来 3.x 版本可能会有较大重构，破坏 API 向下兼容性，Netty 的开发迭代更迅速，<u>API 更简洁、文档更优秀</u>(作者自述)
  * 久经考验（16年），Netty 版本
    * 2.x 2004
    * 3.x 2008
    * 4.x 2013
    * 5.x 已废弃（没有明显的性能提升，维护成本高）



# Hello World项目

## 目标

开发一个简单的服务器端和客户端

* 客户端向服务器端发送 hello, world
* 服务器仅接收，不返回



## 项目依赖

maven项目：

```xml
<dependency>
    <groupId>io.netty</groupId>
    <artifactId>netty-all</artifactId>
    <version>4.1.39.Final</version>
</dependency>
```



## 服务器端

```java
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.string.StringDecoder;

public class HelloServer {
    public static void main(String[] args) {

        // 1.服务器端的启动器：负责组装Netty组件，启动服务器
        new ServerBootstrap()
                // 2. 添加组件：一个EventLoop由一个Thread对象和一个Selector对象组成
                .group(new NioEventLoopGroup())
                // 3. 选择ServerSocketChannel的实现：NIO、OIO(BIO)、操作系统的特定实现
                .channel(NioServerSocketChannel.class)
                // 4. 决定worker(Netty中对应为child)，能执行的操作(handler)
                .childHandler(
                        // 5. Channel代表和客户端进行数据读写的通道，Initializer即初始化器。
                        //    其本身就是一个Handler，职责是添加其他的Handler
                        new ChannelInitializer<NioSocketChannel>() {
                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {
                        // 该方法调用于: 连接建立后
                        // 6. 添加具体的Handler：
                        //    StringDecoder：将传输过来的ByteBuf转换为字符串
                        //    ChannelInboundHandlerAdapter: 自定义的Handler
                        ch.pipeline().addLast(new StringDecoder());
                        ch.pipeline().addLast(new ChannelInboundHandlerAdapter() {
                            // 处理读事件，msg即为StringDecoder转换出的字符串
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                System.out.println(msg);
                            }
                        });
                    }
                })
                // 7.绑定监听端口
                .bind(8080);
    }
}
```



## 客户端

```java
public class HelloClient {

    public static void main(String[] args) throws InterruptedException {
        new Bootstrap()
                .group(new NioEventLoopGroup())
                .channel(NioSocketChannel.class)
                .handler(new ChannelInitializer<NioSocketChannel>() {
                    // 在连接建立后，被调用
                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {
                        // 编码器：字符串->ByteBuf
                        ch.pipeline().addLast(new StringEncoder());
                    }
                })
                // 连接到服务器
                .connect(new InetSocketAddress("localhost", 8080))
                .sync()
                .channel()
                // 向服务器发送数据
                .writeAndFlush("hello,world");
    }
}
```



## 流程梳理

![0040](Netty%E5%85%A5%E9%97%A8.assets/0040.png)

视频链接：[黑马程序员Netty全套教程，全网最全Netty深入浅出教程，Java网络编程的王者_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1py4y1E7oA?p=56&spm_id_from=pageDriver)



其中：

- 服务端的group()方法，添加的EventLoop分别负责多种事件。

  其中accept()事件由Netty自动处理

- 服务器端的childHandler()方法，只是添加了初始化器。

  **<u>但初始化器的方法，执行于连接建立后</u>**

- 客户端的sync()方法，是一个阻塞方法，将阻塞到连接建立

- 收发数据，都要经过handler的处理。

  在客户端的体现，即为"hello,world"字符串被编码为ByteBuf后发出



## 一些正确观念

>### 💡 提示
>
>一开始需要树立正确的观念
>
>* 把 channel 理解为数据的通道
>
>* 把 msg 理解为流动的数据，最开始输入是 ByteBuf，但经过 pipeline 的加工，会变成其它类型对象，最后输出又变成 ByteBuf
>
>* 把 handler 理解为数据的处理工序
>
>  * 工序有多道，合在一起就是 pipeline，pipeline 负责发布事件（读、读取完成...）传播给每个 handler， handler 对自己感兴趣的事件进行处理（重写了相应事件处理方法）
>  * handler 分 Inbound 和 Outbound 两类
>
>* 把 eventLoop 理解为处理数据的工人
>
>  * 工人可以管理多个 channel 的 io 操作，并且一旦工人负责了某个 channel，就要负责到底（绑定）。**主要是为了线程安全**。
>
>  * 工人既可以执行 io 操作，也可以进行任务处理，每位工人有任务队列，队列里可以堆放多个 channel 的待处理任务，任务分为普通任务、定时任务
>  * 工人按照 pipeline 顺序，依次按照 handler 的规划（代码）处理数据，可以为每道工序指定不同的工人



# 组件

## EventLoop

### EventLoop（事件循环）

EventLoop ==本质是一个单线程执行器（同时维护了一个 Selector）==，里面有 run 方法处理 Channel 上源源不断的 io 事件。



它的继承关系比较复杂：

* 一条线是继承自 j.u.c.ScheduledExecutorService，因此包含了线程池中所有的方法
* 另一条线是继承自 netty 自己的 OrderedEventExecutor，
  * 提供了 boolean inEventLoop(Thread thread) 方法判断一个线程是否属于此 EventLoop
  * 提供了 parent 方法来看看自己属于哪个 EventLoopGroup

<img src="Netty%E5%85%A5%E9%97%A8.assets/image-20220428211653707.png" alt="image-20220428211653707" style="zoom:50%;" />



### EventLoopGroup（事件循环组）

==EventLoopGroup 是一组 EventLoop==，Channel 一般会调用 EventLoopGroup 的 register 方法来绑定其中一个 EventLoop，后续这个 Channel 上的 io 事件都由此 EventLoop 来处理（保证了 io 事件处理时的线程安全）

> 即：***EventLoopGroup由一组EventLoop组成，Channel与其中一个EventLoop绑定，处理后续的IO事件***



其继承自 netty 自己的 EventExecutorGroup

* 实现了 Iterable 接口提供遍历 EventLoop 的能力
* 另有 next 方法获取集合中下一个 EventLoop

<img src="Netty%E5%85%A5%E9%97%A8.assets/image-20220428214234729.png" alt="image-20220428214234729" style="zoom:50%;" />



### NioEventLoop处理普通或定时任务

```java
@Slf4j
public class TestEventLoop {

    public static void main(String[] args) {
        // 1.创建事件循环组
        // NioEventLoopGroup能够处理：IO事件、提供普通任务、提交定时任务
        // DefaultEventLoopGroup能够处理：提供普通任务、提交定时任务
        EventLoopGroup group = new NioEventLoopGroup(2); //默认使用CPU线程数*2

        // 2.获取下一个事件循环对象（轮询实现）
        EventLoop next = group.next();

        // 3.执行普通任务（接收Runnable对象）
        group.next().submit(() -> {
            log.debug("eventLoop thread");
        });
        log.debug("main thread");

        // 4.执行定时任务
        group.next().scheduleAtFixedRate(()->{
            log.debug("scheduled Task");
        },0,1, TimeUnit.SECONDS);
    }
}
```

输出：

```
21:38:44 [DEBUG] [main] o.e.n.c.TestEventLoop - main thread
21:38:44 [DEBUG] [nioEventLoopGroup-2-1] o.e.n.c.TestEventLoop - eventLoop thread
21:38:44 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestEventLoop - scheduled Task
21:38:45 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestEventLoop - scheduled Task
21:38:46 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestEventLoop - scheduled Task
21:38:47 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestEventLoop - scheduled Task
21:38:48 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestEventLoop - scheduled Task
21:38:49 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestEventLoop - scheduled Task
21:38:50 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestEventLoop - scheduled Task
```



### 处理IO事件

#### Channel与EventLoop绑定

- 服务端：

  ```java
  @Slf4j
  public class EventLoopServer {
      public static void main(String[] args) {
          new ServerBootstrap()
                  .group(new NioEventLoopGroup(2))
                  .channel(NioServerSocketChannel.class)
                  .childHandler(new ChannelInitializer<NioSocketChannel>() {
  
                      @Override
                      protected void initChannel(NioSocketChannel ch) throws Exception {
                          ch.pipeline().addLast(new ChannelInboundHandlerAdapter() {
                              //此时msg为ByteBuf类型
                              @Override
                              public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                  ByteBuf buf = (ByteBuf) msg;
                                  log.debug(buf.toString(Charset.defaultCharset()));
                              }
                          });
                      }
                  })
                  .bind(8080);
      }
  }
  ```

- 客户端：

  ```java
  public class EventLoopClient {
  
      public static void main(String[] args) throws InterruptedException {
          Channel channel = new Bootstrap()
                  .group(new NioEventLoopGroup())
                  .channel(NioSocketChannel.class)
                  .handler(new ChannelInitializer<NioSocketChannel>() {
                      // 在连接建立后，被调用
                      @Override
                      protected void initChannel(NioSocketChannel ch) throws Exception {
                          // 编码器：字符串->ByteBuf
                          ch.pipeline().addLast(new StringEncoder());
                      }
                  })
                  // 连接到服务器
                  .connect(new InetSocketAddress("localhost", 8080))
                  .sync()
                  .channel();
  
          System.out.println(channel);
          System.out.println("");     //用调试模式，执行channel.writeAndFlush("hello")
      }
  }
  ```

- 输出如下：(以调试模式启动多个EventLoopClient)

  ```
  22:00:07 [DEBUG] [nioEventLoopGroup-2-1] o.e.n.c.EventLoopServer - 1
  22:00:22 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 2
  22:00:25 [DEBUG] [nioEventLoopGroup-2-1] o.e.n.c.EventLoopServer - 1
  22:00:26 [DEBUG] [nioEventLoopGroup-2-1] o.e.n.c.EventLoopServer - 1
  22:00:30 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 2
  22:00:31 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 2
  22:02:26 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 3
  22:02:30 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 3
  22:02:30 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 3
  ```

>可以看到两个工人（即EventLoopGroup中的某个线程）轮流处理 channel，但***工人与 channel 之间进行了绑定***，后续此Channel仍由同一个工人处理

![0042](Netty%E5%85%A5%E9%97%A8.assets/0042.png)



#### 职责分工细化

服务端做两个改进（客户端不变）：

1. 将accept事件与读写事件，交由不同的线程
2. ***将一些耗时操作，从nio工人执行，改为非nio工人执行***

```java
@Slf4j
public class EventLoopServer {
    public static void main(String[] args) {
        // 创建一个独立的EventLoopGroup,用于处理耗时操作
        EventLoopGroup group = new DefaultEventLoopGroup();


        new ServerBootstrap()
                // 第一个参数即为boos（负责处理ServerSocketChannel的accept事件）；
                // 第二个参数即为worker（负责处理SocketChannel的读写事件）
                .group(new NioEventLoopGroup(), new NioEventLoopGroup(2))
                .channel(NioServerSocketChannel.class)
                .childHandler(new ChannelInitializer<NioSocketChannel>() {

                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {

                        ch.pipeline().addLast("handler1", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                ByteBuf buf = (ByteBuf) msg;
                                log.debug(buf.toString(Charset.defaultCharset()));

                                ctx.fireChannelRead(msg);   //将消息传递给下一个handler
                            }
                        }).addLast(group, "handler2", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                //耗时操作，将由非worker的线程完成
                                Thread.sleep(1000);
                                
                                ByteBuf buf = (ByteBuf) msg;
                                log.debug(buf.toString(Charset.defaultCharset()));
                            }
                        });
                    }
                })
                .bind(8080);

    }
}
```

输出：

```
22:38:21 [DEBUG] [nioEventLoopGroup-4-1] o.e.n.c.EventLoopServer - 1
22:38:22 [DEBUG] [defaultEventLoopGroup-2-1] o.e.n.c.EventLoopServer - 1
22:38:27 [DEBUG] [nioEventLoopGroup-4-2] o.e.n.c.EventLoopServer - 2
22:38:28 [DEBUG] [defaultEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 2
22:38:30 [DEBUG] [nioEventLoopGroup-4-1] o.e.n.c.EventLoopServer - 3
22:38:31 [DEBUG] [defaultEventLoopGroup-2-3] o.e.n.c.EventLoopServer - 3
22:38:32 [DEBUG] [nioEventLoopGroup-4-1] o.e.n.c.EventLoopServer - 1
22:38:33 [DEBUG] [defaultEventLoopGroup-2-1] o.e.n.c.EventLoopServer - 1
22:38:33 [DEBUG] [nioEventLoopGroup-4-2] o.e.n.c.EventLoopServer - 2
22:38:34 [DEBUG] [defaultEventLoopGroup-2-2] o.e.n.c.EventLoopServer - 2
22:38:35 [DEBUG] [nioEventLoopGroup-4-1] o.e.n.c.EventLoopServer - 3
22:38:36 [DEBUG] [defaultEventLoopGroup-2-3] o.e.n.c.EventLoopServer - 3
```



可以发现，nio工人与非nio工人，也分别绑定了Channel：

![0041](Netty%E5%85%A5%E9%97%A8.assets/0041.png)



### 源码分析—handler如何切换EventLoop

> 即：上图中，第一行的h1和h2之间，是如何切换EventLoop的

[视频链接](https://www.bilibili.com/video/BV1py4y1E7oA?p=63&spm_id_from=pageDriver)



关键代码 `io.netty.channel.AbstractChannelHandlerContext#invokeChannelRead()`

```java
static void invokeChannelRead(final AbstractChannelHandlerContext next, Object msg) {
    final Object m = next.pipeline.touch(ObjectUtil.checkNotNull(msg, "msg"), next);
    // executor为下一个handler的EventLoop
    EventExecutor executor = next.executor();
    
    // 下一个 handler 的事件循环（executor）是否与当前的事件循环是同一个线程   
    if (executor.inEventLoop()) {
        // 直接执行
        next.invokeChannelRead(m);
    } 
    else {
        // 不是，将要执行的代码作为任务 提交 给下一个事件循环处理（换人）
        executor.execute(new Runnable() {
            @Override
            public void run() {
                next.invokeChannelRead(m);
            }
        });
    }
}
```









# 双向通信