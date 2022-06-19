# 总体结构

- 系统架构图：

  <img src="Spring%20Cloud%20Alibaba%E7%BB%BC%E5%90%88%E9%9B%86%E5%90%88%E6%9E%B6%E6%9E%84.assets/image-20220619205851840.png" alt="image-20220619205851840" style="zoom: 67%;" />

- 说明：

  - API网关 ：系统统一入口，屏蔽架构内部结构，统一安全拦截，采用Zuul实现。
  - application-1 ：应用1，模拟应用，提供http接口服务，使用Feign调用。
  - service-1 ：微服务1，模拟微服务，提供dubbo接口服务。
  - service-2 ：微服务2，模拟微服务，提供dubbo接口服务。

- 调用流程：
  1. 所有访问系统的请求都要经过网关，
  2. 网关转发Http请求至application-1
  3. application-1使用dubbo调用service1完成自身业务，而后sevice1调用service2完成自身业务

> 在架构中，application与sevice的区别：
>
> - service提供了基础服务功能；application组装基础服务功能，提供给用户直接可用的业务。  
> - service服务粒度小、功能基础，不易发生改变；application提供上游业务功能，紧贴业务需求，容易发生改变。  
> - 形成service支撑application的整体架构，增加多变的application甚至不需要变动service  



# 工程结构说明

采用maven工程，结构如下：

```
nacos‐micro‐service 整体父工程
├─api‐gateway API网关，端口：56010
├─application‐1 应用1，端口：56020
├─service‐1 服务1父工程
│  ├─service‐1‐api 服务1API
│  └─service‐1‐server 服务1实现，端口：56030
└─service‐2 服务2父工程
    ├─service‐2‐api 服务2API
    └─service‐2‐server 服务2实现，端口：56040
```



# 创建整体父工程

名为 nacos-micro-service 的 Maven 工程，其pom.xml如下：

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



# Application-1

- pom.xml如下：

  ```xml
  <dependencies>
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
      </dependency>
  
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-web</artifactId>
      </dependency>
  
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-dubbo</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.example</groupId>
          <artifactId>service1-api</artifactId>
          <version>1.0-SNAPSHOT</version>
      </dependency>
  </dependencies>
  ```

- **bootstrap**.yaml：

  ```yaml
  server:
    port: 56020
    servlet:
      context-path: /application1
      
  spring:
    application:
      name: application1
    main:
      allow-bean-definition-overriding: true
    cloud:
      nacos:
        discovery:
          server-addr: 127.0.0.1:8848
          namespace: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          cluster-name: DEFAULT
        config:
          server-addr: 127.0.0.1:8848
          access-key: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          group: NACOS_MICROSERVICE_GROUP
  ```

- **发布配置**

- Controller：

  ```java
  @RestController
  public class HelloController {
  
      @org.apache.dubbo.config.annotation.Reference
      private ConsumerService consumerService;
  
      @GetMapping("/service")
      public String service() {
          String service = consumerService.service();
          return "test" + service;
      }
  }
  ```

- 启动类：

  ```java
  @SpringBootApplication
  @EnableDiscoveryClient
  public class Application1 {
  
      public static void main(String[] args) {
          SpringApplication.run(Application1.class, args);
      }
  }
  ```

  



# Service1

## 创建父工程

pom.xml如下：

```xml
<parent>
    <artifactId>nacos-micro-service</artifactId>
    <groupId>org.example</groupId>
    <version>1.0-SNAPSHOT</version>
</parent>
<modelVersion>4.0.0</modelVersion>

<artifactId>service1</artifactId>
<packaging>pom</packaging>

<name>service1</name>
<modules>
    <module>service1-api</module>
    <module>service1-server</module>
</modules>
```



## 创建service1-api

为了方便其它服务调用dubbo服务，专门定义api工程，**此工程将作为jar被其它工程依赖**

- pom.xml如下：

  ```xml
  <parent>
      <artifactId>service1</artifactId>
      <groupId>org.example</groupId>
      <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>
  
  <artifactId>service1-api</artifactId>
  ```

- 定义服务接口：

  ```java
  package com.itheima.microservice.service1.api;
  
  public interface ConsumerService {
      public String service();
  }
  ```



## 创建service1-server

- pom.xml如下：

  ```xml
  <dependencies>
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
      </dependency>
  
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-web</artifactId>
      </dependency>
  
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-dubbo</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.example</groupId>
          <artifactId>service1-api</artifactId>
          <version>1.0-SNAPSHOT</version>
      </dependency>
      
      <dependency>
          <groupId>org.example</groupId>
          <artifactId>service2-api</artifactId>
          <version>1.0-SNAPSHOT</version>
      </dependency>
  </dependencies>
  ```

- 实现Dubbo服务

  ```java
  package com.itheima.microservice.service1.service;
  
  import com.itheima.microservice.service1.api.ConsumerService;
  
  @org.apache.dubbo.config.annotation.Service // 将此类的方法，暴露为Dubbo接口
  public class ConsumerServiceImpl implements ConsumerService {
  
      @org.apache.dubbo.config.annotation.Reference
      com.itheima.microservice.service2.api.ConsumerService consumerService;
  
      @Override
      public String service() {
          String service = consumerService.service();
          return "Consumer Invoke" + service;
      }
  }
  ```

- bootstrap.yaml：

  ```yaml
  server:
    port: ${port:56030}
  
  spring:
    application:
      name: service1
    main:
      allow-bean-definition-overriding: true
    cloud:
      nacos:
        discovery:
          server-addr: 127.0.0.1:8848
          namespace: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          cluster-name: DEFAULT
        config:
          server-addr: 127.0.0.1:8848
          file-extension: yaml
          access-key: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          group: NACOS_MICROSERVICE_GROUP
  
  dubbo:
    scan:
      base-packages: com.itheima.microservice # Duboo服务扫描基准包
    protocol:
      name: dubbo # 指定为Dubbo协议
      port: ${dubbo_port:20881} # Dubbo协议端口
    registry:
      address: nacos://127.0.0.1:8848
    application:
      qos-enable: false # Dubbo运维服务是否开启
    consumer:
      check: false  # 启动时是否检查依赖的服务
  
  
  ```

  - dubbo.scan.base-packages : 指定 Dubbo 服务实现类的扫描基准包，将
    @org.apache.dubbo.config.annotation.Service注解标注的service暴露为dubbo服务  

  - dubbo.protocol : Dubbo 服务暴露的协议配置，其中子属性 name 为协议名称， port 为dubbo协议端口。

    可以指定多协议，如：dubbo.protocol.rmi.port=1099  

  - dubbo.registry : Dubbo 服务注册中心配置，其中子属性 address 的值 "nacos://127.0.0.1:8848"，说明
    dubbo服务注册到nacos

    相当于原生dubbo的xml配置中的 <dubbo:registry address="10.20.153.10:9090" />

- 启动类：

  ```java
  @SpringBootApplication
  @EnableDiscoveryClient
  public class Service1Bootstrap {
      public static void main(String[] args) {
          SpringApplication.run(Service1Bootstrap.class, args);
      }
  }
  ```



# Service2

## 创建父工程

pom.xml如下：

```xml
<parent>
    <artifactId>nacos-micro-service</artifactId>
    <groupId>org.example</groupId>
    <version>1.0-SNAPSHOT</version>
</parent>
<modelVersion>4.0.0</modelVersion>

<artifactId>service1</artifactId>
<packaging>pom</packaging>

<name>service2</name>
<modules>
    <module>service2-api</module>
    <module>service2-server</module>
</modules>
```



## 创建service2-api

- pom.xml如下：

  ```xml
  <parent>
      <artifactId>service2</artifactId>
      <groupId>org.example</groupId>
      <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>
  
  <artifactId>service2-api</artifactId>
  ```

- 定义服务接口：

  ```java
  package com.itheima.microservice.service2.api;
  
  public interface ConsumerService {
      public String service();
  }
  ```



## 创建service2-server

- pom.xml如下：

  ```xml
  <dependencies>
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
      </dependency>
  
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-web</artifactId>
      </dependency>
  
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-dubbo</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.example</groupId>
          <artifactId>service2-api</artifactId>
          <version>1.0-SNAPSHOT</version>
      </dependency>
  </dependencies>
  ```

- 实现Dubbo服务

  ```java
  package com.itheima.microservice.service2.service;
  
  import com.itheima.microservice.service2.api.ConsumerService;
  
  @org.apache.dubbo.config.annotation.Service // 将此类的方法，暴露为Dubbo接口
  public class ConsumerServiceImpl implements ConsumerService {
  
      @Override
      public String service() {
          return "Consumer2 Invoke";
      }
  }
  
  ```

- bootstrap.yaml：

  ```yaml
  server:
    port: ${port:56040}
  
  spring:
    application:
      name: service2
    main:
      allow-bean-definition-overriding: true
    cloud:
      nacos:
        discovery:
          server-addr: 127.0.0.1:8848
          namespace: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          cluster-name: DEFAULT
        config:
          server-addr: 127.0.0.1:8848
          file-extension: yaml
          access-key: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          group: NACOS_MICROSERVICE_GROUP
  
  dubbo:
    scan:
      base-packages: com.itheima.microservice # Duboo服务扫描基准包
    protocol:
      name: dubbo # 指定为Dubbo协议
      port: ${dubbo_port:20882} # Dubbo协议端口
    registry:
      address: nacos://127.0.0.1:8848
    application:
      qos-enable: false # Dubbo运维服务是否开启
    consumer:
      check: false  # 启动时是否检查依赖的服务
  ```

  - dubbo.scan.base-packages : 指定 Dubbo 服务实现类的扫描基准包，将
    @org.apache.dubbo.config.annotation.Service注解标注的service暴露为dubbo服务  

  - dubbo.protocol : Dubbo 服务暴露的协议配置，其中子属性 name 为协议名称， port 为dubbo协议端口。

    可以指定多协议，如：dubbo.protocol.rmi.port=1099  

  - dubbo.registry : Dubbo 服务注册中心配置，其中子属性 address 的值 "nacos://127.0.0.1:8848"，说明
    dubbo服务注册到nacos

    相当于原生dubbo的xml配置中的 <dubbo:registry address="10.20.153.10:9090" />

- 启动类：

  ```java
  @SpringBootApplication
  @EnableDiscoveryClient
  public class Service2Bootstrap {
      public static void main(String[] args) {
          SpringApplication.run(Service2Bootstrap.class, args);
      }
  }
  ```





# Gateway

## Zuul介绍

- 网关的作用：
  - 提供统一服务入口，让微服务对前台透明
  - 聚合后台的服务，节省流量，提升性能
  - 提供安全，过滤，流控等API管理功能
- Zuul与Nginx在实际项目中需要配合使用：
  - Nginx的作用是反向代理、负载均衡
  - Zuul的作用是保障微服务的安全访问，拦截微服务请求，校验合法性及负载均衡

![image-20220619223808885](Spring%20Cloud%20Alibaba%E7%BB%BC%E5%90%88%E9%9B%86%E5%90%88%E6%9E%B6%E6%9E%84.assets/image-20220619223808885.png)



## 搭建网关工程

- pom.xml：

  ```xml
  <dependencies>
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
      </dependency>
  
      <dependency>
          <groupId>com.alibaba.cloud</groupId>
          <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-web</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.springframework.cloud</groupId>
          <artifactId>spring-cloud-starter-netflix-zuul</artifactId>
      </dependency>
  
      <dependency>
          <groupId>org.springframework.cloud</groupId>
          <artifactId>spring-cloud-starter-openfeign</artifactId>
      </dependency>
  </dependencies>
  ```

- bootstrap.yaml：

  ```yaml
  server:
    port: 56010
  
  spring:
    application:
      name: api-gateway
    main:
      allow-bean-definition-overriding: true
    cloud:
      nacos:
        discovery:
          server-addr: 127.0.0.1:8848
          namespace: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          cluster-name: DEFAULT
        config:
          server-addr: 127.0.0.1:8848
          file-extension: yaml
          access-key: 42019ee9-4c0b-4516-9831-8bd6d43b4f1c
          group: NACOS_MICROSERVICE_GROUP
  
  zuul:
    routes:
      application1:
        strip-prefix: false
        path: /application1/**
  ```

  将请求为 `/application1/` 开头的请求路由至 application1 服务，保留请求url中的 `/application1/ `。

- 启动类：

  ```java
  @SpringBootApplication
  @EnableDiscoveryClient
  @EnableZuulProxy
  public class GatewayApplication {
  
      public static void main(String[] args) {
          SpringApplication.run(GatewayApplication.class, args);
      }
  }
  ```

  

测试：访问http://localhost:56010/application1/service

> 注意：访问的网关，网关将该请求路由至Application1