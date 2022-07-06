# 相关资料

- [视频教程](https://www.bilibili.com/video/BV1hK41137gc)



# File类

File类，是对资源文件的**路径的抽象**。



## 创建File对象

```java
File(String pathname) //通过字符串类型的路径来创建对象

File(String parent, String child) //通过父目录的路径(字符串类型)和文件(文件夹)名称来创建对象

File(File parent, String child)//通过父目录的路径(File类型)和文件(文件夹)名称来创建对象
```



## 常用方法

```java
boolean createNewFile()//根据路径创建一个文件，返回值代表创建是否成功
boolean mkdir() //根据路径创建一个文件夹，返回值代表创建是否成功
boolean mkdirs() //根据路径创建一个文件夹,如果父目录不存在会自动创建父目录

boolean exists() //判断文件或者文件夹是否存在
boolean isFile() //判断是否是一个文件
boolean isDirectory() //判断是否是一个文件夹
    
boolean delete() //删除文件，或者删除空文件夹，返回值代表删除是否成功
    
long length() //获取一个文件的大小，对文件夹无意义

String getName()//获取文件或文件夹的名字
File getParentFile() //获取父目录的File对象
String getAbsolutePath()//获取File对象的绝对路径   
    
File[] listFiles() //如果当前File对象是一个文件夹，可以获取文件夹下的所有文件或者文件夹的File对象。
```





# IO流

## IO流的分类

- 根据处理数据类型的不同，分为：
  - 字符流
  - 字节流
- 根据数据流向不同，分为：
  - 输入流，对输入流只能进行读操作；
  - 输出流，对输出流只能进行写操作。

![img](IO.assets/iostream2xx.png)



## InputStream

- 所有字节输入流的父类是  java.io.InputStream ，它是以字节为单位的输入流 。
- **以FileInputStream为例**进行学习



### FileInputStream对象创建

```java
FileInputStream(String name) throws FileNotFoundException  //传入文件路径创建对象

FileInputStream(File file) throws FileNotFoundException    //传入文件路径的File对象来创建流对象    
```



### 读数据

- 一次读取一个字节：

  ```java
  public int read() throws IOException // 读取一个字节的数据作为返回值返回  返回值为-1时代表以及没有内容了
  ```

- 一次读取一个字节数组：

  ```java
  public int read(byte b[]) throws IOException //传入一个字节数组，最多读取一个字节数组的数据，并且会把数据存入数组中，返回值代表本次读取到的字节的个数。   如果返回值为-1代表没有数据了
  ```

  

### 资源释放

- 手动调用close方法
- JDK7中，使用try with resources语法
- JDK9中，资源的定义也可以不放在try的小括号中，只要在try的小括号中声明要释放的资源即可



## OutputStream

- 所有字节输出流的父类是  java.io.OutputStream ，它是以字节为单位的输出流 。


- 以FileOutputStream为例



### FileOutputStream对象创建

```java
FileOutputStream(String name) throws FileNotFoundException //传入文件路径创建对象

FileOutputStream(File file) throws FileNotFoundException    //传入文件路径的File对象来创建流对象
```



### 写数据

- 一次写一个字节：

  ```java
  public void write(int b) throws IOException //传入一个字节数据，把字节数据写入文件
  ```

- 一次写一个字节数组：

  ```java
  public void write(byte b[]) throws IOException     //  存入一个字节数组，把字节数组中的数据全部写入文件
  public void write(byte b[], int off, int len) throws IOException  //存入一个字节数组，把字节数组中从off索引开始len个元素写入文件
  ```

  

### 文件续写

如果需要续写就需要使用另外的构造方法：

```java
FileOutputStream(String name, boolean append) throws FileNotFoundException //第二个参数代表是否续写
    
FileOutputStream(File file, boolean append) throws FileNotFoundException  //第二个参数代表是否续写
```



## 案例——文件复制

```java
public class CopyFileTest {

    public static void main(String[] args) {
        File src = new File("E:\\pdfs.zip");
        File dest = new File("E:\\copied.zip");

        copyFile(src, dest);
    }

    public static void copyFile(File src, File dest) {
        try (FileInputStream fileInputStream = new FileInputStream(src);
             FileOutputStream fileOutputStream = new FileOutputStream(dest)) {

            byte[] buffer = new byte[1024 * 1024];
            int len;
            while ((len = fileInputStream.read(buffer)) != -1) {
                fileOutputStream.write(buffer, 0, len);
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
```



## Reader

- 所有字符输入流的父类是  java.io.Reader ，它是**以字符为单位的输入流** 


- 以FileReader为例




### 创建FileReader对象

```java
public FileReader(String fileName) throws FileNotFoundException //传入文件路径创建对象

public FileReader(File file) throws FileNotFoundException //传入文件路径的File对象来创建流对象 
```



### 读数据

- 一次读取一个字符：

  ```java
  public int read() throws IOException //一次读取一个字符返回，如果读到文件末尾，返回值为-1
  ```

  例如：

  ```java
  public class IOTest {
  
      public static void main(String[] args) {
          File file = new File("abc.txt");
  
          try (FileReader fileReader = new FileReader(file)) {
              int ch;
              while ((ch = fileReader.read()) != -1) {
                  System.out.println((char) ch);
              }
  
          } catch (IOException e) {
              throw new RuntimeException(e);
          }
      }
  }
  ```

- 一次读取一个字符数组：

  ```java
  public int read(char cbuf[]) throws IOException //一次读取一个字符数组 返回值为-1时代表读到了末尾
  ```

  

## Writer

- 所有字符输出流的父类是  java.io.Writer ，它是以字符为单位的输出流 
- 以FileWriter为例



### 创建FileWriter对象

```java
public FileWriter(String fileName) throws IOException //传入文件路径创建对象
public FileWriter(File file) throws IOException      //传入文件路径的File对象来创建流对象
```



### 写数据

- 一次写入一个字符：

  ```java
  public void write(int c) throws IOException //把一个字符写入目的地
  ```
  
- 一次写出一个字符数组：
  ```java
  public void write(char cbuf[]) throws IOException //把一个字符数组写入目的地
  ```
  
- 一次写出一个字符串：
  ```java
  public void write(String str) throws IOException //把一个字符串写入目的地
  ```

- 冲刷流：
  - flush()
  - close()



## 高效缓冲流

对硬盘进行数据的读取相比于从内存中存取数据要慢的多。所以JDK为我们提供了高效缓冲流来提高我们IO流的效率。内部原理就是借助内存的缓冲区来减少硬盘IO的次数，提高性能。



### 分类

- 字节流：
  - 输入流：BufferedInputStream
  - 输出流：BufferedOutputStream
- 字符流：
  - 输入流：BufferedReader
  - 输出流：BufferedWriter



### 对象创建

```java
public BufferedInputStream(InputStream in) 
public BufferedOutputStream(OutputStream out)
public BufferedReader(Reader in) 
public BufferedWriter(Writer out)
```



### 缓冲字符流的特有方法

| 类             | 方法                                        | 作用                                                         |
| -------------- | ------------------------------------------- | ------------------------------------------------------------ |
| BufferedReader | public String readLine() throws IOException | 一次读取一行数据，读到的内容不包含换行符,读到了文件末尾返回null。 |
| BufferedWriter | public void newLine() throws IOException    | 写入一个换行符，会根据系统变化                               |



## 转换流

如果我们需要把 **字节流 -> 字符流**，可以使用转换流来实现转换。



直接使用转换流的构造方法进行转换即可:

```java
public InputStreamReader(InputStream in)
public InputStreamReader(InputStream in, String charsetName)
public OutputStreamWriter(OutputStream out)
public OutputStreamWriter(OutputStream out, String charsetName)
```

