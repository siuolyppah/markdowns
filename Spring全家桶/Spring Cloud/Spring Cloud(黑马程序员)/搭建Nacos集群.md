#Nacos集群结构

官方给出的Nacos集群图：

![image-20220315004755349](%E6%90%AD%E5%BB%BANacos%E9%9B%86%E7%BE%A4.assets/image-20220315004755349.png)

- SLB：即为负载均衡器。可以使用Nginx
- Nacos：即为Nacos服务



## 集群Demo结构

<img src="%E6%90%AD%E5%BB%BANacos%E9%9B%86%E7%BE%A4.assets/image-20220315004930959.png" alt="image-20220315004930959" style="zoom:80%;" />

三个nacos节点的地址：

| 节点   | ip            | port |
| ------ | ------------- | ---- |
| nacos1 | 192.168.150.1 | 8845 |
| nacos2 | 192.168.150.1 | 8846 |
| nacos3 | 192.168.150.1 | 8847 |



# 集群Demo搭建

搭建集群的基本步骤：

- 搭建数据库，初始化数据库表结构
- 下载nacos安装包
- 配置nacos
- 启动nacos集群
- nginx反向代理



## 初始化数据库

Nacos默认数据存储在内嵌数据库Derby中，不属于生产可用的数据库。

官方推荐的最佳实践是使用带有主从的高可用数据库集群。



<u>但此处以单点的数据库为例</u>：

​	新建名为nacos的数据库，导入此[SQL文件](.\搭建Nacos集群.assets\nacos_db.sql")

## 安装Nacos

## 配置Nacos

1. 进入nacos的conf目录，修改配置文件cluster.conf.example，重命名为cluster.conf。添加内容：

    ```
    127.0.0.1:8845
    127.0.0.1.8846
    127.0.0.1.8847
    ```

2. 修改application.properties文件（同为nacos的conf目录下），添加数据库配置：

    ```properties
    spring.datasource.platform=mysql
    
    db.num=1
    
    db.url.0=jdbc:mysql://127.0.0.1:3306/nacos?characterEncoding=utf8&connectTimeout=1000&socketTimeout=3000&autoReconnect=true&useUnicode=true&useSSL=false&serverTimezone=UTC
    db.user.0=root
    db.password.0=123
    ```

3. 分别修改三个文件夹中的application.properties：

    ```properties
    server.port=8845
    ```

    ![image-20220315010001807](%E6%90%AD%E5%BB%BANacos%E9%9B%86%E7%BE%A4.assets/image-20220315010001807.png)

## 启动Nacos（以集群方式）

```sh
startup.cmd
```

## nginx反向代理

1. 下载并解压nginx（nginx-1.18.0）

2. 修改conf/nginx.conf文件，配置如下：

    ```ngin
    upstream nacos-cluster {
        server 127.0.0.1:8845;
    	server 127.0.0.1:8846;
    	server 127.0.0.1:8847;
    }
    
    server {
        listen       80;
        server_name  localhost;
    
        location /nacos {
            proxy_pass http://nacos-cluster;
        }
    }
    ```

    > 此后在在浏览器访问http://localhost/nacos，即可访问到被负载均衡后的Nacos集群节点

3. 因此，springboot的application.yml文件配置如下：
	
	```yaml
	spring:
	  cloud:
	    nacos:
	      server-addr: localhost:80 # Nacos地址
	```
	
	

