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



# Java线程

## 创建和运行线程

> 事实上，Java程序在启动时，将自动创建一个线程，即`主线程`



### Thread类

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





## 查看进程线程的方法

### Windows

- 任务管理器可以查看进程和线程数，也可以用来杀死进程
- tasklist 查看进程
- taskkill 杀死进程



### linux

- ps -fe 查看所有进程

- ps -fT -p \<PID> 查看某个进程（PID）的所有线程
- kill 杀死进程
- top 按大写 H 切换是否显示线程
- top -H -p\<PID> 查看某个进程（PID）的所有线程



### JDK

> 由JDK提供的命令

- jps 命令查看所有 Java 进程
- jstack \<PID> 查看某个 Java 进程（PID）的所有线程状态

- jconsole 来查看某个 Java 进程中线程的运行情况（图形界面）



### jconsole

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



## 线程运行——原理

### 栈与栈帧

https://www.bilibili.com/video/BV16J411h7Rd?p=20&spm_id_from=pageDriver

### 线程上下文切换(**Thread Context Switch**)









## 线程API





## 线程状态

