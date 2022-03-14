# Nacos进行配置管理

Nacos除了可以做注册中心，同样可以做配置管理来使用。

# 统一配置管理

当微服务部署的实例越来越多，达到数十、数百时，逐个修改微服务配置就会让人抓狂，而且很容易出错。我们需要一种统一配置管理方案，可以集中管理所有实例的配置。

![image-20210714164426792](Nacos%E8%BF%9B%E8%A1%8C%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20210714164426792.png)



Nacos一方面可以将配置集中管理，另一方可以在配置变更时，及时通知微服务，实现配置的热更新。

## 在nacos中添加配置文件

![image-20220315002933308](Nacos%E8%BF%9B%E8%A1%8C%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220315002933308.png)

![image-20220315002937902](Nacos%E8%BF%9B%E8%A1%8C%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220315002937902.png)



> 注意：项目的核心配置，需要热更新的配置才有放到nacos管理的必要。基本不会变更的一些配置还是保存在微服务本地比较好。

## 从微服务拉取配置

微服务要拉取nacos中管理的配置，并且与本地的application.yml配置合并，才能完成项目启动。

但如果尚未读取application.yml，又如何得知nacos地址呢？

因此spring引入了一种新的配置文件：==bootstrap.yaml文件==，会在application.yml之前被读取，流程如下：

![image-20220315003014311](Nacos%E8%BF%9B%E8%A1%8C%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86.assets/image-20220315003014311.png)



配置流程如下：

1. 引入nacos-config依赖

    首先，在user-service服务中，引入nacos-config的客户端依赖：

    ```xml
    <!--nacos配置管理依赖-->
    <dependency>
        <groupId>com.alibaba.cloud</groupId>
        <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
    </dependency>
    ```


2. 添加bootstrap.yaml

    在user-service中添加一个bootstrap.yaml文件，内容如下：

    ```yaml
    spring:
      application:
        name: userservice # 服务名称
      profiles:
        active: dev #开发环境，这里是dev 
      cloud:
        nacos:
          server-addr: localhost:8848 # Nacos地址
          config:
            file-extension: yaml # 文件后缀名
    ```

    在这里，将会

    1. 先根据spring.cloud.nacos.server-addr获取nacos地址
    2. 再根据`${spring.application.name}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}`，读取相应名称的远程配置文件（本例中即为userservice-dev.yaml）

3. [使用Nacos远程配置信息](#实现配置热更新)

## 实现配置热更新

我们最终的目的，是修改nacos中的配置后，微服务中无需重启即可让配置生效，也就是==**配置热更新**==。



可使用两种方式：

- 若`@Value`注解注入的属性想要热更新，还需在类上添加注解`@RefreshScope`：

    ```java
    @RestController
    @RequestMapping("/user")
    @RefreshScope
    public class UserController {
    
        @Value("${pattern.dateformat}")
        private String dateformat;
    
        @GetMapping("/now")
        public String now(){
            return LocalDateTime.now().format(DateTimeFormatter.ofPattern(dateformat));
        }
    }
    ```

- 通过`@ConfigurationProperties`注解注入的属性，自动热更新

    ```java
    @Data
    @Component
    @ConfigurationProperties(prefix = "pattern")
    public class PatternProperties {
        private String dateformat;
    }
    ```

    ```java
    @RestController
    @RequestMapping("/user")
    public class UserController {
    
        @Autowired
        PatternProperties properties;
    
        @GetMapping("/now")
        public String now(){
            return LocalDateTime.now().format(DateTimeFormatter.ofPattern(properties.getDateformat()));
        }
    }
    ```

    

## 配置共享

再微服务启动时，会去nacos读取多个配置文件，例如：

1. `[spring.application.name]-[spring.profiles.active].yaml`，例如：userservice-dev.yaml

2. `[spring.application.name].yaml`，例如：userservice.yaml

    > 由于其不包含环境，因此可以被多个环境共享。

### 配置共享的优先级

当nacos、服务本地同时出现相同属性时，优先级有高低之分：

​	application.yaml<userservice.yaml<userservice-devl.yaml

​	即==本地配置<服务名.yaml<服务名-profile.yaml==

