# 什么是服务发现

## 配置文件方式的远程调用

代码通过读取配置文件的方式，读取服务生产方的网络地址（IP和端口号）

![image-20220618131451191](Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220618131451191.png)



- 服务生产方（Service B）：

  - Controller：

    ```java
    @SpringBootApplication
    @RestController
    public class SpringRestProviderBootstrap {
    
        public static void main(String[] args) {
            SpringApplication.run(SpringRestProviderBootstrap.class, args);
        }
        
        @GetMapping(value = "/service") //暴露服务
        public String service(){
            return "provider invoke";
        }
    }
    ```

  - 配置文件

    ```yaml
    server.port = 56010
    ```

- 服务消费方（Service A）：

  - Controller：

    ```java
    @SpringBootApplication
    @RestController
    public class SpringRestConsumerBootstrap {
        
        public static void main(String[] args) {
            SpringApplication.run(SpringRestConsumerBootstrap.class, args);
        } 
        
        @Value("${provider.address}")
        private String providerAddress;
    
        @GetMapping(value = "/service")
        public String service(){
            RestTemplate restTemplate = new RestTemplate();
            //调用服务
            String providerResult = restTemplate.getForObject("http://" + providerAddress +
                                                              "/service",String.class);
            return "consumer invoke | " + providerResult;
        }
    }
    ```

  - 配置文件：

    ```yaml
    server.port = 56020
    
    provider.address = 127.0.0.1:56010	# 服务生产方地址
    ```

    

但此方案对于微服务应用而言行不通。

- 微服务可能是部署在云环境的，服务实例的网络位置或许是动态分配的  
- 每一个服务一般会有多个实例来做负载均衡，由于宕机或升级，服务实例网络地址会经常动态改变



## 基于服务发现的远程调用

<img src="Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220618131823403.png" alt="image-20220618131823403" style="zoom:67%;" />

上图中服务实例本身并**不记录服务生产方的网络地址**，所有服务实例内部都会**包含服务发现客户端**。

1. 在每个服务启动时会向服务发现中心上报自己的网络位置。  

   在服务发现中心内部会形成一个**服务注册表**，服务注册表是服务发现的核心部分，是**包含所有服务实例的网络地址的数据库** 。

2. 服务发现<u>客户端会定期从服务发现中心同步服务注册表</u> ，并缓存在客户端。

3. 当需要对某服务进行请求时，服务实例通过该注册表，定位目标服务网络地址。若目标服务存在多个网络地址，则使用负载均衡算法从多个服务实例中选择出一个，然后发出请求。  



> 即：各服务实例上报自己的网络地址，在服务中心形成一个完整的服务注册表，各服务实例会通过服务发现中心来获取访问目标服务的网络地址，从而实现服务发现的机制。  



# Nacos的四大功能

- 服务发现与服务健康检查：

  Nacos使服务更容易注册，并通过DNS或HTTP接口发现其他服务，Nacos还提供服务的实时健康检查，以防止向不健康的主机或服务实例发送请求。  

- 动态配置管理：

  动态配置服务允许您在所有环境中以集中和动态的方式管理所有服务的配置。Nacos消除了在更新配置时重新部署应用程序，这使配置的更改更加高效和灵活。  

- 动态DNS服务：

  Nacos提供基于DNS 协议的服务发现能力，旨在支持异构语言的服务发现，支持将注册在Nacos上的服务以域名的方式暴露端点，让三方应用方便的查阅及发现。

- 服务和元数据管理：

  Nacos 能让您从微服务平台建设的视角管理数据中心的所有服务及元数据，包括管理服务的描述、生命周期、服务的静态依赖分析、服务的健康状态、服务的流量管理、路由及安全策略。  



# Nacos 服务发现快速入门

## Spring Cloud服务协作流程

Spring Cloud 常见的集成方式是使用Feign+Ribbon技术来完成服务间远程调用及负载均衡的，如下图：

<img src="Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220618132955181.png" alt="image-20220618132955181" style="zoom: 80%;" />

1. 在微服务启动时，会向服务发现中心上报自身实例信息（IP+Port）

2. 微服务会定期从Nacos Server(服务发现中心)获取服务实例列表

3. 当ServiceA调用ServiceB时，**ribbon组件从本地服务实例列表中查找ServiceB的实例**。

   如获取了多个实例如Instance1、Instance2。这时ribbon会通过用户所配置的负载均衡策略从中选择一个实例。  

4. **Feign组件会通过ribbon选取的实例发送http请求**



## 负载均衡 Ribbon

负载均衡就是将用户请求（流量）通过一定的策略，分摊在多个服务实例上执行，它是系统处理高并发、缓解网络
压力和进行服务端扩容的重要手段之一。它分为<u>服务端负载均衡和客户端负载均衡</u>。  



- 服务器端负载均衡：

  <img src="Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220618133904859.png" alt="image-20220618133904859" style="zoom:50%;" />

  在负载均衡器中维护一个可用的服务实例清单，**当客户端请求来临时**，负载均衡服务器按照某种配置好的规则(负载均衡算法)从可用服务实例清单中选取其一去处理客户端的请求。

  >如通过Nginx进行负载均衡，客户端发送请求至Nginx，Nginx通过负载均衡算法，在多个服务器之间选择一个进行访问。 

- 客户端服务负载均衡：

  <img src="Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220618134027210.png" alt="image-20220618134027210" style="zoom: 50%;" />

  Ribbon就属于客户端负载均衡。在ribbon客户端会有一个服务实例地址列表，在发送请求前通过负载均衡算法选择一个服务实例，然后进行访问，这是客户端负载均衡。

  > 如service1调用service2，其中不需要再经过负载均衡器。



Ribbon核心组件IRule是负载均衡策略接口，它有如下实现：

- RoundRobinRule(默认):轮询，即按一定的顺序轮换获取实例的地址。

- RandomRule:随机，即以随机的方式获取实例的地址。

- AvailabilityFilteringRule: 会先过滤掉由于多次访问故障而处于断路器跳闸状态的服务,以及并发的连接数量超过阈值的服务,然后对剩余的服务列表按照轮询策略进行访问;  

- WeightedResponseTimeRule: 根据平均响应时间计算所有服务的权重,响应时间越快,服务权重越大,被选中的机率越高;  

  刚启动时,如果统计信息不足,则使用RoundRobinRule策略,等统计信息足够时,会切换到WeightedResponseTimeRule  

- RetryRule: 先按照RoundRobinRule的策略获取服务,如果获取服务失败,则在指定时间内会进行重试,获取可用的服务;  

- BestAvailableRule: 会先过滤掉由于多次访问故障而处于断路器跳闸状态的服务,然后选择一个并发量最小的服务;  

- ZoneAvoidanceRule: 默认规则,复合判断server所在区域的性能和server的可用性选择服务器;

可通过下面方式在spring boot 配置文件中修改默认的负载均衡策略：

```properties
account‐service.ribbon.NFLoadBalancerRuleClassName=com.netflix.loadbalancer.RandomRule
```

> 其中 account-service 是调用的服务的名称，后面的组成部分是固定的。



## 远程调用 Feign  

Feign的英文表意为“假装，伪装，变形”， 可以理解为将HTTP报文请求方式伪装为简单的java接口调用方式。



如ServiceA调用ServiceB的例子，我们使用Feign实现这个过程，代码如下：  

- Service B暴露"/service"服务端点，如下：

  ```java
  @SpringBootApplication
  @RestController
  public class SpringRestProviderBootstrap {
      
      public static void main(String[] args) {
          SpringApplication.run(SpringRestProviderBootstrap.class, args);
      } 
  
      @GetMapping(value = "/service") //暴露服务
      public String service(){
          return "provider invoke";
      }
  }
  ```

- Feign调用方式如下：

  1. 声明Feign客户端：

     ```java
     @FeignClient(value = "serviceB")	// value值为：被调用服务的服务名
     public interface ServiceBAgent {
         /**
         * 根据用户名查询账号信息
         * @param username 用户名
         * @return 账号信息
         */
         @GetMapping(value = "/service")
         public String service();
     }
     ```

  2. 业务调用：

     ```java
     @Autowired
     private ServiceBAgent serviceBAgent;
     
     method(){
         serviceBAgent.service();
         ...
     }
     ```

     

Feign帮我们做了哪些事：

- 在 `声明Feign客户端`(即接口) 之后，Feign会根据@FeignClient注解使用java的动态代理技术生成代理类，在这里我们指定@FeignClient value为serviceB，则说明这个类的远程目标为spring cloud的服务名称为serviceB的微服务。  
- serviceB的具体访问地址，Feign会交由ribbon获取，若该服务有多个实例地址，ribbon会采用指定的负载均衡策略选取实例。
- Feign兼容spring的web注解（如：@GetMapping），它会分析声明Feign客户端方法中的Spring注解，得出Http请求method、参数信息以及返回信息结构。  
- 当业务调用Feign客户端方法时，会调用代理类，根据以上分析结果，由代理类完成实际的参数封装、远程http请求，返回结果封装等操作。  



在Spring cloud中使用Feign：

1. 需要引入以下依赖：  

   ```xml
   <dependency>
       <groupId>org.springframework.cloud</groupId>
       <artifactId>spring‐cloud‐starter‐openfeign</artifactId>
   </dependency>
   ```

   >Feign默认集成了Ribbon，可以直接使用。

2. 在spring cloud 启动类中标注`@EnableFeignClients`，表明此项目开启Feign客户端:  

   ```java
   @SpringBootApplication
   @EnableDiscoveryClient
   @EnableFeignClients
   public class SpringRestConsumerBootstrap {
       public static void main(String[] args) {
           SpringApplication.run(SpringRestConsumerBootstrap.class, args);
       }
   }
   ```

   

## Demo

### 搭建 Nacos Server

...（略）



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



### 服务生产者

- pom.xml：

  ```xml
  <dependencies>
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
          <artifactId>spring-cloud-starter-openfeign</artifactId>
      </dependency>
  </dependencies>
  ```

- application.yml：

  ```yaml
  server:
    port: 56010
    
  spring:
    application:
      name: quickstart-provider
    cloud:
      nacos:
        discovery:
          server-addr: 127.0.0.1:8848
          
  logging:
    level:
      root: info
      org.springframework: info
  ```

  >`spring.cloud.nacos.discovery.server-addr `指定了Nacos Server的网络地址和端口号。

- Provider(生产者)服务实现 ：

  ```java
  @RestController
  public class ProviderController {
      
      private static final Logger LOG = LoggerFactory.getLogger(ProviderController.class);
      
      @GetMapping("/service")
      public String service(){
          LOG.info("provider invoke");
          return "provider invoke";
      }
  }
  ```

- 启动类：

  ```java
  @SpringBootApplication
  @EnableDiscoveryClient
  @EnableFeignClients
  public class NacosProviderApp {
      public static void main(String[] args) {
          SpringApplication.run(NacosProviderApp.class, args);
      }
  }
  ```

  >- `@EnableDiscoveryClient`：
  >
  >  表明此项目是一个注册发现客户端，这里注册服务发现使用的是Nacos  
  >
  >- `@EnableFeignClients`：开启FeignClient  



### 服务消费者

- pom.xml：

  ```java
  <dependencies>
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
          <artifactId>spring-cloud-starter-openfeign</artifactId>
      </dependency>
  </dependencies>
  ```

- application.yaml：

  ```yaml
  server:
    port: 56020
    
  spring:
    application:
      name: quickstart-consumer
    cloud:
      nacos:
        discovery:
          server-addr: 127.0.0.1:8848
  ```

- 声明Feign客户端：

  ```java
  @FeignClient(name = "quickstart‐provider")
  public interface ProviderClient {
      
      @GetMapping("/service")
      String service();
  }
  ```

- 调用生产者服务：

  ```java
  @RestController
  public class ConsumerController {
      private static final Logger LOG = LoggerFactory.getLogger(ConsumerController.class);
  
      @Autowired
      private ProviderClient providerClient;
  
      @GetMapping("/service")
      public String service(){
          LOG.info("consumer invoke");
          String providerResult = providerClient.service();
          return "consumer invoke" + "|" + providerResult;
      }
  }
  ```

- 启动类：

  ```java
  @SpringBootApplication
  @EnableDiscoveryClient
  @EnableFeignClients
  public class NacosConsumerApp {
      public static void main(String[] args) {
          SpringApplication.run(NacosConsumerApp.class, args);
      }
  }
  ```



### 测试多实例负载均衡  

1. 修改quickstart-provider工程的application.yaml，实现port动态传入：

   ```yaml
   server:
   	port: ${port:56010} #启动端口
   ```

2. 设置启动参数：

   ![image-20220618140613612](Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220618140613612.png)





# Nacos 服务管理

## 服务发现数据模型

![image-20220619203812214](Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220619203812214.png)



- 命名空间

  - 命名空间，对配置管理和服务发现都生效

- 服务：

  - 服务名

  - 实例：可通过IP:Port进行访问

  - 元信息：

    Nacos数据（如配置和服务）描述信息，如服务版本、权重、容灾策略、负载均衡策略、鉴权配置、各种自定义标签 (label)。

    从作用范围来看，分为服务级别的元信息、集群的元信息及实例的元信息。  

- 集群：

  服务实例的集合，服务实例组成一个默认集群, 集群可以被进一步按需求划分，划分的单位可以是虚拟集群，相同集群下的实例才能相互感知。  



应用通过Namespace、Service、Cluster(DEFAULT)的配置，描述了该服务向哪个环境（如开发环境）的哪个集群注册实例：

```yaml
spring:
 application:
  name: transaction-service
 cloud:
  nacos:
   discovery:
    server-addr: 127.0.0.1:7283 # 注册中心地址
    namespace: a1f8e863‐3117‐48c4‐9dd3‐e9ddc2af90a8 # 开发环境
    cluster-name: DEFAULT # 默认集群，可不填写
```

>- 集群作为实例的隔离，相同集群的实例才能相互感知  
>- namespace的默认是public命名空间
>- cluster-name的默认值为DEFAULT集群



## 服务列表管理

服务列表帮助用户以统一的视图管理其所有的微服务以及服务健康状态。整体界面布局是左上角有服务的搜索框和搜索按钮，页面中央是服务列表的展示。服务列表主要展示服务名、集群数目、实例数目、健康实例数目和详情按钮五个栏目  

![image-20220619205053762](Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220619205053762.png)



## 服务流量权重支持及流量保护

Nacos 为用户提供了流量权重控制的能力，同时开放了服务流量的阈值保护，以帮助用户更好的保护服务服务提供者集群不被意外打垮。如下图所以，可以点击实例的编辑按钮，修改实例的权重。如果想增加实例的流量，可以将权重调大，如果不想实例接收流量，则可以将权重设为0。  

![image-20220619205128150](Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220619205128150.png)



## 服务元数据管理

Nacos提供多个维度的服务元数据的暴露，帮助用户存储自定义的信息。这些信息都是以K-V的数据结构存储，在控制台上，会以k1=v1,k2=v2这样的格式展示。类似的,编辑元数据可以通过相同的格式进行。

例如服务的元数据编辑，首先点击服务详情页右上角的“编辑服务”按钮，然后在元数据输入框输入：version=1.0。  

![image-20220619205217846](Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220619205217846.png)



## 服务优雅上下线  

Nacos还提供服务实例的上下线操作，在服务详情页面，可以点击实例的“上线”或者“下线”按钮，被下线的实例，将不会包含在健康的实例列表。

> 当然，服务本身还是在运行的。

![image-20220619205445462](Nacos%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0.assets/image-20220619205445462.png)