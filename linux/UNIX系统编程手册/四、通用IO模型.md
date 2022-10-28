# 文件描述符File Descriptor

- 所有执行 I/O 操作的系统调用都以文件描述符，一个非负整数（通常是小整数），来==指代打开的文件==。
- 文件描述符用以表示所有类型的已打开文件，==包括**管道（pipe）**、**FIFO**、**socket**、终端、设备和普通文件==。
- ==针对每个进程，文件描述符都自成一套==。



# 标准文件描述符

- 大多数应用，都使用`stdin,stdout,stderr`三个标准文件描述符。

- 在shell中运行的程序，继承了shell文件描述符的副本（指向shell本身）。

  > 如果对输入输出进行了重定向，则shell会先修改文件描述符，再启动程序。

![image-20221023151621848](%E5%9B%9B%E3%80%81%E9%80%9A%E7%94%A8IO%E6%A8%A1%E5%9E%8B.assets/image-20221023151621848.png)

> 其中POSIX名称，定义在`<unistd.h>`中。



# open()打开或新建

fd = open(pathname, flags, mode) 函数打开 pathname 所标识的文件，并返回文件描述符，用以在后续函数调用中指代打开的文件。如果文件不存在，open()函数可以创建之，这取决于对位掩码参数 flags 的设置。flags 参数还可指定文件的打开方式：只读、只写亦或是读写方式。mode 参数则指定了由 open()调用创建文件的访问权限。如果 open()函数并未创建文件，那么可以忽略或省略 mode 参数。



- 文件访问模式flag：

  ![image-20221023163801797](%E5%9B%9B%E3%80%81%E9%80%9A%E7%94%A8IO%E6%A8%A1%E5%9E%8B.assets/image-20221023163801797.png)

  > 由于使用了数字0,1,2来表示，因此`O_RDWR`并不等同于`O_RDONLY | O_WRONLY`

- 文件访问权限，由位掩码参数mode指定：

  ```cpp
  mode_t filePerms = S_IRUSR | S_IWUSR |
      			   S_IRGRP | S_IWGRP |
                     S_IROTH | S_IWOTH; // rw-rw-rw-
  ```

  > mode可省略。

  >新建文件的访问权限不仅仅依赖于参数 mode，而且受到进程的 umask 值和（可能存在的）父目录的默认访问控制列表影响。

  

- open()返回的fd，总是进程未用文件描述符的最小值。

  > 这意味着如果关掉`STDIN_FILENO`，再调用open()，open()将返回0.



## 文件访问模式flags详解

> 4.3.1节

![image-20221023165402002](%E5%9B%9B%E3%80%81%E9%80%9A%E7%94%A8IO%E6%A8%A1%E5%9E%8B.assets/image-20221023165402002.png)

![image-20221023165412175](%E5%9B%9B%E3%80%81%E9%80%9A%E7%94%A8IO%E6%A8%A1%E5%9E%8B.assets/image-20221023165412175.png)



**文件访问标志**可分为如下几组：

- 文件访问模式标志：

  O_RDONLY、O_WRONLY 和 O_RDWR 标志均在此列，调用 open()时，上述三者在 flags 参数中不能同时使用，只能指定其中一种。

- 文件创建标志：

  涉及后续 I/O 操作的各个选项。这些标志不能检索，也无法修改。

- 已打开文件的状态标志：

  使用 fcntl()的 F_GETFL 和F_SETFL 操作可以分别检索和修改此类标志。有时干脆将其称之为==文件状态标志==。



## open()可能的错误

> 4.3.2节

若打开文件时发生错误，open()将返回−1，错误号 errno 标识错误原因。



## create()系统调用

>**在早期的 UNIX 实现中**，open()只有两个参数，无法创建新文件，而是使用 creat()系统调用来创建并打开一个新文件。

creat()系统调用根据 pathname 参数创建并打开一个文件，若文件已存在，则打开文件，并清空文件内容，将其长度清 0。



等价于如下 open()调用：

```cpp
fd = open(pathname, O_WRONLY| O_CREAT | O_TRUNC, mode);
```



# read()

numread = read(fd, buffer, count) 调用从 fd 所指代的打开文件中读取至多 count 字节的数据，并存储到 buffer 中。read()调用的返回值为实际读取到的字节数。如果再无字节可读（例如：读到文件结尾符 EOF 时），则返回值为 0。

![image-20221023171121392](%E5%9B%9B%E3%80%81%E9%80%9A%E7%94%A8IO%E6%A8%A1%E5%9E%8B.assets/image-20221023171121392.png)

- count 参数指定==最多==能读取的字节数。

- 返回值：
  - 如果 read()调用成功，将返回实际读取的字节数，
  - 如果遇到文件结束（EOF）则返回 0，
  - 如果出现错误则返回-1。

>**==*一次 read()调用所读取的字节数可以小于请求的字节数*==**。对于普通文件而言，这有可能是因为当前读取位置靠近文件尾部。默认情况下从终端读取字符，一遇到换行符（\n），read()调用就会结束。

- ==如果输入缓冲区的结尾处需要一个表示终止的空字符，必须显式追加==。





# write()

numwritten = write(fd, buffer, count) 调用从 buffer 中读取多达 count 字节的数据写入由 fd 所指代的已打开文件中。write()调用的返回值为实际写入文件中的字节数，且有可能小于 count。

![image-20221023173637710](%E5%9B%9B%E3%80%81%E9%80%9A%E7%94%A8IO%E6%A8%A1%E5%9E%8B.assets/image-20221023173637710.png)

- 返回结果：

  - 如果 write()调用成功，将返回实际写入文件的字节数，该返回值可能小于 count 参数值。即“==部分写==”。

    >造成“部分写”的原因可能是由于磁盘已满，或是因为进程资源对文件大小的限制。

- ==write()调用成功，并不能保证数据已经写入磁盘==。因为为了减少磁盘活动量和加快 write()系统调用，内核会缓存磁盘的 I/O 操作。





# close()

status = close(fd)在所有输入/输出操作完成后，调用 close()，释放文件描述符 fd 以及与之相关的内核资源。



# lseek()改变文件偏移量

针对文件描述符 fd 参数所指代的已打开文件，lseek()系统调用依照 offset 和 whence 参数值调整该文件的偏移量。lseek()调用成功会返回新的文件偏移量。

![image-20221023181312844](%E5%9B%9B%E3%80%81%E9%80%9A%E7%94%A8IO%E6%A8%A1%E5%9E%8B.assets/image-20221023181312844.png)

- whence参数：

  - SEEK_SET 

    将文件偏移量设置为从文件头部起始点开始的 offset 个字节。

  - SEEK_CUR 

    相对于当前文件偏移量，将文件偏移量调整 offset 个字节 。

  - SEEK_END 

    将文件偏移量设置为起始于文件尾部的 offset 个字节。也就是说，offset 参数应该从文件最后一个字节之后的下一个字节算起。

- 如果 whence 参数值为 SEEK_CUR 或 SEEK_END，offset 参数可以为正数也可以为负数；

  如果 whence 参数值为 SEEK_SET，offset 参数值必须为非负数。

  >简而言之，相对于文件头部的绝对偏移量=当前文件偏移量+offset。

- lseek()并不适用于所有类型的文件。不允许将 lseek()应用于管道、FIFO、socket 或者终端。

  一旦如此，调用将会失败，并将 errno 置为 ESPIPE。



文件空洞：

- 如果程序的文件偏移量已然跨越了文件结尾，然后再执行 I/O 操作，
  - 对于read，将返回0，表示文件结尾。
  - 对于write，写入数据，才分配磁盘块。

>[百度百科 (baidu.com)](https://vhsagj.smartapps.baidu.com/pages/lemma/lemma?lemmaTitle=文件空洞&lemmaId=6169344&from=bottomBarShare&_swebfr=1&_swebFromHost=mibrowser)。
>
>**==*空洞是否占用硬盘空间是由文件系统（file system）决定的*==**。



# demo

## copy

```cpp
#include <iostream>
#include <string>
#include <fcntl.h>
#include "tlpi-lib/tlpi_hdr.h"

/**
 * ./copy oldfile newfile
 */
int main(int argc, char *argv[])
{
    using namespace std;

    if (argc != 3 && string(argv[1]) == "--help")
    {
        cerr << argv[0] << " old-file new-file" << endl;
        exit(EXIT_FAILURE);
    }

    string old_file(argv[1]);
    string new_file(argv[2]);

    /* Open input and output files */
    int inputFd = open(old_file.c_str(), O_RDONLY);
    if (inputFd == -1)
    {
        cerr << "open old-file fail:" << old_file << endl;
        exit(EXIT_FAILURE);
    }

    int openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                       S_IROTH | S_IWOTH; // rw-rw-rw-

    int outputFd = open(new_file.c_str(), openFlags, filePerms);
    if (outputFd == -1)
    {
        cerr << "open new-file fail:" << new_file << endl;
        exit(EXIT_FAILURE);
    }

    /* Transfer data */
    ssize_t numRead;

    constexpr int BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        if (write(outputFd, buf, numRead) != numRead)
        {
            cerr << "couldn't write whole buffer" << endl;
            terminate();
        }
    }
    if (numRead == -1)
    {
        cerr << "read error" << endl;
        exit(EXIT_FAILURE);
    }

    if(close(inputFd) == -1){
        cerr << "close old-file error" << endl;
        exit(EXIT_FAILURE);
    }

    if(close(outputFd) == -1){
        cerr << "close new-file error" << endl;
        exit(EXIT_FAILURE);
    }


    return 0;
}
```



