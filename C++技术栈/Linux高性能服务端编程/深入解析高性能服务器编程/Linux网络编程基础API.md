# 本章内容

将从如下3个方面讨论Linux网络API：  

- socket地址API。socket最开始的含义是一个IP地址和端口对（ip，port）。它唯一地表示了使用TCP通信的一端。本书称其为socket地址。
- socket基础API。socket的主要API都定义在sys/socket.h头文件中，包括创建socket、命名socket、监听socket、接受连接、发起连接、读写数据、获取地址信息、检测带外标记，以及读取和设置socket选项。  

- 网络信息API。Linux提供了一套网络信息API，以实现主机名和IP地址之间的转换，以及服务名称和端口号之间的转换。这些API都定义在netdb.h头文件中。





# Socket地址API

5.1节