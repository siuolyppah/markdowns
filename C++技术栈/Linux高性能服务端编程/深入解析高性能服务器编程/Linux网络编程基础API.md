# 本章内容

将从如下3个方面讨论Linux网络API：  

- socket地址API。socket最开始的含义是一个IP地址和端口对（ip，port）。它唯一地表示了使用TCP通信的一端。本书称其为socket地址。
- socket基础API。socket的主要API都定义在sys/socket.h头文件中，包括创建socket、命名socket、监听socket、接受连接、发起连接、读写数据、获取地址信息、检测带外标记，以及读取和设置socket选项。  

- 网络信息API。Linux提供了一套网络信息API，以实现主机名和IP地址之间的转换，以及服务名称和端口号之间的转换。这些API都定义在netdb.h头文件中。





# Socket地址API

## 主机字节序与网络字节序

对于32位机器，CPU的累加器一次能装载至少4字节。那么这4个字节在内存中排列的顺序，将影响它被累加器装载成的整数的值。



- 主机字节序：

  - 大端字节序：

    大端字节序是指一个整数的高位字节（23～31 bit）存储在内存的低地址处，低位字节（0～7 bit）存储在内存的高地址处。  

  - ==小端字节序==：

    整数的高位字节存储在内存的高地址处，而低位字节则存储在内存的低地址处。  

    >现代PC大多采用小端字节序，因此==小端字节序又被称为主机字节序==。  



测试字节序的代码：

```C++
#include <iostream>

void byteorder(){
	using namespace std;
	union{
		short value;
		char union_bytes[sizeof(short)];
	}test;

	test.value = 0x0102;
	if((test.union_bytes[0]==1) && (test.union_bytes[1]==2)){
		cout << "big endian" <<endl;
		return;
	}

	if((test.union_bytes[0]==2) && (test.union_bytes[1]==1)){
		cout << "small endian" <<endl;
		return;
	}
	
	cout << "unknown"<<endl;
}
```



==发送端总是把要发送的数据转化成大端字节序数据后再发送，==而接收端知道对方传送过来的数据总是采用大端字节序，所以接收端可以根据自身采用的字节序决定是否对接收到的数据进行转换（小端机转换，大端机不转换）。因此==大端字节序也称为网络字节序==，它给所有接收数据的主机提供了一个正确解释收到的格式化数据的保证。  



Linux提供了如下4个函数来完成主机字节序和网络字节序之间的转换：

![image-20220913124303044](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913124303044.png)

比如htonl表示“host to network long”，即将长整型（32 bit）的主机字节序数据转化为网络字节序数据。  



## 通用socket地址

socket网络编程接口中==表示socket地址的是结构体sockaddr(旧)==，其定义如下：  

![image-20220913124401454](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913124401454.png)

- ==其中`sa_family_t`是地址族类型，通常与协议族（protocol family, 也称domain）对应==。

  ![image-20220913124527658](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913124527658.png)

  宏PF\_\*和AF\_\*都定义在bits/socket.h头文件中，且后者与前者有完全相同的值，所以二者通常混用。  

- sa_data成员，用于存放socket地址值。不同的协议族的地址值具有不同的含义和长度：

  ![image-20220913124708721](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913124708721.png)

  

14字节的sa_data根本无法完全容纳多数协议族的地址值。因此，Linux定义了下面这个新的通用socket地址结构体：  

![image-20220913124905367](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913124905367.png)

这个结构体不仅提供了足够大的空间用于存放地址值，而且是内存对齐的（这是__ss_align成员的作用）。  



## 专用socket地址

上面这两个通用socket地址结构体显然很不好用，比如设置与获取IP地址和端口号就需要执行烦琐的位操作。所以Linux为各个协议族提供了专门的socket地址结构体。  

- UNIX本地域协议族：

  ![image-20220913125058102](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913125058102.png)

- TCP/IPv4：

  ![image-20220913125109047](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913125109047.png)

- TCP/IPv6：

  ![image-20220913125115194](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913125115194.png)

  

==所有专用socket地址（以及sockaddr_storage）类型的变量在实际使用时都需要转化为通用socket地址类型sockaddr（强制转换即可）== 。



## IP地址转换函数

点分十进制字符串表示的IPv4地址和用网络字节序整数表示的IPv4地址之间的转换：  

![image-20220913125306646](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913125306646.png)

- inet_addr函数将用点分十进制字符串表示的IPv4地址转化为用网络字节序整数表示的IPv4地址。它失败时返回INADDR_NONE。 
-  inet_aton函数完成和inet_addr同样的功能，但是将转化结果存储于参数inp指向的地址结构中。它成功时返回1，失败则返回0。  
- inet_ntoa函数将用网络字节序整数表示的IPv4地址转化为用点分十进制字符串表示的IPv4地址。但需要注意的是，该函数内部用一个静态变量存储转化结果，函数的返回值指向该静态内存，因此inet_ntoa是不可重入的。  



下面这对==更新==的函数也能完成和前面3个函数同样的功能，并且它们同时适用于IPv4地址和IPv6地址：  

![image-20220913125508890](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913125508890.png)

- inet_pton函数将用字符串表示的IP地址src（用点分十进制字符串表示的IPv4地址或用十六进制字符串表示的IPv6地址）转换成用网络字节序整数表示的IP地址，并把转换结果存储于dst指向的内存中。  其中，af参数指定地址族，可以是AF_INET或者AF_INET6。inet_pton成功时返回1，失败则返回0并设置errno[1]。  

- inet_ntop函数进行相反的转换，前三个参数的含义与inet_pton的参数相同，最后一个参数cnt指定目标存储单元的大小。下面的两个宏能帮助我们指定这个大小（分别用于IPv4和IPv6） ：

  ![image-20220913125610837](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913125610837.png)

  inet_ntop成功时返回目标存储单元的地址，失败则返回NULL并设置errno。



# 创建socket

>UNIX/Linux的一个哲学是：所有东西都是文件。  

==socket是可读、可写、可控制、可关闭的文件描述符==。



创建socket的系统调用：
![image-20220913125915693](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913125915693.png)

- domain参数告诉系统使用哪个底层协议族。对TCP/IP协议族而言，该参数应该设置为PF_INET（Protocol Family of Internet，用于IPv4）或PF_INET6（用于IPv6）；对于UNIX本地域协议族而言，该参数应该设置为PF_UNIX。关于socket系统调用支持的所有协议族，请读者自己参考其man手册。  

- type参数指定服务类型。服务类型主要有SOCK_STREAM服务（流服务）和SOCK_UGRAM（数据报）服务。对TCP/IP协议族而言，其值取SOCK_STREAM表示传输层使用TCP协议，取SOCK_DGRAM表示传输层使用UDP协议。 

  >自Linux内核版本2.6.17起，type参数可以接受上述服务类型与下面两个重要的标志相与的值：SOCK_NONBLOCK和SOCK_CLOEXEC。它们分别表示将新创建的socket设为非阻塞的，以及用fork调用创建子进程时在子进程中关闭该socket。在内核版本2.6.17之前的Linux中，文件描述符的这两个属性都需要使用额外的系统调用（比如fcntl）来设置。  

- protocol参数是在前两个参数构成的协议集合下，再选择一个具体的协议。不过这个值通常都是唯一的（前两个参数已经完全决定了它的值）。几乎在所有情况下，我们都应该把它设置为0，表示使用默认协议。  

socket系统调用成功时返回一个socket文件描述符，失败则返回-1并设置errno。  



# 命名socket

创建socket时，我们给它指定了地址族，但是并未指定使用该地址族中的哪个具体socket地址。==将一个socket与socket地址绑定称为给socket命名==。  

>在服务器程序中，我们通常要命名socket，因为只有命名后客户端才能知道该如何连接它。客户端则通常不需要命名socket，而是采用匿名方式，即使用操作系统自动分配的socket地址。  



命名socket的系统调用是bind，其定义如下：  

![image-20220913130302695](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913130302695.png)

- bind将my_addr所指的socket地址分配给未命名的sockfd文件描述符，addrlen参数指出该socket地址的长度。  

bind成功时返回0，失败则返回-1并设置errno。其中两种常见的errno是EACCES和EADDRINUSE，它们的含义分别是：  

- EACCES，被绑定的地址是受保护的地址，仅超级用户能够访问。比如普通用户将socket绑定到知名服务端口（端口号为0～1023）上时，bind将返回EACCES错误。  
- EADDRINUSE，被绑定的地址正在使用中。比如将socket绑定到一个处于TIME_WAIT状态的socket地址。  



# 监听socket

socket被命名之后，还不能马上接受客户连接，我们需要使用如下系统调用来创建一个监听队列以存放待处理的客户连接：  

![image-20220913130434945](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913130434945.png)

- sockfd参数指定被监听的socket。  

- backlog参数提示内核监听队列的最大长度。监听队列的长度如果超过backlog，服务器将不受理新的客户连接，客户端也将收到ECONNREFUSED错误信息。

  >在内核版本2.2之前的Linux中，backlog参数是指所有处于半连接状态（SYN_RCVD）和完全连接状态（ESTABLISHED）的socket的上限。但自内核版本2.2之后，它只表示处于完全连接状态的socket的上限，
  >处于半连接状态的socket的上限则由/proc/sys/net/ipv4/tcp_max_syn_backlog内核参数定义。
  >
  >backlog参数的典型值是5。  

  listen成功时返回0，失败则返回-1并设置errno。  



# 接收连接

下面的系统调用==从listen监听队列中接受一个连接==：  

![image-20220913133401160](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913133401160.png)

- sockfd参数是执行过listen系统调用的监听socket。  
- addr参数用来获取被接受连接的远端socket地址，该socket地址的长度由addrlen参数指出。  

accept成功时返回一个新的连接socket，该socket唯一地标识了被接受的这个连接，服务器可通过读写该socket来与被接受连接对应的客户端通信。accept失败时返回-1并设置errno。  

>我们把执行过 listen调用、处于 LISTEN状态的 socket称为==监听socket==，而所有处于ESTABLISHED状态的socket则称为==连接socket==。  



# 发起连接

服务器通过listen调用来被动接受连接，那么客户端需要通过如下系统调用来主动与服务器建立连接：  

![image-20220913133800848](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913133800848.png)

sockfd参数由socket系统调用返回一个socket。serv_addr参数是服务器监听的socket地址，addrlen参数则指定这个地址的长度。  

connect成功时返回0。==一旦成功建立连接，sockfd就唯一地标识了这个连接，客户端就可以通过读写sockfd来与服务器通信==。



connect失败则返回-1并设置errno。其中两种常见的errno是ECONNREFUSED和
ETIMEDOUT，它们的含义如下：  

- ECONNREFUSED，目标端口不存在，连接被拒绝。  
- ETIMEDOUT，连接超时。



# 关闭连接

关闭一个连接实际上就是关闭该连接对应的socket，这可以通过如下关闭普通文件描述符的系统调用来完成：  

![image-20220913133946117](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913133946117.png)

fd参数是待关闭的socket。不过，close系统调用并非总是立即关闭一个连接，而是将fd的引用计数减1。只有当fd的引用计数为0时，才真正关闭连接。  

>多进程程序中，一次fork系统调用默认将使父进程中打开的socket的引用计数加1，因此我们必须在父进程和子进程中都对该socket执行close调用才能将连接关闭。  



如果无论如何都要立即终止连接（而不是将socket的引用计数减1），可以使用如下的shutdown系统调用（相对于close来说，它是专门为网络编程设计的）：  

![image-20220913134033054](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913134033054.png)

sockfd参数是待关闭的socket。howto参数决定了shutdown的行为：

![image-20220913134051776](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913134051776.png)

shutdown成功时返回0，失败则返回-1并设置errno。





# 数据读写

 对文件的读写操作read和write同样适用于socket。  



## TCP数据读写

socket编程接口提供了几个专门用于socket数据读写的系统调用，它们增加了对数据读写的控制。其中用于TCP流数据读写的系统调用是：

![image-20220913135250224](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913135250224.png)

recv读取sockfd上的数据，buf和len参数分别指定读缓冲区的位置和大小，flags参数的含义见后文，通常设置为0即可。recv成功时返回实际读取到的数据的长度，它可能小于我们期望的长度len。因此我们可能要多次调用recv，才能读取到完整的数据。recv可能返回0，这意味着通信对方已经关闭连接了。recv出错时返回-1并设置errno。



send往sockfd上写入数据，buf和len参数分别指定写缓冲区的位置和大小。send成功时返回实际写入的数据的长度，失败则返回-1并设置errno。  



flags参数为数据收发提供了额外的控制，它可以取表5-4所示选项中的一个或几个的==逻辑或==：

![image-20220913135405046](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913135405046.png)



## UDP数据读写

socket编程接口中用于UDP数据报读写的系统调用是：

![image-20220913135729557](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913135729557.png)

recvfrom读取sockfd上的数据，buf和len参数分别指定读缓冲区的位置和大小。因为UDP通信没有连接的概念，所以我们每次读取数据都需要获取发送端的socket地址，即参数src_addr所指的内容，addrlen参数则指定该地址的长度。  



sendto往sockfd上写入数据，buf和len参数分别指定写缓冲区的位置和大小。dest_addr参数指定接收端的socket地址，addrlen参数则指定该地址的长度。  

>值得一提的是，recvfrom/sendto系统调用也可以用于面向连接（STREAM）的socket的数据读写，只需要把最后两个参数都设置为NULL以忽略发送端/接收端的socket地址（因为我们已经和对方建立了连接，所以已经知道其socket地址了）。  



## 通用数据读写函数

socket编程接口还提供了一对通用的数据读写系统调用。它们不仅能用于TCP流数据，也能用于UDP数据报：  

![image-20220913135846703](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913135846703.png)

sockfd参数指定被操作的目标socket。msg参数是msghdr结构体类型的指针，msghdr结构体的定义如下：  

![image-20220913140141770](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913140141770.png)

msg_name成员指向一个socket地址结构变量。它指定通信对方的socket地址。对于面向连接的TCP协议，该成员没有意义，必须被设置为NULL。这是因为对数据流socket而言，对方的地址已经知道。

msg_namelen成员则指定了msg_name所指socket地址的长度。  

msg_iov成员是iovec结构体类型的指针，iovec结构体的定义如下：

![image-20220913140231384](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913140231384.png)

iovec结构体封装了一块内存的起始位置和长度。msg_iovlen指定这样的iovec结构对象有多少个。  

对于recvmsg而言，数据将被读取并存放在msg_iovlen块分散的内存中，这些内存的位置和长度则由msg_iov指向的数组指定，这称为分散读（scatter read）；对sendmsg而言，msg_iovlen块分散内存中的数据将被一并发送，这称为集中写（gather write）。

msg_control和msg_controllen成员用于辅助数据的传送。我们不详细讨论它们，仅在第13章介绍如何使用它们来实现在进程间传递文件描述符。  

msg_flags成员无须设定，它会复制recvmsg/sendmsg的flags参数的内容以影响数据读写过程。recvmsg还会在调用结束前，将某些更新后的标志设置到msg_flags中。  

recvmsg/sendmsg的flags参数以及返回值的含义均与send/recv的flags参数及返回值相同。  



# 带外标记

但在实际应用中，我们通常无法预期带外数据何时到来。

好在Linux内核检测到TCP紧急标志时，将通知应用程序有带外数据需要接收。内核通知应用程序带外数据到达的两种常见方式是：I/O复用产生的异常事件和SIGURG信号。

但是，即使应用程序得到了有带外数据需要接收的通知，还需要知道带外数据在数据流中的具体位置，才能准确接收带外数据。这一点可通过如下系统调用实现：

 ![image-20220913140521574](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913140521574.png)

sockatmark判断sockfd是否处于带外标记，即下一个被读取到的数据是否是带外数据。如果是，sockatmark返回1，此时我们就可以利用带MSG_OOB标志的recv调用来接收带外数据。如果不是，则sockatmark返回0。  



# 地址信息函数

在某些情况下，我们想知道一个连接socket的本端socket地址，以及远端的socket地址。下面这两个函数正是用于解决这个问题：  

![image-20220913140626877](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913140626877.png)

getsockname获取sockfd对应的本端socket地址，并将其存储于address参数指定的内存中，该socket地址的长度则存储于address_len参数指向的变量中。如果实际socket地址的长度大于address所指内存区的大小，那么该socket地址将被截断。getsockname成功时返回0，失败返回-1并设置errno。  

getpeername获取sockfd对应的远端socket地址，其参数及返回值的含义与getsockname的参数及返回值相同。  



# socket选项

> 5.11节

如果说fcntl系统调用是控制文件描述符属性的通用POSIX方法，那么下面两个系统调用则是专门用来读取和设置socket文件描述符属性的方法：  

![image-20220913140741177](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913140741177.png)

sockfd参数指定被操作的目标socket。level参数指定要操作哪个协议的选项（即属性），比如IPv4、IPv6、TCP等。option_name参数则指定选项的名字。我们在表5-5中列举了socket通信中几个比较常用的socket选项。option_value和option_len参数分别是被操作选项的值和长度。不同的选项具有不同类型的值，如表5-5中“数据类型”一列所示。  

![image-20220913140807162](Linux%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80API.assets/image-20220913140807162.png)

getsockopt和setsockopt这两个函数成功时返回0，失败时返回-1并设置errno。  