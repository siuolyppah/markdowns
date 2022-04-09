# 相关资料

https://www.bilibili.com/video/BV16J411h7Rd



# lombok和logback依赖

> logback即为log4j的实现



```xml
<dependencies>
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <version>1.18.10</version>
    </dependency>
    <dependency>
        <groupId>ch.qos.logback</groupId>
        <artifactId>logback-classic</artifactId>
        <version>1.2.3</version>
    </dependency>
</dependencies>
```

> jdk版本为1.8:
>
> ```xml
> <properties>
>     <maven.compiler.source>1.8</maven.compiler.source>
>     <maven.compiler.target>1.8</maven.compiler.target>
> </properties>
> ```



logback.xml[^1]配置如下：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration xmlns="http://ch.qos.logback/xml/ns/logback"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://ch.qos.logback/xml/ns/logback logback.xsd">
    <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
        <encoder>
            <pattern>%date{HH:mm:ss} [%t] %logger - %m%n</pattern>
        </encoder>
    </appender>
    <logger name="c" level="debug" additivity="false">
        <appender-ref ref="STDOUT"/>
    </logger>
    <root level="ERROR">
        <appender-ref ref="STDOUT"/>
    </root>
</configuration>
```



[^1]:位于resource文件夹下即可





# 基础概念

## 进程和线程

### **进程**

- 进程可以视为程序的一个***实例***。

- 大部分程序可以同时运行多个实例进程（例如记事本、画图、浏览器等），也有的程序只能启动一个实例进程（例如网易云音乐、360 安全卫士等）



### **线程**

- 一个进程之内，可以分为一到多个线程。

- 一个线程就是一个指令流，将指令流中的一条条指令以一定的顺序交给 CPU 执行

- 在Java 中，线程作为最小调度单位，进程作为资源分配的最小单位

- 在 windows 中进程是不活动的，只是作为线程的容器



>在单核 cpu 下，多线程虽然不能实际提高程序运行效率，但能避免一个线程长时间占用处理机



>不是所有计算任务都能拆分（参考后文的【***阿姆达尔定律***】）



### 对比与联系

- 进程基本上相互独立的，而线程存在于进程内，是进程的一个子集

- 进程拥有共享的资源，如内存空间等，供其内部的线程共享

- 进程间通信较为复杂

  - 同一台计算机的进程通信称为 IPC（Inter-process communication）

  - 不同计算机之间的进程通信，需要通过网络，并遵守共同的协议，例如 HTTP

- 线程通信相对简单，因为它们共享进程内的内存，一个例子是多个线程可以访问同一个共享变量

- 线程更轻量，线程上下文切换成本一般上要比进程上下文切换低





## 并发与并行概念

- 并发`concurrent`：微观串行，宏观并行

- 并行`parallel`



## 异步调用及场景

站在方法调用者的角度来说，若：

- `Sync`：需要等待结果返回，才能继续运行就是同步
- `ASync`：不需要等待结果返回，就能继续运行就是异步



异步调用的例子：

- 比如在项目中，视频文件需要转换格式等操作比较费时，这时开一个新线程处理视频转换，避免阻塞主线程
- tomcat 的异步 servlet 也是类似的目的，让用户线程处理耗时较长的操作，避免阻塞 tomcat 的工作线程
- ui 程序中，开线程进行其他操作，避免阻塞 ui 线程

-  IO 操作不占用 cpu，但一般拷贝文件使用的是【`阻塞 IO`】，这时相当于线程虽然不用 cpu，但需要一直等待 IO 结束，没能充分利用线程。

  所以才有后面的【非阻塞 IO】和【异步 IO】优化



# 查看进程线程状态的命令

## Windows

- 任务管理器可以查看进程和线程数，也可以用来杀死进程
- tasklist 查看进程
- taskkill 杀死进程



## linux

- ps -fe 查看所有进程

- ps -fT -p \<PID> 查看某个进程（PID）的所有线程
- kill 杀死进程
- top 按大写 H 切换是否显示线程
- top -H -p\<PID> 查看某个进程（PID）的所有线程



## JDK

> 由JDK提供的命令

- jps 命令查看所有 Java 进程
- jstack \<PID> 查看某个 Java 进程（PID）的所有线程状态

- jconsole 来查看某个 Java 进程中线程的运行情况（图形界面）



## jconsole

若`连接远程进程`：

- 需要以如下方式运行你的 java 类

  ```java
  java -Djava.rmi.server.hostname=ip地址 -Dcom.sun.management.jmxremote -
  Dcom.sun.management.jmxremote.port=`连接端口` -Dcom.sun.management.jmxremote.ssl=是否安全连接 -
  Dcom.sun.management.jmxremote.authenticate=是否认证 java类
  ```

  是否对应着true/false

- 修改 /etc/hosts 文件将 127.0.0.1 映射至主机名

- 如果要认证访问，还需要做如下步骤

  - 复制 jmxremote.password 文件

  - 修改 jmxremote.password 和 jmxremote.access 文件的权限为 600 即文件所有者可读写

  - 连接时填入 controlRole（用户名），R&D（密码）





# Java线程

## 创建和运行线程

> 事实上，Java程序在启动时，将自动创建一个线程，即`主线程`



### 子类覆盖Thread类run()方法

```java
// 创建线程对象
Thread t = new Thread() {
    public void run() {
        // 要执行的任务
    }
};
// 启动线程
t.start();
```



例如：

```java
import lombok.extern.slf4j.Slf4j;

@Slf4j(topic = "threadTest")
public class ThreadTest1 {

    public static void main(String[] args) {
        // 构造方法的参数是给线程指定名字，推荐
        Thread t1 = new Thread("t1") {
            @Override
            // run 方法内实现了要执行的任务
            public void run() {
                log.debug("hello"); //22:33:32.887 [t1] DEBUG threadTest - hello
            }
        };
        t1.start();

        log.debug("hello"); //22:33:32.887 [main] DEBUG threadTest - hello
    }
}
```



### Runnable接口与Thread类

- Thread 代表线程
- Runnable 可运行的任务（线程要执行的代码）

```java
Runnable runnable = new Runnable() {
    public void run(){
        // 要执行的任务
    }
};

// 创建线程对象
Thread t = new Thread( runnable );
// 启动线程
t.start();
```



例如：

```java
// 创建任务对象
Runnable task2 = new Runnable() {
    @Override
    public void run() {
        log.debug("hello"); //19:19:00 [t2] c.ThreadStarter - hello
    }
};

// 参数1 是任务对象; 参数2 是线程名字，推荐
Thread t2 = new Thread(task2, "t2");
t2.start();
```



Java 8 以后可以使用 lambda 精简Runnable对象的创建：

```java
// 创建任务对象
Runnable task2 = () -> log.debug("hello");

// 参数1 是任务对象; 参数2 是线程名字，推荐
Thread t2 = new Thread(task2, "t2");
t2.start();
```



相较于Thread类，Runnable接口的优点：

- 用 Runnable 更容易与线程池等高级 API 配合

- 用 Runnable 让任务类脱离了 Thread 继承体系，更灵活

  > 组合关系，优于继承关系



### FutureTask接口与Thread类

```java
// 创建任务对象
FutureTask<Integer> task3 = new FutureTask<>(() -> {
    log.debug("hello");	//19:22:27 [t3] c.ThreadStarter - hello
    return 100;
});

// 参数1 是任务对象; 参数2 是线程名字，推荐
new Thread(task3, "t3").start();

// 主线程阻塞，同步等待 task 执行完毕的结果
Integer result = task3.get();
log.debug("结果是:{}", result);//结果是:100
```

> 注意，Callable接口的***get()***方法，在调用时将***阻塞等待***，直至获得返回值



### 小总结

线程的创建有三种：

- 创建Thread类的子类，覆盖其run()方法
- 创建Runnable接口对象，并用其构造Thread对象
- 创建FutureTask接口对象，并用其构造Thread对象



线程的启动，统一用start()方法[^4]

[^4]:调用start()方法只是说明该线程已就绪，真正执行的时机由操作系统的任务调度器决定





## 线程运行——原理

### 栈与栈帧

- 栈（Java 虚拟机栈）：Java Virtual Machine Stacks 

- JVM的组成：由堆、栈、方法区所组成

  其中栈内存就是给线程使用的，每个线程启动后，虚拟机就会为其分配一块栈内存。

- 每个`栈`由多个`栈帧（Frame）`组成，对应着每次`方法`调用时所占用的内存

- 每个线程只能有一个`活动栈帧`，对应着当前正在执行的那个方法





### 线程上下文切换(**Thread Context Switch**)

因为以下一些原因导致 cpu 不再执行当前的线程，转而执行另一个线程的代码

- 线程的 cpu 时间片用完

- 垃圾回收

- 有更高优先级的线程需要运行

- 线程自己调用了 sleep、yield、wait、join、park、synchronized、lock 等方法

>Context Switch 频繁发生将会影响性能



当 Context Switch 发生时，需要由操作系统保存当前线程的状态，并恢复另一个线程的状态[^3]

[^3]:Java 中对应的概念即为程序计数器（Program Counter Register），它的作用是记住下一条 jvm 指令的执行地址，是线程私有的状态。包括程序计数器、虚拟机栈中每个栈帧的信息，如局部变量、操作数栈、返回地址等



## java.lang.Thread类API

java.lang.Thread类：

|      方法名      | 是否static |                           功能说明                           |                            注意点                            |
| :--------------: | :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|     start()      |            |       启动一个新线程，在新的线程运行 run 方法中的代码        | start 方法只是让线程进入就绪，由操作系统的任务调度器决定什么时候真正开始执行（即分配CPU 的时间片）。<br />每个线程对象的start方法只能调用一次，如果调用了多次会出现IllegalThreadStateException |
|      run()       |            |                   新线程启动后会调用的方法                   | 如果在构造 Thread 对象时传递了 Runnable 参数，则线程启动后会调用 Runnable 中的 run 方法，否则默认不执行任何操作。但可以创建 Thread 的子类对象，来覆盖默认行为 |
|      join()      |            |                       等待线程运行结束                       |                                                              |
|   join(long n)   |            |               等待线程运行结束,最多等待 n 毫秒               |                                                              |
|     getId()      |            |                       获取线程长整型ID                       |                            ID唯一                            |
|    getName()     |            |                          获取线程名                          |                                                              |
|    setName()     |            |                          修改线程名                          |                                                              |
|  getPriority()   |            |                        获取线程优先级                        |                                                              |
| setPriority(int) |            |                        修改线程优先级                        | Java中规定的线程优先级为1~10的整数，值越大优先级越高，该线程被处理机调度的概率越高 |
|    getState()    |            |                         获取线程状态                         | Java 中线程状态是用 6 个 enum 表示，分别为：NEW, RUNNABLE, BLOCKED, WAITING, TIMED_WAITING, TERMINATED |
| isInterrupted()  |            |                        判断是否被打断                        |                      不会清除`打断标记`                      |
|    isAlive()     |            |                         线程是否存活                         |                     存活即还没有运行完毕                     |
|   interrupt()    |            |                           打断线程                           | 如果被打断线程正在 sleep，wait，join 会导致被打断的线程抛出 InterruptedException，并清除`打断标记 `；如果打断的正在运行的线程，则会设置`打断标记` ；park 的线程被打断，也会设置 `打断标记` |
|  interrupted()   |   static   |                    判断当前线程是否被打断                    |                       会清除`打断标记`                       |
| currentThread()  |   static   |                        获取当前正在执                        |                                                              |
|  sleep(long n)   |   static   | 让当前执行的线程休眠n毫秒，休眠时让出 cpu 的时间片给其它线程 |                                                              |
|     yield()      |   static   |           `提示`线程调度器让出当前线程对CPU的使用            |                     主要是为了测试和调试                     |





### start()与run()方法

源码：

- start()方法

  ```java
  public synchronized void start() {
      /**
           * This method is not invoked for the main method thread or "system"
           * group threads created/set up by the VM. Any new functionality added
           * to this method in the future may have to also be added to the VM.
           *
           * A zero status value corresponds to state "NEW".
           */
      if (threadStatus != 0)
          throw new IllegalThreadStateException();
  
      /* Notify the group that this thread is about to be started
           * so that it can be added to the group's list of threads
           * and the group's unstarted count can be decremented. */
      group.add(this);
  
      boolean started = false;
      try {
          start0();
          started = true;
      } finally {
          try {
              if (!started) {
                  group.threadStartFailed(this);
              }
          } catch (Throwable ignore) {
              /* do nothing. If start0 threw a Throwable then
                    it will be passed up the call stack */
          }
      }
  }
  
  private native void start0();
  ```

- run()方法

  ```java
  @Override
  public void run() {
      if (target != null) {
          target.run();
      }
  }
  ```

  



测试如下：

```java
@Slf4j(topic = "c.Test4")
public class Test4 {

    public static void main(String[] args) {
        Thread t1 = new Thread("t1") {
            @Override
            public void run() {
                log.debug("run()方法执行");
            }
        };

        t1.run();       //[main] DEBUG c.Test4 - run()方法执行
        t1.start();     //[t1] DEBUG c.Test4 - run()方法执行
    }
}
```



- 直接调用 run 是在主线程中执行了 ***<u>run</u>***，***没有启动新的线程***，***仍是同步调用***

- 使用 start 是启动新的线程，通过新的线程间接执行 run 中的代码，为异步调用

  > 同一Thread对象，***不能多次调用start()方法***，否则将抛出IllegalThreadStateException异常





### sleep()与yield()方法[^5]

[^5]:这两个方法，同为static方法



- sleep()方法：

  1. 调用 sleep 会让当前线程从 *Running* 进入 *Timed Waiting* 状态（`阻塞`）

  2.  其它线程可以使用 interrupt 方法打断正在睡眠的线程，这时 sleep 方法会抛出 InterruptedException

     ```java
     @Slf4j
     public class Test5 {
     
         public static void main(String[] args) throws InterruptedException {
             Thread t1 = new Thread("t1") {
                 @Override
                 public void run() {
                     try {
                         log.debug("enter sleeping");    //23:27:59.162 [t1] DEBUG com.example.juc_learn.Test5 - enter sleeping
                         Thread.sleep(2_000);
                         log.debug("end sleeping");      //并不会执行，因为被打断
                     } catch (InterruptedException e) {
                         e.printStackTrace();
                     }
                 }
             };
     
             t1.start();
             Thread.sleep(500);
             t1.interrupt();
             log.debug("interrupt"); //23:27:59.667 [main] DEBUG com.example.juc_learn.Test5 - interrupt
         }
     }
     ```

  3.  睡眠结束后的线程未必会立刻得到执行

  4.  建议用 TimeUnit 的 sleep 代替 Thread 的 sleep 来获得更好的可读性

     ```java
     @Slf4j
     public class Test6 {
     
         public static void main(String[] args) throws InterruptedException {
             TimeUnit.SECONDS.sleep(1);  //当前线程sleep 1秒
         }
     }
     ```

- yield()方法[^6]：

  1. 调用 yield 会让当前线程从 *Running* 进入 *Runnable* `就绪`状态，然后调度执行其它线程
  2. 具体的实现依赖于操作系统的任务调度器

  [^6]:yield本意为让步



小总结：

- sleep方法会使得当前线程进入阻塞状态，处于阻塞状态的线程必须先被唤醒，进入就绪状态，才能被调度
- yield方法会使得当前线程进入就绪状态，***处于就绪状态的线程仍可被处理机调度***



#### 线程优先级

- 线程优先级会提示（hint）调度器优先调度该线程，***但它仅仅是一个提示，调度器可以忽略它***
- 如果 cpu 比较忙，那么优先级高的线程会获得更多的时间片，但 cpu 闲时，优先级几乎没作用

> Java线程优先级为1~10，值越大，优先级越高



例如：

```java
@Slf4j
public class Test7 {

    public static void main(String[] args) {
        Runnable task1 = () -> {
            int count = 0;
            while (true) {
                System.out.println("task1:" + count++);
            }
        };

        Runnable task2 = () -> {
            int count = 0;
            while (true) {
//                Thread.yield();
                System.out.println("\ttask2:" + count++);
            }
        };

        Thread t1 = new Thread(task1, "t1");
        Thread t2 = new Thread(task2, "t2");

//        t1.setPriority(Thread.MIN_PRIORITY);
//        t2.setPriority(Thread.MAX_PRIORITY);

        t1.start();
        t2.start();
    }
}
```



### join()方法

https://www.bilibili.com/video/BV16J411h7Rd?p=32



## 线程状态

