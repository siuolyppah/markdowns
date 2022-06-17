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

### 修改密码

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



### 关闭登录功能

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

## 从单体架构到微服务

### 单体架构

Web应用程序发展的早期，大部分web工程师将所有的功能模块打包到一起并放在一个web容器中运行，所有功能
模块使用同一个数据库，同时，它还提供API或者UI访问的web模块等。

<img src="Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220617195404689.png" alt="image-20220617195404689" style="zoom:50%;" />

尽管也是模块化逻辑，但是最终它还是会打包并部署为单体式应用，这种将**所有功能都部署在一个web容器**中运行
的系统就叫做单体架构（也叫：巨石型应用）。  



单体架构有很多好处：

- 开发效率高：模块之间交互采用本地方法调用，并节省微服务之间的交互讨论时间与开发成本。
- 容易测试：IDE都是为开发单个应用设计的、容易测试——在本地就可以启动完整的系统。
- 容易部署：运维成本小，直接打包为一个完整的包，拷贝到web容器的某个目录下即可运行。



但是，上述的好处是有条件的，它适用于小型简单应用，对于大规模的复杂应用，就会展现出来以下的不足：

- 复杂性逐渐变高，可维护性逐渐变差 ：所有业务模块部署在一起，复杂度越来越高，修改时牵一发动全身。
- 版本迭代速度逐渐变慢：修改一个地方就要将整个应用全部编译、部署、启动时间过长、回归测试周期过长。
- 阻碍技术创新：若更新技术框架，除非你愿意将系统全部重写，无法实现部分技术更新。
- 无法按需伸缩：通过冗余部署完整应用的方式来实现水平扩展，无法针对某业务按需伸缩。



### 微服务

- 许多大型公司，通过采用微服务架构解决了上述问题。其思路不是开发一个巨大的单体式的应用，而是将应用分解为小的、互相连接的微服务。

- 一个微服务一般完成某个特定的功能，比如订单服务、用户服务等等。

  **每一个微服务都是完整应用，都有自己的业务逻辑和数据库**。一些微服务还会发布API给其它微服务和应用客户端使用。



比如，根据前面描述系统可能的分解如下：

<img src="Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220617195619222.png" alt="image-20220617195619222" style="zoom:50%;" />

每一个业务模块都使用独立的服务完成，这种微服务架构模式也影响了应用和数据库之间的关系，不像传统多个业务模块共享一个数据库，**微服务架构每个服务都有自己的数据库**。  



微服务架构的好处：

- 分而治之，职责单一；易于开发、理解和维护、方便团队的拆分和管理
- 可伸缩；能够单独的对指定的服务进行伸缩
- 局部容易修改，容易替换，容易部署，有利于持续集成和快速迭代
- 不会受限于任何技术栈



## Spring Cloud  与 Spring Cloud Alibaba  

- Spring Cloud是一系列框架的有序集合。它利用Spring Boot的开发便利性巧妙地简化了分布式系统基础设施的开发，如服务发现注册、配置中心、消息总线、负载均衡、断路器、数据监控等，都可以用Spring Boot的开发风格做到一键启动和部署。

- Spring Cloud Alibaba是阿里巴巴公司提供的开源的基于Spring cloud的微服务套件合集，它致力于提供微服务开发的一站式解决方
  案。

  > 可以理解为spring cloud是一套微服务开发的标准 ，spring cloud alibaba与spring cloud Netflix是实现。

  使用 Spring Cloud Alibaba方案，开发者只需要添加一些注解和少量配置，就可以将 Spring Cloud 应用接入阿里分布式应用解决方案，通过阿里中间件来迅速搭建分布式应用系统。  



以下将使用Spring Cloud Alibaba Nacos Config在Spring Cloud应用中集成Nacos。



## 分布式应用配置管理  

下图展示了如何通过Nacos集中管理多个服务的配置：

![image-20220617195737451](Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220617195737451.png)

- 用户**通过Nacos Server的控制台**，集中对多个服务的配置进行管理。
- 各服务统一从Nacos Server中获取各自的配置，并监听配置的变化  



### 1. 发布配置

首先在nacos发布配置，我们规划了两个服务service1、service2，并且想对这两个服务的配置进行集中维护。

1. 浏览器访问 http://127.0.0.1:8848/nacos ，打开nacos控制台，并点击菜单配置管理->配置列表

2. 添加如下配置：

   - service1

     ```
     Namespace: c67e4a97‐a698‐4d6d‐9bb1‐cfac5f5b51c4 #开发环境
     Data ID: service1.yaml
     Group : TEST_GROUP
     配置格式: YAML
     配置内容： 
     common:
     	name: service1 config
     ```

   - service2

     ```
     Namespace: c67e4a97‐a698‐4d6d‐9bb1‐cfac5f5b51c4 #开发环境
     Data ID: service2.yaml
     Group : TEST_GROUP
     配置格式: YAML
     配置内容： 
     common:
     	name: service2 config
     ```



### 创建父工程

pom.xml如下：

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-alibaba-dependencies</artifactId>
            <version>2.1.0.RELEASE</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>Greenwich.RELEASE</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-dependencies</artifactId>
            <version>2.1.3.RELEASE</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```



### 微服务service1配置

- pom.xml如下：

  ```xml
  <parent>
      <artifactId>nacos-simple-demo</artifactId>
      <groupId>org.example</groupId>
      <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>
  
  <artifactId>service1</artifactId>
  
  <name>service1</name>
  
  
  <properties>
      <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
      <maven.compiler.source>11</maven.compiler.source>
      <maven.compiler.target>11</maven.compiler.target>
  </properties>
  
  <dependencies>
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
      </dependency>
      <dependency>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-web</artifactId>
      </dependency>
  </dependencies>
  ```

- 配置 **bootstrap**.yaml：

  ```yaml
  server:
    port: 56010 #启动端口 命令行注入
    
  spring:
    application:
      name: service1
    cloud:
      nacos:
        config:
          server‐addr: localhost:8848 # 配置中心地址
          file‐extension: yaml
          namespace: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c # 开发环境
          group: TEST_GROUP # 测试组
  ```

  >- spring-cloud-starter-alibaba-nacos-config 在加载配置的时候：
  >
  >  dataid为：`${spring.application.name}.${file-extension:properties}` 的基础配置  
  >
  >- 若没有指定spring.cloud.nacos.config.group配置,则默认为DEFAULT_GROUP

- 启动配置客户端：

  ```java
  @SpringBootApplication
  public class Service1Bootstrap {
      
      public static void main(String[] args) {
          SpringApplication.run(Service1Bootstrap.class, args);
      } 
  }
  ```



### 微服务service2配置

- pom.xml如下：

  ```xml
  <parent>
      <artifactId>nacos-simple-demo</artifactId>
      <groupId>org.example</groupId>
      <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>
  
  <artifactId>service2</artifactId>
  
  <name>service2</name>
  
  
  <properties>
      <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
      <maven.compiler.source>11</maven.compiler.source>
      <maven.compiler.target>11</maven.compiler.target>
  </properties>
  
  <dependencies>
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
      </dependency>
      <dependency>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-web</artifactId>
      </dependency>
  </dependencies>
  ```

- 配置 **bootstrap**.yaml：

  ```yaml
  server:
    port: 56020 #启动端口 命令行注入
  
  spring:
    application:
      name: service2
    cloud:
      nacos:
        config:
          server‐addr: localhost:8848 # 配置中心地址
          file‐extension: yaml
          namespace: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c # 开发环境
          group: TEST_GROUP # 测试组
  ```

- 启动配置客户端：

  ```java
  @SpringBootApplication
  public class Service1Bootstrap {
      
      public static void main(String[] args) {
          SpringApplication.run(Service1Bootstrap.class, args);
      } 
  }
  ```

  

### 2. 获取配置

```java
@RestController
public class ConfigController {

    // 使用Value注解读取配置信息
    @Value("${common.name}")
    private String name;

    @GetMapping("/configs")
    public String getConfigs() {
        return name;
    }
}
```



> 注意：
>
> - 当在控制台更改配置时，客户端将进行配置的更新。
> - 但`@Value`注解比较特殊，不会进行变化。



### 3. 配置的动态更新

若要实现配置的动态更新，只需要进行如下改造：  

```java
@RestController
public class ConfigController {

    //    @Value("${common.name}")
    //    private String name;
    //
    //    @GetMapping("/configs")
    //    public String getConfigs() {
    //        return name;
    //    }

    // 注入配置文件上下文
    @Autowired
    private ConfigurableApplicationContext applicationContext;

    @GetMapping("/configs")
    public String getConfigs() {
        return applicationContext.getEnvironment().getProperty("common.name");
    }
}
```



>可通过配置spring.cloud.nacos.config.refresh.enabled=false，来关闭动态刷新



### 扩展 Data Id 配置

Spring Cloud Alibaba Nacos Config可支持自定义 Data Id 的配置，例如：
```yaml
server:
  port: 56010 #启动端口 命令行注入

spring:
  application:
    name: service1
  cloud:
    nacos:
      config:
        server‐addr: localhost:8848 # 配置中心地址
        file‐extension: yaml
        namespace: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c # 开发环境
        group: TEST_GROUP # 测试组
        ext-config[0]:
          data-id: ext-config-common01.properties # 默认的组 DEFAULT_GROUP,不支持配置的动态刷新
        ext-config[1]:
          data-id: ext-config-common02.properties # 组 GLOBALE_GROUP,不支持配置的动态刷新
          group: GLOBALE_GROUP
        ext‐config[2]:
          data‐id: ext-config-common03.properties # 组 REFRESH_GROUP，支持动态刷新
          group: REFRESH_GROUP
          refresh: true
```

> 在进行拓展配置时，必须指定文件拓展名



### 共享 Data Id 配置  

yaml如下：

```yaml
spring:
  cloud:
    nacos:
      config:
        shared‐dataids: ext‐config‐common01.properties,ext‐config‐common02.properties
        refreshable‐dataids: ext‐config‐common01.properties
```



> 共享 Data ID 配置，只有属于DEFAULT_GROUP的配置才生效。
>
> > 因此建议使用拓展 Data ID配置



### 配置的优先级问题

Spring Cloud Alibaba Nacos Config 目前提供了三种配置能力从 Nacos 拉取相关的配置。

- A: 通过 `spring.cloud.nacos.config.shared-dataids` 支持多个共享 Data Id 的配置
- B: 通过 `spring.cloud.nacos.config.ext-config[n].data-id` 的方式支持多个扩展 Data Id 的配置，多个Data Id 同时配置时，他的优先级关系是 `spring.cloud.nacos.config.ext-config[n].data-id` 其中 n 的值越大，优先级越高。

- C: 通过内部相关规则(应用名、扩展名 )自动生成相关的 Data Id 配置（即项目本身的配置）

当三种方式共同使用时，他们的一个优先级关系是:**C > B >A**

> 优先级高的配置，将覆盖优先级低的配置



### 完全关闭配置  

通过设置 spring.cloud.nacos.config.enabled = false 来完全关闭 Spring Cloud Nacos Config



## Nacos Server集群部署  

### 集群部署

3个或3个以上Nacos节点才能构成集群



1. 安装3个以上Nacos

   可以复制之前已经解压好的nacos文件夹，分别命名为nacos、nacos1、nacos2

   >如果服务器有多个ip也要指定具体的ip地址，如：nacos.inetutils.ip-address=127.0.0.1
   >
   >```
   >server.port=8850
   >nacos.inetutils.ip‐address=127.0.0.1
   >```

2. 配置集群配置文件

   在所有nacos目录的conf目录下，有文件 cluster.conf.example ，将其命名为 cluster.conf ，并将每行配置成
   ip:port。（请配置3个或3个以上节点）

   ```
   # ip:port
   127.0.0.1:8848
   127.0.0.1:8849
   127.0.0.1:8850
   ```

3. 集群模式启动

   **分别执行**nacos目录的bin目录下的startup：

   ```sh
   startup ‐m cluster
   ```

   

### 客户端配置

yaml如下：

```yaml
spring:
 application:
  name: xxxx
 cloud:
  nacos:
   config:
    server‐addr: 127.0.0.1:8848,127.0.0.1:8849,127.0.0.1:8850
```



### 生产环境的部署建议

- 下图是官方推荐的集群方案，通过域名 + VIP模式的方式来实现。

  客户端配置的nacos，当Nacos集群迁移时，**客户端配置无需修改**。  

![image-20220617221154513](Nacos%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220617221154513.png)



至于数据库，生产环境下建议至少主备模式。通过修改${nacoshome}/conf/application.properties文件，能够使
nacos拥有多个数据源。  

```properties
spring.datasource.platform=mysql

db.num=2
db.url.0=jdbc:mysql://127.0.0.1:3306/nacos_config?characterEncoding=utf8&autoReconnect=true
db.url.1=jdbc:mysql://127.0.0.1:3306/nacos_config?characterEncoding=utf8&autoReconnect=true
db.user=root
db.password=root
```

