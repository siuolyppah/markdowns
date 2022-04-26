# NIO

> non-blocking io：即非阻塞 IO



# 三大组件

## Channel & Buffer

Channel类似于Stream（流），但***Channel是读写数据的双向通道***。

- 可以从Channel将数据读入Buffer
- 也可以从Buffer读出数据到Channel

> - InputStream 和 OutputStream 是***单向***的
> - Channel 比 Stream 更为底层

```mermaid
graph LR
channel --> buffer
buffer --> channel
```



常见的Channel，有：

- FileChannel
- DatagramChannel：UDP网络编程时使用的通道
- SocketChannel：TCP，客户端和服务器端都可
- ServerSocketChannel：TCP，专用于服务器端



Buffer 则***用来缓冲Channel的读写数据***，常见的 buffer 有：

- **ByteBuffer**（抽象类）
  * MappedByteBuffer
  * DirectByteBuffer
  * HeapByteBuffer
- ShortBuffer
- IntBuffer
- LongBuffer
- FloatBuffer
- DoubleBuffer
- CharBuffer



## Selector

selector 单从字面意思不好理解，需要结合服务器的设计演化来理解它的用途

1. 多线程的设计：

   ```mermaid
   graph TD
   subgraph 多线程版
   t1(thread) --> s1(socket1)
   t2(thread) --> s2(socket2)
   t3(thread) --> s3(socket3)
   end
   ```

   - 说明：每个客户端和服务器端分别建立一条socket连接，服务器端同时新建对应的线程从对应的socket中进行读写
   - 缺点：
     - 内存占用高
     - 线程上下文切换成本高
     - 只适合连接数少的场景

2. 线程池版的设计：

   ```mermaid
   graph TD
   subgraph 线程池版
   t4(thread) --> s4(socket1)
   t5(thread) --> s5(socket2)
   t4(thread) -.-> s6(socket3)
   t5(thread) -.-> s7(socket4)
   end
   ```

   - 说明：对线程进行复用。

   - 缺点：

     - socker API阻塞模式下，一个线程仅能同时处理一个 socket 连接

       也就是说，只有当socket1断开连接后，socket3才能被线程服务。

       ***导致线程的利用率不高***（一个socket可以占用一个线程，但却不做事）

     - 仅适合<u>短连接</u>的场景

3. selector 版的设计：

   ```mermaid
   graph TD
   subgraph selector 版
   thread --> selector
   selector --> c1(channel)
   selector --> c2(channel)
   selector --> c3(channel)
   end
   ```

   ***selector 的作用就是配合一个线程来管理多个 channel***，获取这些 channel 上发生的事件(分为可连接、可读、可写），这些 channel 工作在<u>***非阻塞模式***</u>下，不会让线程吊死在一个 channel 上。<u>适合连接数特别多，但流量低的场景</u>（low traffic）


   ***调用 selector 的 select() 会阻塞直到 channel 发生了读写就绪事件，这些事件发生，select 方法就会返回这些事件交给 thread 来处理***



# 项目依赖

```xml
<dependencies>
    <dependency>
        <groupId>io.netty</groupId>
        <artifactId>netty-all</artifactId>
        <version>4.1.39.Final</version>
    </dependency>

    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <version>1.18.10</version>
    </dependency>
    <dependency>
        <groupId>com.google.code.gson</groupId>
        <artifactId>gson</artifactId>
        <version>2.8.5</version>
    </dependency>

    <dependency>
        <groupId>com.google.guava</groupId>
        <artifactId>guava</artifactId>
        <version>19.0</version>
    </dependency>
    <dependency>
        <groupId>ch.qos.logback</groupId>
        <artifactId>logback-classic</artifactId>
        <version>1.2.3</version>
    </dependency>
</dependencies>
```



# logback配置文件

> 位于resource目录下

```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration
        xmlns="http://ch.qos.logback/xml/ns/logback"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://ch.qos.logback/xml/ns/logback logback.xsd">
    <!-- 输出控制，格式控制-->
    <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
        <encoder>
            <pattern>%date{HH:mm:ss} [%-5level] [%thread] %logger{17} - %m%n </pattern>
        </encoder>
    </appender>
    <appender name="FILE" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <!-- 日志文件名称 -->
        <file>logFile.log</file>
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!-- 每天产生一个新的日志文件 -->
            <fileNamePattern>logFile.%d{yyyy-MM-dd}.log</fileNamePattern>
            <!-- 保留 15 天的日志 -->
            <maxHistory>15</maxHistory>
        </rollingPolicy>
        <encoder>
            <pattern>%date{HH:mm:ss} [%-5level] [%thread] %logger{17} - %m%n </pattern>
        </encoder>
    </appender>

    <!-- 用来控制查看那个类的日志内容（对mybatis name 代表命名空间） -->
    <logger name="org.example" level="DEBUG" additivity="false">
        <appender-ref ref="STDOUT"/>
    </logger>

    <logger name="io.netty.handler.logging.LoggingHandler" level="DEBUG" additivity="false">
        <appender-ref ref="STDOUT"/>
    </logger>

    <root level="ERROR">
        <appender-ref ref="STDOUT"/>
    </root>
</configuration>
```





# ByteBuffer

## ByteBuffer与Channel的使用入门

```java
@Slf4j
public class TestByteBuffer {
    public static void main(String[] args) {

        try (FileChannel channel = new FileInputStream("data.txt").getChannel()) {
            //准备缓冲区，即ByteBuffer
            ByteBuffer buffer = ByteBuffer.allocate(10);

            while (true) {
                //从 channel 中读取数据；写入到 buffer 中
                int len = channel.read(buffer); //返回值-1，表示已读取到文件末尾
                log.debug("本次读取到的字节数：{}", len);
                if (len == -1) {
                    break;
                }

                buffer.flip();      //将buffer切换到 读模式
                while (buffer.hasRemaining()) {
                    byte b = buffer.get();
                    log.debug("\t本次获取的字节为：{}", (char) b);
                }

                buffer.clear();     //将buffer切换到 写模式
            }
        } catch (IOException e) {}
    }
}
```



## ByteBuffer的正确使用套路

1. 向 buffer 写入数据，例如调用 channel.read(buffer)
2. 调用 flip() 切换至**读模式**
3. 从 buffer 读取数据，例如调用 buffer.get()
4. 调用 clear() 或 compact() 切换至**写模式**
5. 重复 1~4 步骤



## ByteBuffer的内部结构

ByteBuffer 有以下重要属性：

* capacity
* position：***读写指针***
* limit



1. 一开始（刚创建时）

   ![](NIO%E7%BC%96%E7%A8%8B.assets/0021.png)

2. 写模式下，position 是写入位置，limit 等于容量，下图表示写入了 4 个字节后的状态

   > ByteBuffer创建后，默认处于写模式

   ![](NIO%E7%BC%96%E7%A8%8B.assets/0018.png)

3. ***flip 动作发生后，position 切换为读取位置，limit 切换为读取限制***

   ![](NIO%E7%BC%96%E7%A8%8B.assets/0019.png)

4. 读取 4 个字节后，状态

   ![](NIO%E7%BC%96%E7%A8%8B.assets/0020.png)

5. clear 动作发生后，状态

   ![](NIO%E7%BC%96%E7%A8%8B.assets/0021.png)

6. compact 方法，是把未读完的部分向前压缩，然后切换至写模式

   ![](NIO%E7%BC%96%E7%A8%8B.assets/0022.png)

   

> clear和compat都会切换到写模式，但：
>
> - clear：将从头开始写，未读取的数据将丢失
> - compat：保留未读的数据，在其位置后继续写入



### flip、clear、compat总结

flip和clear、compat都可以将读写状态翻转，但：

- flip：翻转为读状态

- clear、compat翻转为写状态

  但：clear会清空剩余数据；compat会保留剩余数据

  > 默认状态为写状态

- 





### ByteBuffer调试工具类

```java
import io.netty.util.internal.StringUtil;

import java.nio.ByteBuffer;

import static io.netty.util.internal.MathUtil.isOutOfBounds;
import static io.netty.util.internal.StringUtil.NEWLINE;

public class ByteBufferUtil {
    private static final char[] BYTE2CHAR = new char[256];
    private static final char[] HEXDUMP_TABLE = new char[256 * 4];
    private static final String[] HEXPADDING = new String[16];
    private static final String[] HEXDUMP_ROWPREFIXES = new String[65536 >>> 4];
    private static final String[] BYTE2HEX = new String[256];
    private static final String[] BYTEPADDING = new String[16];

    static {
        final char[] DIGITS = "0123456789abcdef".toCharArray();
        for (int i = 0; i < 256; i++) {
            HEXDUMP_TABLE[i << 1] = DIGITS[i >>> 4 & 0x0F];
            HEXDUMP_TABLE[(i << 1) + 1] = DIGITS[i & 0x0F];
        }

        int i;

        // Generate the lookup table for hex dump paddings
        for (i = 0; i < HEXPADDING.length; i++) {
            int padding = HEXPADDING.length - i;
            StringBuilder buf = new StringBuilder(padding * 3);
            for (int j = 0; j < padding; j++) {
                buf.append("   ");
            }
            HEXPADDING[i] = buf.toString();
        }

        // Generate the lookup table for the start-offset header in each row (up to 64KiB).
        for (i = 0; i < HEXDUMP_ROWPREFIXES.length; i++) {
            StringBuilder buf = new StringBuilder(12);
            buf.append(NEWLINE);
            buf.append(Long.toHexString(i << 4 & 0xFFFFFFFFL | 0x100000000L));
            buf.setCharAt(buf.length() - 9, '|');
            buf.append('|');
            HEXDUMP_ROWPREFIXES[i] = buf.toString();
        }

        // Generate the lookup table for byte-to-hex-dump conversion
        for (i = 0; i < BYTE2HEX.length; i++) {
            BYTE2HEX[i] = ' ' + StringUtil.byteToHexStringPadded(i);
        }

        // Generate the lookup table for byte dump paddings
        for (i = 0; i < BYTEPADDING.length; i++) {
            int padding = BYTEPADDING.length - i;
            StringBuilder buf = new StringBuilder(padding);
            for (int j = 0; j < padding; j++) {
                buf.append(' ');
            }
            BYTEPADDING[i] = buf.toString();
        }

        // Generate the lookup table for byte-to-char conversion
        for (i = 0; i < BYTE2CHAR.length; i++) {
            if (i <= 0x1f || i >= 0x7f) {
                BYTE2CHAR[i] = '.';
            } else {
                BYTE2CHAR[i] = (char) i;
            }
        }
    }

    /**
     * 打印所有内容
     * @param buffer
     */
    public static void debugAll(ByteBuffer buffer) {
        int oldlimit = buffer.limit();
        buffer.limit(buffer.capacity());
        StringBuilder origin = new StringBuilder(256);
        appendPrettyHexDump(origin, buffer, 0, buffer.capacity());
        System.out.println("+--------+-------------------- all ------------------------+----------------+");
        System.out.printf("position: [%d], limit: [%d]\n", buffer.position(), oldlimit);
        System.out.println(origin);
        buffer.limit(oldlimit);
    }

    /**
     * 打印可读取内容
     * @param buffer
     */
    public static void debugRead(ByteBuffer buffer) {
        StringBuilder builder = new StringBuilder(256);
        appendPrettyHexDump(builder, buffer, buffer.position(), buffer.limit() - buffer.position());
        System.out.println("+--------+-------------------- read -----------------------+----------------+");
        System.out.printf("position: [%d], limit: [%d]\n", buffer.position(), buffer.limit());
        System.out.println(builder);
    }

    private static void appendPrettyHexDump(StringBuilder dump, ByteBuffer buf, int offset, int length) {
        if (isOutOfBounds(offset, length, buf.capacity())) {
            throw new IndexOutOfBoundsException(
                    "expected: " + "0 <= offset(" + offset + ") <= offset + length(" + length
                            + ") <= " + "buf.capacity(" + buf.capacity() + ')');
        }
        if (length == 0) {
            return;
        }
        dump.append(
                "         +-------------------------------------------------+" +
                        NEWLINE + "         |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |" +
                        NEWLINE + "+--------+-------------------------------------------------+----------------+");

        final int startIndex = offset;
        final int fullRows = length >>> 4;
        final int remainder = length & 0xF;

        // Dump the rows which have 16 bytes.
        for (int row = 0; row < fullRows; row++) {
            int rowStartIndex = (row << 4) + startIndex;

            // Per-row prefix.
            appendHexDumpRowPrefix(dump, row, rowStartIndex);

            // Hex dump
            int rowEndIndex = rowStartIndex + 16;
            for (int j = rowStartIndex; j < rowEndIndex; j++) {
                dump.append(BYTE2HEX[getUnsignedByte(buf, j)]);
            }
            dump.append(" |");

            // ASCII dump
            for (int j = rowStartIndex; j < rowEndIndex; j++) {
                dump.append(BYTE2CHAR[getUnsignedByte(buf, j)]);
            }
            dump.append('|');
        }

        // Dump the last row which has less than 16 bytes.
        if (remainder != 0) {
            int rowStartIndex = (fullRows << 4) + startIndex;
            appendHexDumpRowPrefix(dump, fullRows, rowStartIndex);

            // Hex dump
            int rowEndIndex = rowStartIndex + remainder;
            for (int j = rowStartIndex; j < rowEndIndex; j++) {
                dump.append(BYTE2HEX[getUnsignedByte(buf, j)]);
            }
            dump.append(HEXPADDING[remainder]);
            dump.append(" |");

            // Ascii dump
            for (int j = rowStartIndex; j < rowEndIndex; j++) {
                dump.append(BYTE2CHAR[getUnsignedByte(buf, j)]);
            }
            dump.append(BYTEPADDING[remainder]);
            dump.append('|');
        }

        dump.append(NEWLINE +
                "+--------+-------------------------------------------------+----------------+");
    }

    private static void appendHexDumpRowPrefix(StringBuilder dump, int row, int rowStartIndex) {
        if (row < HEXDUMP_ROWPREFIXES.length) {
            dump.append(HEXDUMP_ROWPREFIXES[row]);
        } else {
            dump.append(NEWLINE);
            dump.append(Long.toHexString(rowStartIndex & 0xFFFFFFFFL | 0x100000000L));
            dump.setCharAt(dump.length() - 9, '|');
            dump.append('|');
        }
    }

    public static short getUnsignedByte(ByteBuffer buffer, int index) {
        return (short) (buffer.get(index) & 0xFF);
    }
}
```



### ByteBuffer的内部结构测试

测试put()，read()，flip()，compact()：

```java
public class TestByteBufferReadWrite {
    public static void testPut() {
        ByteBuffer buffer = ByteBuffer.allocate(10);

        buffer.put((byte) 0x61);    //'a'
        debugAll(buffer);

        buffer.put(new byte[]{0x62, 0x63, 0x64});
        debugAll(buffer);
    }

    //flip切换为读状态
    public static void testGetAndFlip() {
        ByteBuffer buffer = ByteBuffer.allocate(10);
        buffer.put(new byte[]{0x61, 0x62, 0x63, 0x64});

//        byte b = buffer.get();  //从pos位置读取，也就是第5个
//        System.out.println(b);  //输出0

        buffer.flip();
        System.out.println(buffer.get());
        debugAll(buffer);
    }

    //Compact切换为写状态
    public static void testCompact(){
        ByteBuffer buffer = ByteBuffer.allocate(10);
        buffer.put(new byte[]{0x61, 0x62, 0x63, 0x64});

        debugAll(buffer);

        buffer.flip();
        buffer.get();
        buffer.get();

        buffer.compact();
        debugAll(buffer);

    }
    
    public static void main(String[] args) {
//        testWrite();

//        testGetAndFlip();

        testCompact();
    }
}
```



## ByteBuffer的常见方法

### 分配空间

可以使用 allocate 方法为 ByteBuffer 分配空间，其它 buffer 类也有该方法

```java
public class ByteBufferAllocateTest {

    public static void main(String[] args) {
        //class java.nio.HeapByteBuffer
        System.out.println(ByteBuffer.allocate(16).getClass()); 
        
        //class java.nio.DirectByteBuffer
        System.out.println(ByteBuffer.allocateDirect(16).getClass());
    }
}
```

- HeapByteBuffer：使用JVM的堆内存

  - 读写效率低
  - 能受到 GC 影响
  - 分配速度快

- DirectByteBuffer：使用系统的直接内存

  - 读写效率高（少一次拷贝）

  - 不会受到 GC 影响。 

    > 因此，DirectByteBuffer需要释放，否则将造成内存泄漏

  - 分配速度慢



### 向 buffer 写入数据

- 调用 channel 的 read 方法

  ```java
  int readBytes = channel.read(buf);
  ```

- 调用 buffer 自己的 put 方法

  ```
  buf.put((byte)127);
  ```



### 从 buffer 读取数据

同样有两种办法

* 调用 channel 的 write 方法

  ```java
  int writeBytes = channel.write(buf);
  ```

* 调用 buffer 自己的 get 方法

  ```java
  byte b = buf.get();
  ```

  

***get 方法会让 position 读指针向后走***，如果想重复读取数据

- 可以调用 rewind 方法将 position 重新置为 0
- 或者调用 ***get(int i) 方法获取索引 i 的内容，它不会移动读指针***

```java
public class ByteBufferReadTest {

    public static void testRewind(){
        ByteBuffer buffer = ByteBuffer.allocate(10);
        buffer.put(new byte[]{'a','b','c','d'});
        buffer.flip();  //切换到读模式

        buffer.get(new byte[4]);
        debugAll(buffer);

        buffer.rewind();
        debugAll(buffer);
    }

    public static void testGetI(){
        ByteBuffer buffer = ByteBuffer.allocate(10);
        buffer.put(new byte[]{'a','b','c','d'});
        buffer.flip();  //切换到读模式

        debugAll(buffer);
        buffer.get(0);
        debugAll(buffer);
    }
}

```



### mark 和 reset

- mark会记录下当前的position指针的位置
- reset会重置position指针的位置

>**注意**：rewind 和 flip 都会清除 mark 位置



### 字符串与 ByteBuffer 互转

```java
public class ByteBufferAndStringTest {

    public static void stringToByteBuffer1() {
        //字符串直接转Byte数组
        ByteBuffer buffer = ByteBuffer.allocate(16);
        //此时buffer仍处于 写模式
        buffer.put("hello".getBytes());     //使用操作系统的默认编码

        debugAll(buffer);           
    }

    public static void stringToByteBuffer2() {
        //使用CharSet类
        Charset utf8 = StandardCharsets.UTF_8;
        //此时buffer处于 读模式
        ByteBuffer buffer = utf8.encode("hello");   

        debugAll(buffer);
    }

    public static void stringToByteBuffer3() {
        //使用wrap方法
        //此时buffer处于 读模式
        ByteBuffer buffer = ByteBuffer.wrap("hello".getBytes());    

        debugAll(buffer);
    }


    public static void ByteBufferToString() {
        ByteBuffer buffer = ByteBuffer.wrap("hello".getBytes());

        //decode之前，ByteBuffer要处于读模式
        CharBuffer charBuffer = StandardCharsets.UTF_8.decode(buffer);
        String str = charBuffer.toString();
        System.out.println(str);
    }

    public static void main(String[] args) {
//        stringToByteBuffer1();
//        stringToByteBuffer2();
//        stringToByteBuffer3();

        ByteBufferToString();
    }
}
```





### Buffer的线程安全性

> Buffer 是**非线程安全的**



## Scattering Reads

> 即***将读入的字节，分散到多个ByteBuffer中***



假设现在存在一文本文件 3parts.txt，其内容如下：

```
onetwothree
```

 由于三个ByteBuffer的长度已知，可以做到在读取时就将其分散：

```java
public class ScatteringReadsTest {

    public static void main(String[] args) {
        try (FileChannel channel = new RandomAccessFile("3parts.txt", "r").getChannel()) {

            ByteBuffer one = ByteBuffer.allocate(3);
            ByteBuffer two = ByteBuffer.allocate(3);
            ByteBuffer three = ByteBuffer.allocate(5);

            channel.read(new ByteBuffer[]{one, two, three});

            one.flip();
            two.flip();
            three.flip();

            debugAll(one);
            debugAll(two);
            debugAll(three);

        } catch (IOException e) {}
    }
}
```



## Gathering Writes

> 集中写出，即：将多个ByteBuffer，写出到一个文件

```java
public class GatheringWritesTest{

    public static void main(String[] args) {
        ByteBuffer b1 = StandardCharsets.UTF_8.encode("hello");
        ByteBuffer b2 = StandardCharsets.UTF_8.encode(",");
        ByteBuffer b3 = StandardCharsets.UTF_8.encode("world");

        try (FileChannel channel = new RandomAccessFile("words.txt", "rw").getChannel()) {
            channel.write(new ByteBuffer[]{b1,b2,b3});
        } catch (IOException e) {}
    }
}
```



## 黏包、半包问题

网络上有多条数据发送给服务端，数据之间使用 \n 进行分隔
但由于某种原因这些数据在接收时，被进行了重新组合。

例如原始数据有3条（即预期发送3条消息）为：

* Hello,world\n
* I'm zhangsan\n
* How are you?\n

变成了下面的两个 byteBuffer：

* Hello,world\nI'm zhangsan\nHo

  >黏包：两条消息被合为一条

* w are you?\n

  >半包：一条消息被截断为两条



- 黏包的原因：出于传输效率考虑
- 半包的原因：服务器的ByteBuffer容量有限



如下代码，可将数据还原为3条：

```java
public class ReorganizeByteBufferTest {

    public static void main(String[] args) {
        ByteBuffer source = ByteBuffer.allocate(32);
        source.put("Hello,world\nI'm zhangsan\nHo".getBytes());

        spilt(source);
        source.put("w are you?".getBytes());
        spilt(source);
    }

    private static void spilt(ByteBuffer source) {
        source.flip();

        for (int i = 0; i < source.limit(); i++) {
            if (source.get(i) == '\n') {
                int length = i + 1 - source.position();

                //将一条完整的消息，存入新的ByteBuffer
                ByteBuffer target = ByteBuffer.allocate(length);
                for (int j = 0; j < length; j++) {
                    target.put(source.get());
                }
                target.flip();
                System.out.println(StandardCharsets.UTF_8.decode(target));
            }
        }

        source.compact();   //保证source回到写模式
    }
}
```



# 文件编程

## FileChannel

### FileChannel的工作模式

> FileChannel 只能工作在阻塞模式下。
>
> 这意味着 FileChannel 不能配合 Selector 一起使用 



### FileChannel的获取

不能直接打开 FileChannel，必须通过 FileInputStream、FileOutputStream 或者 RandomAccessFile 来获取 FileChannel，它们都有 getChannel 方法

* 通过 FileInputStream 获取的 channel 只能读
* 通过 FileOutputStream 获取的 channel 只能写
* 通过 RandomAccessFile 是否能读写根据构造 RandomAccessFile 时的读写模式决定

> 虽然FileChannel代表的双向通道，但其获取的源头，决定了其是否可读可写



### 从FileChannel读数据

- 调用其 read() 方法：

  会从 channel 读取数据填充 ByteBuffer，返回值表示读到了多少字节，***-1 表示到达了文件的末尾***

```java
int readBytes = channel.read(buffer);
```



### 向FileChannel写数据

应按照如下套路：

```java
ByteBuffer buffer = ...;
buffer.put(...); // 存入数据
buffer.flip();   // 切换读模式

while(buffer.hasRemaining()) {
    channel.write(buffer);
}
```

> 因为常用的SocketChannel，不能保证一次性将Buffer的数据全部写入到Channel中



### 关闭FileChannel

channel 必须关闭，不过调用了 FileInputStream、FileOutputStream 或者 RandomAccessFile 的 close 方法会间接地调用 channel 的 close 方法

> 即：关闭Stream或RandomAccessFile，就能关闭Channel



### FileChannel的Position

类似与ByteBuffer的position属性。

- 获取当前位置

  ```java
  long pos = channel.position();
  ```

- 设置当前位置

  ```java
  long newPos = ...;
  channel.position(newPos);
  ```

  如果将当前位置设置为文件的末尾：

  - 这时读取会返回 -1 
  - 这时写入，会追加内容，但要注意如果 position 超过了文件末尾，再写入时在新内容和原末尾之间会有空洞（00）



### 大小

使用 size 方法获取文件的大小



### 强制写入

操作系统出于性能的考虑，会将数据缓存，不是立刻写入磁盘。可以调用 force(true)  方法将文件内容和元数据（文件的权限等信息）立刻写入磁盘

> 即：操作系统会将数据缓存，直到关闭FileChannel后将才数据真正写入磁盘



### 两个 Channel 传输数据

FileChannel的transferTo方法：

```java
public class TestFileChannelTransferTo {

    public static void main(String[] args) {
        try (FileChannel from = new FileInputStream("data.txt").getChannel();
             FileChannel to = new FileOutputStream("to.txt").getChannel();
        ) {
            left -= from.transferTo(0, from.size(), to);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

> 相比于使用输入输出流，tansferTo的效率更高。
>
> 因为其在底层使用了操作系统的零拷贝进行优化



***但要注意，调用一次transferTo()方法，仅会传递2G数据***。多余内容不会传递。

做如下改进：

```java
public static void main(String[] args) {
    try (FileChannel from = new FileInputStream("data.txt").getChannel();
         FileChannel to = new FileOutputStream("to.txt").getChannel();
        ) {
        long size = from.size();

        // left表示未传输的字节数
        for (long left = size; left > 0; ) {
            left -= from.transferTo(size - left, left, to);
        }
    } catch (IOException e) {
        e.printStackTrace();
    }
}
```



## Path类

jdk7 引入了 Path 和 Paths 类：

- Path 用来表示文件路径
- Paths 是工具类，用来获取 Path 实例



- ***从==用户工作目录"user.dir"==定位***：

  ```java
  Path source = Paths.get("1.txt"); // 相对路径 使用 user.dir 环境变量来定位 1.txt
  ```

- 使用绝对路径定位：

  ```java
  Path source = Paths.get("d:\\1.txt"); // 使用反斜杠/需要转义
  
  Path source = Paths.get("d:/1.txt"); // 使用正斜杠则不用
  ```

- 多个路径拼接：

  ```java
  Path projects = Paths.get("d:\\data", "projects"); // 代表了  d:\data\projects
  ```

>注意：
>
>- 使用`/`需要转义
>- 使用`\`则无需转义





Path类的normalize()方法，能将路径精简：

```java
Path path = Paths.get("d:\\data\\projects\\a\\..\\b");
System.out.println(path);				  //d:\data\projects\a\..\b
System.out.println(path.normalize()); 		//d:\data\projects\b
```



## Files工具类

### 基础使用

检查文件（或文件夹）是否存在

```java
Path path = Paths.get("helloword/data.txt");
System.out.println(Files.exists(path));
```



创建一级目录

```java
Path path = Paths.get("helloword/d1");
Files.createDirectory(path);
```

* 如果目录已存在，会抛异常 FileAlreadyExistsException
* 不能一次创建多级目录，否则会抛异常 NoSuchFileException



创建多级目录用

```java
Path path = Paths.get("helloword/d1/d2");
Files.createDirectories(path);
```



拷贝文件

```java
Path source = Paths.get("helloword/data.txt");
Path target = Paths.get("helloword/target.txt");

Files.copy(source, target);
```

* 如果文件已存在，会抛异常 FileAlreadyExistsException

如果希望用 source 覆盖掉 target，需要用 StandardCopyOption 来控制

```java
Files.copy(source, target, StandardCopyOption.REPLACE_EXISTING);
```

> 使用的操纵系统底层的文件拷贝，与FileChannel的transferTo()效率相近





移动文件

```java
Path source = Paths.get("helloword/data.txt");
Path target = Paths.get("helloword/data.txt");

Files.move(source, target, StandardCopyOption.ATOMIC_MOVE);
```

* StandardCopyOption.ATOMIC_MOVE 保证文件移动的原子性



删除文件

```java
Path target = Paths.get("helloword/target.txt");

Files.delete(target);
```

* 如果文件不存在，会抛异常 NoSuchFileException



删除目录

```java
Path target = Paths.get("helloword/d1");

Files.delete(target);
```

* 如果目录还有内容，会抛异常 DirectoryNotEmptyException



### walkFileTree()方法

遍历目录文件：Files类的walkFileTree()

```java
public static void main(String[] args) throws IOException {
    Path path = Paths.get("C:\\Program Files\\Java\\jdk1.8.0_91");
    AtomicInteger dirCount = new AtomicInteger();
    AtomicInteger fileCount = new AtomicInteger();
    
    //该匿名内部类，决定了对遍历到的文件做何种操作
    Files.walkFileTree(path, new SimpleFileVisitor<Path>(){
        @Override
        public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs) 
            throws IOException {
            System.out.println(dir);
            dirCount.incrementAndGet();
            return super.preVisitDirectory(dir, attrs);
        }

        @Override
        public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) 
            throws IOException {
            System.out.println(file);
            fileCount.incrementAndGet();
            return super.visitFile(file, attrs);
        }
    });
    System.out.println(dirCount); // 133
    System.out.println(fileCount); // 1479
}
```

> 使用了 ***访问者设计模式***



统计 jar 的数目

```java
Path path = Paths.get("C:\\Program Files\\Java\\jdk1.8.0_91");
AtomicInteger fileCount = new AtomicInteger();
Files.walkFileTree(path, new SimpleFileVisitor<Path>(){
    @Override
    public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) 
        throws IOException {
        if (file.toFile().getName().endsWith(".jar")) {
            fileCount.incrementAndGet();
        }
        return super.visitFile(file, attrs);
    }
});
System.out.println(fileCount); // 724
```



删除多级目录

```java
Path path = Paths.get("d:\\a");
Files.walkFileTree(path, new SimpleFileVisitor<Path>(){
    @Override
    public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) 
        throws IOException {
        Files.delete(file);
        return super.visitFile(file, attrs);
    }

    @Override
    public FileVisitResult postVisitDirectory(Path dir, IOException exc) 
        throws IOException {
        Files.delete(dir);
        return super.postVisitDirectory(dir, exc);
    }
});
```



> #### ⚠️ 删除很危险
>
> 删除是危险操作，确保要递归删除的文件夹没有重要内容



拷贝多级目录

```java
long start = System.currentTimeMillis();
String source = "D:\\Snipaste-1.16.2-x64";
String target = "D:\\Snipaste-1.16.2-x64aaa";

Files.walk(Paths.get(source)).forEach(path -> {
    try {
        String targetName = path.toString().replace(source, target);
        // 是目录
        if (Files.isDirectory(path)) {
            Files.createDirectory(Paths.get(targetName));
        }
        // 是普通文件
        else if (Files.isRegularFile(path)) {
            Files.copy(path, Paths.get(targetName));
        }
    } catch (IOException e) {
        e.printStackTrace();
    }
});
long end = System.currentTimeMillis();
System.out.println(end - start);
```





# 网络编程

[黑马程序员Netty全套教程，全网最全Netty深入浅出教程，Java网络编程的王者_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1py4y1E7oA?p=22&spm_id_from=pageDriver)