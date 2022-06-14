# 什么是配置中心

## 什么是配置

应用程序在启动和运行的时候往往需要读取一些配置信息，配置基本上伴随着应用程序的整个生命周期，比如：数据库连接参数、启动参数等。  



配置主要有以下几个特点：

- 配置是独立于程序的只读变量：

  配置对于程序是只读的，程序通过读取配置来改变自己的行为，**但是程序不应该去改变配置**

- 配置伴随应用的整个生命周期：

  配置贯穿于应用的整个生命周期，**应用在启动时通过读取配置来初始化**，在运行时根据配置调整行为。

  > 比如：启动时需要读取服务的端口号、系统在运行过程中需要读取定时策略执行定时任务等。  

- 配置可以有多种加载方式：

  常见的有程序内部hard code，配置文件，环境变量，启动参数，基于数据库等

- 配置需要治理：

  同一份程序在不同的环境（开发，测试，生产）、不同的集群（如不同的数据中心）经常需要有不同的配置，所以需要有完善的环境、集群配置管理  



## 什么是配置中心

在微服务架构中，当系统从一个单体应用，被拆分成分布式系统上一个个服务节点后，配置文件也必须跟着迁移（分割），这样配置就分散了，不仅如此，分散中还包含着冗余，如下图  ：

![image-20220614132450463](Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220614132450463.png)



下图显示了配置中心的功能，配置中心将配置从各应用中剥离出来，对配置进行统一管理，**应用自身不需要自己去管理配置**。  

![image-20220614132512895](Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220614132512895.png)



配置中心的服务流程如下：

1. 用户在配置中心更新配置信息。  
2. 服务A和服务B及时得到配置更新通知，从配置中心获取配置。  





# Nacos快速入门

## 安装Nacos Server  

### 系统环境

1. 64 bit OS，支持 Linux/Unix/Mac/Windows，推荐选用 Linux/Unix/Mac。
2. 64 bit JDK 1.8+；
3. Maven 3.2.x+；
4. nacos-server-1.1.3  



### 下载Nacos源码或安装包

可以通过源码和发行包两种方式来获取 Nacos：

- 从Github 上下载源码方式：

  ```sh
  git clone https://github.com/alibaba/nacos.git
  cd nacos/
  mvn ‐Prelease‐nacos clean install ‐U
  ls ‐al distribution/target/
  
  // change the $version to your actual path
  cd distribution/target/nacos‐server‐$version/nacos/bin
  ```

- 下载已经编译过的压缩包：

  1. 下载地址：https://github.com/alibaba/nacos/releases

  2. 下载后解压：

     ```sh
     unzip nacos‐server‐$version.zip 或者 tar ‐xvf nacos‐server‐$version.tar.gz
     cd nacos/bin
     ```

>本教程版本：nacos-server-1.1.3  



### 启动服务器

nacos的默认端口是8848，需要保证8848默认端口没有被其他进程占用。



进入安装程序的bin目录：

- linux/unix/mac：

  ```sh
  sh startup.sh -m standalone
  ```

- Windows启动方式：

  ```sh
  cmd startup.cmd
  ```

  >standalone代表着单机模式运行，非集群模式  



启动成功，可通过浏览器访问 http://127.0.0.1:8848/nacos ，打开如下nacos控制台登录页面

> - 默认用户名：nacos
> - 默认密码：nacos



### OPEN API 配置管理测试  

启动nacos成功后，可通过nacos提供的http api验证nacos服务运行是否正常。  



以curl工具来测试nacos的open api：  

- curl的windows版本：curl-7.66.0_2-win64-mingw，[下载地址](https://curl.haxx.se/windows/)



- 发布配置：

  ```sh
  curl -X POST "http://127.0.0.1:8848/nacos/v1/cs/configs?dataId=nacos.cfg.dataId&group=test&content=HelloWorld"
  ```

- 获取配置：

  ```sh
  curl -X GET "http://127.0.0.1:8848/nacos/v1/cs/configs?dataId=nacos.cfg.dataId&group=test"
  ```

  

### 关闭服务器

- linux/unix/mac：

  ```sh
  sh shutdown.sh
  ```

- Windows启动方式：

  ```sh
  cmd shutdown.cmd
  ```



### 外部MySQL数据库支持

- **单机模式**时，nacos**默认使用嵌入式数据库**实现数据的存储

- 若想使用外部mysql存储nacos数据，需要进行以下步骤：  

  1. 安装数据库，版本要求：5.6.5+ ，mysql 8 以下

  2. 初始化mysql数据库，新建数据库nacos_config，数据库初始化文件：${nacoshome}/conf/nacosmysql.sql  

  3. 修改${nacoshome}/conf/application.properties文件，增加支持mysql数据源配置（目前只支持mysql），添加mysql数据源的url、用户名和密码：

     ```properties
     spring.datasource.platform=mysql
     
     db.num=1
     db.url.0=jdbc:mysql://11.162.196.16:3306/nacos_config?
     characterEncoding=utf8&connectTimeout=1000&socketTimeout=3000&autoReconnect=true
     db.user=nacos_devtest
     db.password=youdontknow
     ```

     

## Nacos配置管理入门

### 发布配置

浏览器访问 http://127.0.0.1:8848/nacos ，打开nacos控制台，并点击菜单配置管理->配置列表，添加如下配置：

```
Data ID: nacos‐simple‐demo.yaml
Group : DEFAULT_GROUP
配置格式: YAML
配置内容： 
common:
	config1: something
```

>dataid默认以 properties为扩展名  



### Nacos客户端获取配置

新建一Maven项目，依赖如下：

```xml
<dependency>
    <groupId>com.alibaba.nacos</groupId>
    <artifactId>nacos‐client</artifactId>
    <version>1.1.3</version>
</dependency>
```



测试类如下：

```java
package com.itheima.nacos;
import com.alibaba.nacos.api.NacosFactory;
import com.alibaba.nacos.api.config.ConfigService;
import com.alibaba.nacos.api.exception.NacosException;
import java.util.Properties;

public class SimpleDemoMain {
    public static void main(String[] args) throws NacosException {
        //nacos 地址
        String serverAddr = "127.0.0.1:8848";
        //Data Id
        String dataId = "nacos-simple-demo.yaml";
        //Group
        String group = "DEFAULT_GROUP";
        Properties properties = new Properties();
        properties.put("serverAddr", serverAddr);
        
        ConfigService configService = NacosFactory.createConfigService(properties);
        
        //获取配置,String dataId, String group, long timeoutMs
        String content = configService.getConfig(dataId, group, 5000);
        System.out.println(content);
    }
}
```



# Nacos配置管理基础应用

## Nacos配置管理模型

对于Nacos配置管理，通过`Namespace、group、Data ID`能够定位到一个配置集。

![image-20220614143502516](Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220614143502516.png)

- 配置集（Data ID）：

  在系统中，一个配置文件通常就是一个配置集，一个配置集可以包含了系统的各种配置信息。

  例如，一个配置集可能包含了数据源、线程池、日志级别等配置项。每个配置集都可以定义一个有意义的名称，就是配置集的ID即Data ID。

- 配置分组(Group)：

  - 配置分组是对配置集进行分组，通过一个有意义的字符串（如 Buy 或 Trade）来表示，**不同的配置分组下可以有相同的配置集**（Data ID）。
  - 当您在 Nacos 上创建一个配置时，如果未填写配置分组的名称，则配置分组的名称默认采用 DEFAULT_GROUP 。  

  >配置分组的常见场景：可用于区分不同的项目或应用  

- 命名空间(Namespace)：

  - 命名空间（namespace）可用于进行不同环境的配置隔离。

    例如可以隔离开发环境、测试环境和生产环境，因为它们的配置可能各不相同，或者是隔离不同的用户，不同的开发人员使用同一个nacos管理各自的配置，可通过namespace隔离。

  - 不同的命名空间下，可以存在相同名称的配置分组(Group) 或 配置集。  



最佳实践：

- Namespace：代表不同环境，如开发、测试、生产环境。
- Group：代表某项目，如XX医疗项目、XX电商项目。
- DataId：每个项目下往往有若干个工程，每个配置集(DataId)是一个工程的主配置文件  

![image-20220614143836102](Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220614143836102.png)





在获取配置集时：

- nacos服务地址，必须指定
- namespace，如不指定默认public  
- group，如不指定默认 DEFAULT_GROUP  
- dataId，必须指定  

```java
// 初始化配置服务，
String serverAddr = "127.0.0.1:8848";
String namespace = "ee247dde‐d838‐425c‐b371‐029dab26232f"; //开发环境
String group = "DEFAULT_GROUP"; //默认组
String dataId = "nacos‐simple‐demo.yaml";

Properties properties = new Properties();
properties.put("serverAddr", serverAddr);
properties.put("namespace", namespace);

ConfigService configService = NacosFactory.createConfigService(properties);

//获取配置，并输出控制台
String content = configService.getConfig(dataId, group, 5000);
System.out.println(content);
```

以上代码说明将从地址为127.0.0.1:8848的nacos配置中心获取配置，通过以下信息定位配置集：

- namespace：ee247dde-d838-425c-b371-029dab26232f
- group：DEFAULT_GROUP
- Data Id：nacos-simple-demo.yaml



## 配置管理

### 配置列表

- 新增配置 与 DIFF 展示
- 配置集导出与导入
- 配置集克隆：用于将配置迁移到其他Namespace  



### 历史版本

可用于回滚配置集



### 监听查询

Nacos提供配置订阅者即监听者查询能力，同时提供客户端当前配置的MD5校验值，以便帮助用户更好的**检查配置变更是否推送到 Client 端**。  



通过以下代码可对某配置进行监听：

```java
public class SimpleDemoMainListener {
    public static void main(String[] args) throws NacosException {
        //nacos 地址
        String serverAddr = "127.0.0.1:8848";
        //Data Id
        String dataId = "nacos‐simple‐demo.yaml";
        //Group
        String group = "DEFAULT_GROUP";
        Properties properties = new Properties();
        properties.put("serverAddr",serverAddr);
        ConfigService configService = NacosFactory.createConfigService(properties);
        //获取配置,String dataId, String group, long timeoutMs
        String content = configService.getConfig(dataId, group, 5000);
        System.out.println(content);
        
        //添加监听String dataId, String group, Listener listener
        configService.addListener(dataId, group, new Listener() {
            public Executor getExecutor() {
                return null;
            } 
            
            public void receiveConfigInfo(String s) {
                //当配置发生变化时的响应
                System.out.println(s);
            }
        });
        
        // 测试让主线程不退出，因为订阅配置是守护线程，主线程退出守护线程就会退出。 正式代码中无需下面代
        码
            while (true) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
    }
}
```

控制台输出如下：

```
SLF4J: Failed to load class "org.slf4j.impl.StaticLoggerBinder".
SLF4J: Defaulting to no-operation (NOP) logger implementation
SLF4J: See http://www.slf4j.org/codes.html#StaticLoggerBinder for further details.
common:
    config1: true

common:
    config1: false
```



![image-20220614144957335](Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220614144957335.png)



## 登陆管理

修改密码的流程如下：

1. 引入加密工具类依赖：

   ```xml
   <dependency>
       <groupId>org.springframework.security</groupId>
       <artifactId>spring‐security‐core</artifactId>
       <version>5.1.4.RELEASE</version>
   </dependency>
   ```

   ```java
   public class PasswordEncoderUtil {
       public static void main(String[] args) {
           System.out.println(new BCryptPasswordEncoder().encode("123"));
       }
   }
   ```

   >采用BCrypt加密方法在每次生成密码时会加随机盐，所以生成密码每次可能不一样。  

2. 将上边程序输出的密码更新到数据库：

   ```sql
   INSERT INTO users (username, password, enabled) VALUES ('nacos1','$2a$10$SmtL5C6Gp2sLjBrhrx1vj.dJAbJLa4FiJYZsBb921/wfvKAmxKWyu', TRUE);
   
   INSERT INTO roles (username, role) VALUES ('nacos1', 'ROLE_ADMIN');
   ```

   > 注意：
   >
   > - 登陆用明文密码
   > - 数据库存储暗文密码



关闭登陆功能：

由于部分公司自己开发控制台，不希望被nacos的安全filter拦截。因此nacos支持定制关闭登录功能找到配置文件
${nacoshome}/conf/application.properties ， 替换以下内容即可  

```properties
## spring security config
### turn off security
spring.security.enabled=false
management.security=false
security.basic.enabled=false
nacos.security.ignore.urls=/**

#nacos.security.ignore.urls=/,/**/*.css,/**/*.js,/**/*.html,/**/*.map,/**/*.svg,/**/*.png,/**/*.ico,/console‐fe/public/**,/v1/auth/login,/v1/console/health,/v1/cs/**,/v1/ns/**,/v1/cmdb/**,/actuator/**
```







# Nacos配置管理应用于分布式系统

[Spring Cloud Alibaba Nacos配置中心与服务发现_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1VJ411X7xX?p=10&spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)

