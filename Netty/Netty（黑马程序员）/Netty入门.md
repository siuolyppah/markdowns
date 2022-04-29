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



# Hello World案例

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



## Channel

channel 的主要作用：

* close() 可以用来关闭 channel
* closeFuture() 用来处理 channel 的关闭
  * sync 方法作用是同步等待 channel 关闭
  * 而 addListener 方法是异步等待 channel 关闭
* pipeline() 方法添加处理器
* write() 方法将数据写入
* writeAndFlush() 方法将数据写入并刷出



### ChannelFuture类

#### connect()、sync()、addListener()

将客户端的代码做拆分（不再链式编程）

```java
@Slf4j
public class EventLoopClient {

    public static void main(String[] args) throws InterruptedException {
        ChannelFuture channelFuture = new Bootstrap()
                .group(new NioEventLoopGroup())
                .channel(NioSocketChannel.class)
                .handler(new ChannelInitializer<NioSocketChannel>() {
                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {
                        ch.pipeline().addLast(new StringEncoder());
                    }
                })
                .connect(new InetSocketAddress("localhost", 8080));

//        channelFuture.sync();
        Channel channel = channelFuture.channel();
        log.debug("{}", channel);       //13:42:49 [DEBUG] [main] o.e.n.c.EventLoopClient - [id: 0xca63d606]

        channel.writeAndFlush("hello,world");
    }
}
```

会发现，当sync被注释掉后，服务端并不会正确的受到数据。



原因在于：

1. ***connect()方法是异步非阻塞的***，也就是说：

   主线程调用connect()方法后立即返回，由另一个线程（NIO线程）真正执行连接操作

2. 主线程接下来调用channel()方法，但此时获取到的，是未建立好连接的channel



解决方案：

- 调用ChannelFuture的sync()方法，来***==同步==***处理结果：

  ***主线程在调用sync()方法后被阻塞，直至nio线程将连接建立***

  ```java
  channelFuture.sync();
  Channel channel = channelFuture.channel();
  channel.writeAndFlush("hello,world");
  ```

- 调用ChannelFuture的addListener(回调对象)方法，来***==异步==***处理结果：

  主线程在调用addListener()后立即返回，NIO线程将连接建立后，调用回调对象的operationComplete()方法

  ```java
  channelFuture.addListener(new ChannelFutureListener() {
      @Override
      public void operationComplete(ChannelFuture future) throws Exception {
          Channel channel = channelFuture.channel();
          log.debug("{}", channel);   //13:55:24 [DEBUG] [nioEventLoopGroup-2-1] o.e.n.c.EventLoopClient - [id: 0x801b5e39, L:/127.0.0.1:60835 - R:localhost/127.0.0.1:8080]
          channel.writeAndFlush("hello,world");
      }
  });
  ```

  

### Channel类

#### close()、closeFuture()

- close方法是异步的，即：真正执行close业务逻辑的是一个线程（NIO线程）。

- closeFuture()方法，将返回一个CloseFuture对象，用它可以进行善后工作

  - 同步方式：

    ```java
    @Slf4j
    public class EventLoopClient {
    
        public static void main(String[] args) throws InterruptedException {
            ChannelFuture channelFuture = new Bootstrap()
                    .group(new NioEventLoopGroup())
                    .channel(NioSocketChannel.class)
                    .handler(new ChannelInitializer<NioSocketChannel>() {
                        @Override
                        protected void initChannel(NioSocketChannel ch) throws Exception {
                            ch.pipeline().addLast(new LoggingHandler(LogLevel.DEBUG));
                            ch.pipeline().addLast(new StringEncoder());
                        }
                    })
                    .connect(new InetSocketAddress("localhost", 8080));
    
            Channel channel = channelFuture.sync().channel();
    
            new Thread(() -> {
                Scanner scanner = new Scanner(System.in);
                while (true) {
                    String line = scanner.nextLine();
                    if ("q".equals(line)) {
                        channel.close();
                        break;
                    }
                    channel.writeAndFlush(line);
                }
    
            }, "input").start();
    
            ChannelFuture closeFuture = channel.closeFuture();
            closeFuture.sync(); //同步方式
            log.debug("channel已关闭，进行善后工作");
        }
    }
    ```

  - 异步方式：

    NIO线程将channel关闭后，调用回调对象的operationComplete()方法

    ```java
    @Slf4j
    public class EventLoopClient {
    
        public static void main(String[] args) throws InterruptedException {
            ChannelFuture channelFuture = new Bootstrap()
                    .group(new NioEventLoopGroup())
                    .channel(NioSocketChannel.class)
                    .handler(new ChannelInitializer<NioSocketChannel>() {
                        @Override
                        protected void initChannel(NioSocketChannel ch) throws Exception {
                            ch.pipeline().addLast(new LoggingHandler(LogLevel.DEBUG));
                            ch.pipeline().addLast(new StringEncoder());
                        }
                    })
                    .connect(new InetSocketAddress("localhost", 8080));
    
            Channel channel = channelFuture.sync().channel();
    
            new Thread(() -> {
                Scanner scanner = new Scanner(System.in);
                while (true) {
                    String line = scanner.nextLine();
                    if ("q".equals(line)) {
                        channel.close();
                        break;
                    }
                    channel.writeAndFlush(line);
                }
    
            }, "input").start();
    
            ChannelFuture closeFuture = channel.closeFuture();
            closeFuture.addListener(new ChannelFutureListener() {   //异步方式
                @Override
                public void operationComplete(ChannelFuture future) throws Exception {
                    log.debug("channel已关闭，进行善后工作");
                }
            });
        }
    }
    ```

    

#### 优雅关闭

Channel已关闭，但整个Java进程却未结束，原因在于：

***NioEventLoopGroup中的线程并未结束，此时调用NioEventLoopGroup的shutdownGracefully()方法即可***。



客户端：

```java
@Slf4j
public class EventLoopClient {

    public static void main(String[] args) throws InterruptedException {
        NioEventLoopGroup group = new NioEventLoopGroup();

        ChannelFuture channelFuture = new Bootstrap()
                .group(group)
                .channel(NioSocketChannel.class)
                .handler(new ChannelInitializer<NioSocketChannel>() {
                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {
                        ch.pipeline().addLast(new LoggingHandler(LogLevel.DEBUG));
                        ch.pipeline().addLast(new StringEncoder());
                    }
                })
                .connect(new InetSocketAddress("localhost", 8080));

        Channel channel = channelFuture.sync().channel();

        new Thread(() -> {
            Scanner scanner = new Scanner(System.in);
            while (true) {
                String line = scanner.nextLine();
                if ("q".equals(line)) {
                    channel.close();
                    break;
                }
                channel.writeAndFlush(line);
            }

        }, "input").start();

        ChannelFuture closeFuture = channel.closeFuture();
        closeFuture.addListener(new ChannelFutureListener() {   //异步方式
            @Override
            public void operationComplete(ChannelFuture future) throws Exception {
                log.debug("channel已关闭，进行善后工作");
                group.shutdownGracefully();
            }
        });
    }
}
```



## Future & Promise

在异步处理时，经常用到这两个接口

> 注意：
>
> - netty 中的 Future 与 jdk 中的 Future 同名
> - netty 的 Future 继承自 jdk 的 Future
> - netty 的 Promise 又对 netty Future 进行了扩展

* jdk Future 只能同步等待任务结束（或成功、或失败）才能得到结果

* netty Future 可以同步等待任务结束得到结果，也可以异步方式得到结果

  > 使用Netty Future时，Future对象的创建以及运行结果的设置，都不是可控的

* netty Promise 不仅有 netty Future 的功能，而且脱离了任务独立存在，只作为两个线程间传递结果的容器



|  功能/名称   |           jdk Future           |                         netty Future                         |   Promise    |
| :----------: | :----------------------------: | :----------------------------------------------------------: | :----------: |
|    cancel    |            取消任务            |                              -                               |      -       |
|  isCanceled  |          任务是否取消          |                              -                               |      -       |
|    isDone    | 任务是否完成，不能区分成功失败 |                              -                               |      -       |
|     get      |     获取任务结果，阻塞等待     |                              -                               |      -       |
|    getNow    |               -                |        获取任务结果，非阻塞，还未产生结果时返回 null         |      -       |
|    await     |               -                | 等待任务结束，如果任务失败，不会抛异常。还通过 isSuccess 判断任务是否成功 |      -       |
|     sync     |               -                |     等待任务结束（并不获取结果），如果任务失败，抛出异常     |      -       |
|  isSuccess   |               -                |                       判断任务是否成功                       |      -       |
|    cause     |               -                |         获取失败信息，非阻塞，如果没有失败，返回null         |      -       |
| addLinstener |               -                |                    添加回调，异步接收结果                    |      -       |
|  setSuccess  |               -                |                              -                               | 设置成功结果 |
|  setFailure  |               -                |                              -                               | 设置失败结果 |



### JDKFuture案例

```java
public class TestJDKFuture {

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        // 1.创建线程池
        ExecutorService service = Executors.newFixedThreadPool(2);

        // 2.提交任务
        Future<Integer> future = service.submit(new Callable<Integer>() {

            @Override
            public Integer call() throws Exception {
                Thread.sleep(1000);
                return 100;
            }
        });

        // 3.主线程通过future获取结果
        System.out.println(future.get());
    }
}
```



### NettyFuture案例

```java
@Slf4j
public class TestNettyFuture {

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        // 1.获取一个EventLoop对象
        NioEventLoopGroup group = new NioEventLoopGroup();
        EventLoop eventLoop = group.next();

        // 2.提交任务
        Future<Integer> future = eventLoop.submit(new Callable<Integer>() {

            @Override
            public Integer call() throws Exception {
                log.debug("computing");
                Thread.sleep(1000);
                return 100;
            }
        });

        // 3.主线程获取结果
        // 3.1同步阻塞获取
        log.debug("result:{}", future.get());
        // 3.2异步非阻塞获取
        future.addListener(new GenericFutureListener<Future<? super Integer>>() {
            @Override
            public void operationComplete(Future<? super Integer> future) throws Exception {
                log.debug("result:{}", future.getNow());   //此处没必要用get()
            }
        });
    }
}
```



### NettyPromise案例

```java
@Slf4j
public class TestNettyPromise {

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        // 1.准备EventLoop
        EventLoop eventLoop = new NioEventLoopGroup().next();

        // 2.主动创建Promise对象（Future对象是方法的返回值）
        DefaultPromise<Integer> promise = new DefaultPromise<>(eventLoop);

        new Thread(() -> {
            log.debug("computing");
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            // 3.执行计算后向promise对象填充结果
            promise.setSuccess(66);
        }).start();

        log.debug("waiting result");
        // 4.接收结果
        log.debug("result:{}", promise.get());
    }
}
```

```
15:26:34 [DEBUG] [main] o.e.n.c.TestNettyPromise - waiting result
15:26:34 [DEBUG] [Thread-0] o.e.n.c.TestNettyPromise - computing
15:26:35 [DEBUG] [main] o.e.n.c.TestNettyPromise - result:66
```



模拟出异常的情况：

```java
@Slf4j
public class TestNettyPromise {

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        EventLoop eventLoop = new NioEventLoopGroup().next();

        DefaultPromise<Integer> promise = new DefaultPromise<>(eventLoop);

        new Thread(() -> {
            log.debug("computing");
            try {
                Thread.sleep(1000);

                int i = 1 / 0;		//模拟出现异常

                promise.setSuccess(66);
            } catch (Exception e) {
                promise.setFailure(e);
                e.printStackTrace();
            }
        }).start();

        log.debug("waiting result");
        log.debug("result:{}", promise.get());
    }
}
```

```
Exception in thread "main" java.lang.ArithmeticException: / by zero
	at org.example.netty.c3.TestNettyPromise.lambda$main$0(TestNettyPromise.java:26)
	at java.base/java.lang.Thread.run(Thread.java:829)
java.util.concurrent.ExecutionException: java.lang.ArithmeticException: / by zero
	at io.netty.util.concurrent.AbstractFuture.get(AbstractFuture.java:41)
	at org.example.netty.c3.TestNettyPromise.main(TestNettyPromise.java:38)
Caused by: java.lang.ArithmeticException: / by zero
	at org.example.netty.c3.TestNettyPromise.lambda$main$0(TestNettyPromise.java:26)
	at java.base/java.lang.Thread.run(Thread.java:829)
```



## Handler & Pipline

### Handler的执行顺序

ChannelHandler 用来处理 Channel 上的各种事件，分为入站、出站两种。所有 ChannelHandler 被连成一串，就是 ==Pipeline==

> 即一组Handler，组成一个Pipeline
>
> 打个比喻，每个 Channel 是一个产品的加工车间，Pipeline 是车间中的流水线，ChannelHandler 就是流水线上的各道工序，而后面要讲的 ByteBuf 是原材料，经过很多工序的加工：先经过一道道入站工序，再经过一道道出站工序最终变成产品

- 入站处理器通常是 `ChannelInboundHandlerAdapter` 的子类，主要用来***读取客户端数据，写回结果***
- 出站处理器通常是 `ChannelOutboundHandlerAdapter` 的子类，主要***对写回结果进行加工***



服务端：
```java
@Slf4j
public class TestPipeline {

    public static void main(String[] args) {
        new ServerBootstrap()
                .group(new NioEventLoopGroup())
                .channel(NioServerSocketChannel.class)
                .childHandler(new ChannelInitializer<NioSocketChannel>() {
                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {
                        // 1.通过Channel获取Pipeline
                        ChannelPipeline pipeline = ch.pipeline();

                        // 2.添加处理器：head -> h1 -> h2 -> h3 -> h4 -> tail
                        // 2.1 入站处理器
                        pipeline.addLast("h1", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                log.debug("h1-read()");
                                super.channelRead(ctx, msg);
                            }
                        });
                        pipeline.addLast("h2", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                log.debug("h2-read()");
                                super.channelRead(ctx, msg);
                            }
                        });
                        pipeline.addLast("h3", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                log.debug("h3-read()");

                                // 用于令出站处理器工作
                                ch.writeAndFlush(ctx.alloc().buffer().writeBytes("some message to client".getBytes()));
                                super.channelRead(ctx, msg);
                            }
                        });

                        // 2.2 出站处理器
                        pipeline.addLast("h4", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h4-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                        pipeline.addLast("h5", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h5-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                        pipeline.addLast("h6", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h6-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                    }
                })
                .bind(8080);
    }
}
```



客户端：

```java
new Bootstrap()
    .group(new NioEventLoopGroup())
    .channel(NioSocketChannel.class)
    .handler(new ChannelInitializer<Channel>() {
        @Override
        protected void initChannel(Channel ch) {
            ch.pipeline().addLast(new StringEncoder());
        }
    })
    .connect("127.0.0.1", 8080)
    .addListener((ChannelFutureListener) future -> {
        future.channel().writeAndFlush("hello,world");
    });
```



服务端将有如下输出：
```
16:27:18 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h1-read()
16:27:18 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h2-read()
16:27:18 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h3-read()
16:27:18 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h6-write()
16:27:18 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h5-write()
16:27:18 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h4-write()
```



可以发现：

- ChannelInboundHandlerAdapter 是按照 addLast 的***顺序执行***的
- 而 ChannelOutboundHandlerAdapter 是按照 addLast 的***逆序执行***的

ChannelPipeline 的实现是一个 ChannelHandlerContext（包装了 ChannelHandler） 组成的***双向链表***：

![0008](Netty%E5%85%A5%E9%97%A8.assets/0008.png)



### Handler的协作

#### 入站处理器

服务端如下（客户端同上）：

```java
@Slf4j
public class TestPipeline {

    public static void main(String[] args) {
        new ServerBootstrap()
                .group(new NioEventLoopGroup())
                .channel(NioServerSocketChannel.class)
                .childHandler(new ChannelInitializer<NioSocketChannel>() {
                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {
                        ChannelPipeline pipeline = ch.pipeline();

                        pipeline.addLast("h1", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                ByteBuf buf = (ByteBuf) msg;
                                String str = buf.toString(Charset.defaultCharset());
                                log.debug("h1-read()获得的msg为：{}",msg);

                                super.channelRead(ctx, str);      
                            }
                        });
                        pipeline.addLast("h2", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                String str = (String) msg;
                                Student student = new Student(str);
                                log.debug("h2-read()获得的msg为：{}",msg);

                                super.channelRead(ctx, student);
                            }
                        });
                        pipeline.addLast("h3", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                Student student = (Student) msg;
                                log.debug("h3-read()获得的msg为：{}",student);

                                // 用于令出站处理器工作
                                ch.writeAndFlush(ctx.alloc().buffer().writeBytes("some message to client".getBytes()));

                                // 事实上，此处可以不再调用super，因为不存在下一个入站处理器
                                super.channelRead(ctx, msg);
                            }
                        });

                        pipeline.addLast("h4", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h4-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                        pipeline.addLast("h5", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h5-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                        pipeline.addLast("h6", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h6-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                    }
                })
                .bind(8080);
    }

    @Data
    @AllArgsConstructor
    static class Student{
        private String name;
    }
}
```

```
16:40:23 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h1-read()获得的msg为：PooledUnsafeDirectByteBuf(ridx: 0, widx: 8, cap: 1024)
16:40:23 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h2-read()获得的msg为：xiaoming
16:40:23 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h3-read()获得的msg为：TestPipeline.Student(name=xiaoming)
16:40:23 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h6-write()
16:40:23 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h5-write()
16:40:23 [DEBUG] [nioEventLoopGroup-2-2] o.e.n.c.TestPipeline - h4-write()
```



如何调用下一个入站处理器：

- `super.channelRead(ctx, msg);`
- 或`ctx.fireChannelRead(msg);` ，即super的内部逻辑



#### 出站处理器

```java
@Slf4j
public class TestPipeline {

    public static void main(String[] args) {
        new ServerBootstrap()
                .group(new NioEventLoopGroup())
                .channel(NioServerSocketChannel.class)
                .childHandler(new ChannelInitializer<NioSocketChannel>() {
                    @Override
                    protected void initChannel(NioSocketChannel ch) throws Exception {
                        ChannelPipeline pipeline = ch.pipeline();

                        pipeline.addLast("h1", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                log.debug("h1-read()");

                                super.channelRead(ctx, msg);
                            }
                        });
                        pipeline.addLast("h2", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                log.debug("h2-read()");

                                super.channelRead(ctx, msg);
                            }
                        });
                        pipeline.addLast("h3", new ChannelInboundHandlerAdapter() {
                            @Override
                            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                                log.debug("h3-read()");

                                // 用于令出站处理器工作：从tail向前寻找出站处理器
                                ch.writeAndFlush(ctx.alloc().buffer().writeBytes("some message to client".getBytes()));

                                // 易错点：
                                // ctx.writeAndFlush(ctx.alloc().buffer().writeBytes("some message to client".getBytes()));
                                // 此方法，将从当前的处理器向前寻找出站处理器
                            }
                        });

                        // 出站处理器
                        pipeline.addLast("h4", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h4-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                        pipeline.addLast("h5", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h5-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                        pipeline.addLast("h6", new ChannelOutboundHandlerAdapter() {
                            @Override
                            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                                log.debug("h6-write()");
                                super.write(ctx, msg, promise);
                            }
                        });
                    }
                })
                .bind(8080);
    }
}
```



>`一个易错点`：
>
>- ch.writeAndFlush()：从tail向前寻找出站处理器
>- ctx.writeAndFlush()：将从当前的处理器向前寻找出站处理器



### EmbeddedChannel

> EmbeddedChannel可以模拟数据的入站和出站

```java
@Slf4j
public class TestEmbeddedChannel {

    public static void main(String[] args) {

        // 入站处理器
        ChannelInboundHandlerAdapter h1 = new ChannelInboundHandlerAdapter() {
            @Override
            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                log.debug("h1-read()");
                super.channelRead(ctx, msg);
            }
        };
        ChannelInboundHandlerAdapter h2 = new ChannelInboundHandlerAdapter() {
            @Override
            public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
                log.debug("h2-read()");
                super.channelRead(ctx, msg);
            }
        };

        // 出站处理器
        ChannelOutboundHandlerAdapter h3 = new ChannelOutboundHandlerAdapter() {
            @Override
            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                log.debug("h3-write()");
                super.write(ctx, msg, promise);
            }
        };
        ChannelOutboundHandlerAdapter h4 = new ChannelOutboundHandlerAdapter() {
            @Override
            public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
                log.debug("h4-write()");
                super.write(ctx, msg, promise);
            }
        };

        EmbeddedChannel embeddedChannel = new EmbeddedChannel(h1, h2, h3, h4);

        // 模拟数据入站
        embeddedChannel.writeInbound(ByteBufAllocator.DEFAULT.buffer().writeBytes("some message from client".getBytes()));
        log.debug("------");
        // 模拟数据出站
        embeddedChannel.writeOutbound(ByteBufAllocator.DEFAULT.buffer().writeBytes("some message to client".getBytes()));
    }
}
```



## ByteBuf

> Netty的ByteBuf，是对NIO的ByteBuf的增强



### 创建

```java
ByteBuf buffer = ByteBufAllocator.DEFAULT.buffer();
```

上述代码创建了一个ByteBuf，其采用默认容量256B。



> ***Netty的ByteBuf，是可以自动扩容的***。

```java
@Slf4j
public class TestByteBuf {

    public static void main(String[] args) {
        ByteBuf buf = ByteBufAllocator.DEFAULT.buffer();     // 默认容量256
        
        log(buf);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 300; i++) {
            sb.append("a");
        }

        buf.writeBytes(sb.toString().getBytes());
        log(buf);
    }

    private static void log(ByteBuf buffer) {
        int length = buffer.readableBytes();
        int rows = length / 16 + (length % 15 == 0 ? 0 : 1) + 4;
        StringBuilder buf = new StringBuilder(rows * 80 * 2)
                .append("read index:").append(buffer.readerIndex())
                .append(" write index:").append(buffer.writerIndex())
                .append(" capacity:").append(buffer.capacity())
                .append(NEWLINE);
        appendPrettyHexDump(buf, buffer);
        System.out.println(buf.toString());
    }
}
```



### 直接内存 vs 堆内存

- 创建基于堆的ByteBuf：

  ```
  ByteBuf buffer = ByteBufAllocator.DEFAULT.heapBuffer(10);
  ```

- 创建基于直接内存的ByteBuf：

  ```java
  ByteBuf buffer = ByteBufAllocator.DEFAULT.directBuffer(10);
  ```

  >ByteBufAllocator.DEFAULT.buffer();获取的也是直接内存

  

直接内存与堆内存的比较：

* 直接内存创建和销毁的代价昂贵，但读写性能高（少一次内存复制），适合配合池化功能一起用
* 直接内存对 GC 压力小，因为这部分内存不受 JVM 垃圾回收的管理，但也要注意及时主动释放



### 池化 vs 非池化

***池化的最大意义在于可以重用 ByteBuf***，优点有

* 没有池化，则每次都得创建新的 ByteBuf 实例，这个操作对直接内存代价昂贵，就算是堆内存，也会增加 GC 压力
* 有了池化，则可以重用池中 ByteBuf 实例，并且采用了与 jemalloc 类似的内存分配算法提升分配效率
* 高并发时，池化功能更节约内存，减少内存溢出的可能



开启池化功能：
```
-Dio.netty.allocator.type={unpooled|pooled}
```

- 4.1 以后，非 Android 平台默认启用池化实现，Android 平台启用非池化实现
- 4.1 之前，池化功能还不成熟，默认是非池化实现





### ByteBuf的组成

ByteBuf 由四部分组成

- 读指针

- 写指针

  >最开始读写指针都在 0 位置

- 容量

- 最大容量：默认为int的最大值。也可以指定

![](Netty%E5%85%A5%E9%97%A8.assets/0010.png)



相较于NIO的ByteBuffer，Netty的ByteBuf：

1. 读写分别用两个指针，不用来回切换读写模式
2. 支持扩容



### 写入相关的方法

省略一些不重要的方法：

| 方法签名                                                     | 含义                   | 备注                                        |
| ------------------------------------------------------------ | ---------------------- | ------------------------------------------- |
| writeBoolean(boolean value)                                  | 写入 boolean 值        | 用一字节 01\|00 代表 true\|false            |
| writeByte(int value)                                         | 写入 byte 值           |                                             |
| writeShort(int value)                                        | 写入 short 值          |                                             |
| writeInt(int value)                                          | 写入 int 值            | Big Endian，即 0x250，写入后 00 00 02 50    |
| writeIntLE(int value)                                        | 写入 int 值            | Little Endian，即 0x250，写入后 50 02 00 00 |
| writeLong(long value)                                        | 写入 long 值           |                                             |
| writeChar(int value)                                         | 写入 char 值           |                                             |
| writeFloat(float value)                                      | 写入 float 值          |                                             |
| writeDouble(double value)                                    | 写入 double 值         |                                             |
| writeBytes(ByteBuf src)                                      | 写入 netty 的 ByteBuf  |                                             |
| writeBytes(byte[] src)                                       | 写入 byte[]            |                                             |
| writeBytes(ByteBuffer src)                                   | 写入 nio 的 ByteBuffer |                                             |
| int writeCharSequence(CharSequence sequence, Charset charset) | ==写入字符串==         |                                             |

>注意：
>
>* 这些方法的未指明返回值的，其返回值都是 ByteBuf，意味着可以链式调用
>* 网络传输，默认习惯是 Big Endian （大端写入）
>* 还有一类方法是 set 开头的一系列方法，也可以写入数据，但不会改变写指针位置



例如

- 写入 4 个字节：

  ```java
  buffer.writeBytes(new byte[]{1, 2, 3, 4});
  log(buffer);
  ```

  结果为：

  ```
  read index:0 write index:4 capacity:10
           +-------------------------------------------------+
           |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
  +--------+-------------------------------------------------+----------------+
  |00000000| 01 02 03 04                                     |....            |
  +--------+-------------------------------------------------+----------------+
  ```

- 再写入一个 int 整数，也是 4 个字节：

  ```java
  buffer.writeInt(5);
  log(buffer);
  ```

  结果为：

  ```
  read index:0 write index:8 capacity:10
           +-------------------------------------------------+
           |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
  +--------+-------------------------------------------------+----------------+
  |00000000| 01 02 03 04 00 00 00 05                         |........        |
  +--------+-------------------------------------------------+----------------+
  ```



### 扩容

承接[上面的例子](#写入相关的方法)，再写入一个int整数时：

```java
buffer.writeInt(6);
log(buffer);
```

容量不够了（初始容量是 10），这时会引发扩容，扩容规则：

* 如何写入后数据大小未超过 512，则选择下一个 16 的整数倍，例如写入后大小为 12 ，则扩容后 capacity 是 16
* 如果写入后数据大小超过 512，则选择下一个 2^n，例如写入后大小为 513，则扩容后 capacity 是 2^10^=1024，2^9^=512 已经不够了）
* 扩容不能超过 max capacity 会报错

将打印输出：
```
read index:0 write index:12 capacity:16
         +-------------------------------------------------+
         |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
+--------+-------------------------------------------------+----------------+
|00000000| 01 02 03 04 00 00 00 05 00 00 00 06             |............    |
+--------+-------------------------------------------------+----------------+
```



### 读取

承接上例，例如读了 4 次，每次一个字节：

```java
System.out.println(buffer.readByte());
System.out.println(buffer.readByte());
System.out.println(buffer.readByte());
System.out.println(buffer.readByte());
log(buffer);
```

读过的内容，就属于废弃部分了，再读只能读那些尚未读取的部分(只打印了读指针到写指针之间的数据)

```
1
2
3
4
read index:4 write index:12 capacity:16
         +-------------------------------------------------+
         |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
+--------+-------------------------------------------------+----------------+
|00000000| 00 00 00 05 00 00 00 06                         |........        |
+--------+-------------------------------------------------+----------------+
```



如果需要重复读取 int 整数 5，怎么办？

1. ***可以在 read 前先做个标记 mark***

   ```java
   buffer.markReaderIndex();
   System.out.println(buffer.readInt());
   log(buffer);
   ```

   输出：

   ```
   5
   read index:8 write index:12 capacity:16
            +-------------------------------------------------+
            |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
   +--------+-------------------------------------------------+----------------+
   |00000000| 00 00 00 06                                     |....            |
   +--------+-------------------------------------------------+----------------+
   ```

2. 这时要重复读取的话，重置到标记位置 reset

   ```java
   buffer.resetReaderIndex();
   log(buffer);
   ```

   输出：

   ```
   read index:4 write index:12 capacity:16
            +-------------------------------------------------+
            |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
   +--------+-------------------------------------------------+----------------+
   |00000000| 00 00 00 05 00 00 00 06                         |........        |
   +--------+-------------------------------------------------+----------------+
   ```

>还有种办法是采用 get 开头的一系列方法，这些方法不会改变 read index



### 内存释放（retain & release）

由于 Netty 中有堆外内存的 ByteBuf 实现，堆外内存最好是手动来释放，而不是等 GC 垃圾回收。

* UnpooledHeapByteBuf 使用的是 JVM 内存，只需等 GC 回收内存即可
* UnpooledDirectByteBuf 使用的就是直接内存了，需要特殊的方法来回收内存
* PooledByteBuf 和它的子类使用了池化机制，需要更复杂的规则来回收内存



Netty 这里采用了引用计数法来控制回收内存，每***种 ByteBuf 都实现了 ReferenceCounted 接口***：

* 每个 ByteBuf 对象的初始计数为 1
* 调用 release 方法计数减 1，如果计数为 0，ByteBuf 内存被回收
* 调用 retain 方法计数加 1，表示调用者没用完之前，其它 handler 即使调用了 release 也不会造成回收
* ***当计数为 0 时，底层内存会被回收，这时即使 ByteBuf 对象还在，其各个方法均无法正常使用***



谁来负责 release 呢？

- 错误的想法：

  ```java
  ByteBuf buf = ...
  try {
      ...
  } finally {
      buf.release();
  }
  ```

  请思考，因为 pipeline 的存在，一般需要将 ByteBuf 传递给下一个 ChannelHandler，如果在 finally 中 release 了，就失去了传递性（当然，如果在这个 ChannelHandler 内这个 ByteBuf 已完成了它的使命，那么便无须再传递）

- 正确的使用：

  基本规则是，**谁是最后使用者，谁负责 release**，详细分析如下：

  * 入站 ByteBuf 处理原则
    * 对原始 ByteBuf 不做处理，调用 ctx.fireChannelRead(msg) 向后传递，这时无须 release
    * 将原始 ByteBuf 转换为其它类型的 Java 对象，这时 ByteBuf 就没用了，必须 release
    * 如果不调用 ctx.fireChannelRead(msg) 向后传递，那么也必须 release
    * 注意各种异常，如果 ByteBuf 没有成功传递到下一个 ChannelHandler，必须 release
    * 假设消息一直向后传，那么 TailContext 会负责释放未处理消息（原始的 ByteBuf）
  * 出站 ByteBuf 处理原则
    * 出站消息最终都会转为 ByteBuf 输出，一直向前传，由 HeadContext flush 后 release

  

源码：

TailContext的收尾工作(自动释放msg携带的ByteBuf)：

1. io.netty.channel.DefaultChannelPipeline.TailContext#channelRead

2. ...

3. io.netty.util.ReferenceCountUtil#release(java.lang.Object)

   ```java
   public static boolean release(Object msg) {
       if (msg instanceof ReferenceCounted) {
           return ((ReferenceCounted) msg).release();
       }
       return false;
   }
   ```



### 零拷贝相关

#### slice

slice是Netty对于“零拷贝”（减少内存复制）的体现之一。



假设现在有一种需求，需要对原始ByteBuf分为多个部分进行处理：

- 一种方案是，将ByteBuf的复制为多个小部分

- 另一种方案（即slice）是：将原始 ByteBuf 进行切片成多个 ByteBuf，

  - 切片后的 ByteBuf 并没有发生内存复制，还是使用原始 ByteBuf 的内存；

    > 这意味着，如果原有的ByteBuf调用了release()，切片结果的ByteBuf在使用时将抛出异常。一般采用如下写法：
    >
    > ```java
    > ByteBuf buf = ByteBufAllocator.DEFAULT.buffer(10);
    > buf.release();
    > 
    > ByteBuf buf1 = buf.slice(0, 5);
    > buf1.retain();
    > // do some work
    > buf1.release();
    > ```

  - ***切片后的 ByteBuf 维护独立的 read，write 指针***；

  - ***切换返回的ByteBuf，不允许扩容***（切片后的 max capacity 被固定为这个区间的大小）



例如：

```java
public class TestSlice {

    public static void main(String[] args) {
        // 1.初始化ByteBuf
        ByteBuf buf = ByteBufAllocator.DEFAULT.buffer(10);
        buf.writeBytes(new byte[]{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});

        // 2.slice()
        ByteBuf buf1 = buf.slice(0, 5);
        ByteBuf buf2 = buf.slice(5, 5);

        // 证明没有内存复制
        buf1.setByte(0, 'z');
        log(buf);
    }

    private static void log(ByteBuf buffer) {
        int length = buffer.readableBytes();
        int rows = length / 16 + (length % 15 == 0 ? 0 : 1) + 4;
        StringBuilder buf = new StringBuilder(rows * 80 * 2)
                .append("read index:").append(buffer.readerIndex())
                .append(" write index:").append(buffer.writerIndex())
                .append(" capacity:").append(buffer.capacity())
                .append(NEWLINE);
        appendPrettyHexDump(buf, buffer);
        System.out.println(buf.toString());
    }
}
```

>无参 slice 是从原始 ByteBuf 的 read index 到 write index 之间的内容进行切片，切片后的 max capacity 被固定为这个区间的大小，因此不能追加 write





#### duplicate

duplicate 也是“零拷贝”（减少复制）的体现之一，就好比截取了原始 ByteBuf 所有内容，并且没有 max capacity 的限制，也是与原始 ByteBuf 使用同一块底层内存，**<u>*只是读写指针是独立的*</u>**



#### copy

会将底层内存数据进行深拷贝，因此无论读写，都与原始 ByteBuf 无关



#### CompositeByteBuf

【零拷贝】的体现之一，可以将多个 ByteBuf 合并为一个逻辑上的 ByteBuf，避免拷贝

```java
public class TestCompositeByteBuf {

    public static void main(String[] args) {
        ByteBuf buf1 = ByteBufAllocator.DEFAULT.buffer();
        buf1.writeBytes(new byte[]{1, 2, 3, 4, 5});

        ByteBuf buf2 = ByteBufAllocator.DEFAULT.buffer();
        buf2.writeBytes(new byte[]{6, 7, 8, 9, 10});

//        // 发生数据复制的方案：
//        ByteBuf buf = ByteBufAllocator.DEFAULT.buffer();
//        buf.writeBytes(buf1).writeBytes(buf2);

        // 使用Composite的方案：
        CompositeByteBuf byteBufs = ByteBufAllocator.DEFAULT.compositeBuffer();
        // true 表示增加新的 ByteBuf 自动递增 write index, 否则 write index 会始终为 0
        byteBufs.addComponents(true, buf1, buf2);
        log(byteBufs);
    }
}
```

CompositeByteBuf 是一个组合的 ByteBuf，它内部维护了一个 Component 数组，每个 Component 管理一个 ByteBuf，记录了这个 ByteBuf 相对于整体偏移量等信息，代表着整体中某一段的数据。

* 优点，***对外是一个虚拟视图***，组合这些 ByteBuf 不会产生内存复制
* 缺点，复杂了很多，多次操作会带来性能的损耗

>- addComponents方法的true参数：表示增加新的 ByteBuf 自动递增 write index, 否则 write index 会始终为 0
>- 注意考虑release问题



#### Unpooled工具类

Unpooled 是一个工具类，类如其名，提供了***非池化的 ByteBuf*** 创建、组合、复制等操作



这里仅介绍其跟【零拷贝】相关的 wrappedBuffer 方法，可以用来包装 ByteBuf：

```java
ByteBuf buf1 = ByteBufAllocator.DEFAULT.buffer(5);
buf1.writeBytes(new byte[]{1, 2, 3, 4, 5});
ByteBuf buf2 = ByteBufAllocator.DEFAULT.buffer(5);
buf2.writeBytes(new byte[]{6, 7, 8, 9, 10});

// 当包装 ByteBuf 个数超过一个时, 底层使用了 CompositeByteBuf
ByteBuf buf3 = Unpooled.wrappedBuffer(buf1, buf2);
System.out.println(ByteBufUtil.prettyHexDump(buf3));
```

输出：
```
         +-------------------------------------------------+
         |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
+--------+-------------------------------------------------+----------------+
|00000000| 01 02 03 04 05 06 07 08 09 0a                   |..........      |
+--------+-------------------------------------------------+----------------+
```



也可以用来包装普通字节数组，底层也不会有拷贝操作：

```java
ByteBuf buf4 = Unpooled.wrappedBuffer(new byte[]{1, 2, 3}, new byte[]{4, 5, 6});
System.out.println(buf4.getClass());
System.out.println(ByteBufUtil.prettyHexDump(buf4));
```

输出：
```
class io.netty.buffer.CompositeByteBuf
         +-------------------------------------------------+
         |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |
+--------+-------------------------------------------------+----------------+
|00000000| 01 02 03 04 05 06                               |......          |
+--------+-------------------------------------------------+----------------+
```



### ByteBuf 优势

* 支持池化 - 可以重用池中 ByteBuf 实例，更节约内存，减少内存溢出的可能
* 读写指针分离，不需要像 ByteBuffer 一样切换读写模式
* 可以自动扩容
* 支持链式调用，使用更流畅
* 很多地方体现零拷贝，例如 slice、duplicate、CompositeByteBuf



# 双向通信