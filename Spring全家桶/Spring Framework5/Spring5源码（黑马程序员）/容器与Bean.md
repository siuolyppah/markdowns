# 容器接口及其功能

Spring中比较重要的容器接口：

- BeanFactory接口
- ApplicationContext接口





事实上，SpringBoot启动类中的run方法，是有返回值的：

```java
@SpringBootApplication
public class ShowApplication {

    public static void main(String[] args) {
        ConfigurableApplicationContext run = SpringApplication.run(ShowApplication.class, args);
    }
}
```

其为ConfigurableApplicationContext类型

![image-20220426212158335](%E5%AE%B9%E5%99%A8%E4%B8%8EBean.assets/image-20220426212158335.png)

> IDEA查看类图快捷键：Ctrl+Alt+U



可见，***ApplicationContext接口，间接继承了BeanFactory接口***



## BeanFactory接口

### BeanFactoy与其他接口的关系

BeanFactory 接口：

- 它为 ApplicaitonContext 接口的父接口

- 它才是 Spring 的核心容器。主要的各种 ApplicationContext 实现，都“组合”了它的功能。

  > 也就是说，***BeanFactory是ApplicationContext的一个成员变量***。

  



### BeanFactory接口的功能

> Ctrl+F12：查看此类的所有方法

- 在表面上，只有getBean()方法，获取对应的Bean实例

- 但实际上，***控制反转、基本的依赖注入、 Bean生命周期的各种功能，都由它的实现类提供***

  > 实现类：DefaultListableBeanFactory。最主要的，该类继承了DefaultSingletonBeanRegistry类
  >
  > ```java
  > @SpringBootApplication
  > public class ShowApplication {
  > 
  >     public static void main(String[] args) throws NoSuchFieldException, IllegalAccessException {
  >         ConfigurableApplicationContext context = SpringApplication.run(ShowApplication.class, args);
  > 
  >         Field singletonObjects = DefaultSingletonBeanRegistry.class.getDeclaredField("singletonObjects");
  >         singletonObjects.setAccessible(true);
  > 
  >         ConfigurableListableBeanFactory beanFactory = context.getBeanFactory();
  >         Map<String, Object> objectMap = (Map<String, Object>) singletonObjects.get(beanFactory);
  >         objectMap.forEach((k, v) -> {
  >             System.out.println(k + "=" + v);
  >         });
  > 
  >     }
  > }
  > ```



## ApplicationContext接口

ApplicationContext接口，是BeanFactory接口的子接口



### 是对BeanFactory接口的拓展

![image-20220426215607812](%E5%AE%B9%E5%99%A8%E4%B8%8EBean.assets/image-20220426215607812.png)

主要拓展了以下功能（接口）：

- MessageSource接口：处理国际化资源
- ResourcePatternResolver接口：通配符匹配资源
- ApplicationEventPublisher接口：发布事件
- EnvironmentCapable接口：处理SpringBoot中的环境信息



#### 国际化功能

```java
public class TestMessageSource {
    public static void main(String[] args) {
        GenericApplicationContext context = new GenericApplicationContext();

        context.registerBean("messageSource", MessageSource.class, () -> {
            ResourceBundleMessageSource ms = new ResourceBundleMessageSource();
            ms.setDefaultEncoding("utf-8");
            ms.setBasename("messages");
            return ms;
        });

        context.refresh();

        System.out.println(context.getMessage("hi", null, Locale.ENGLISH));
        System.out.println(context.getMessage("hi", null, Locale.CHINESE));
        System.out.println(context.getMessage("hi", null, Locale.JAPANESE));
    }
}
```

国际化文件均在 src/resources 目录下：

- messages.properties（空）

- messages_en.properties

  ```properties
  hi=Hello
  ```

- messages_ja.properties

  ```pro
  hi=こんにちは

- messages_zh.properties

  ```properties
  hi=你好
  ```

>***注意***：
>
>* ApplicationContext 中 MessageSource bean 的名字固定为 messageSource
>* 使用 SpringBoot 时，国际化文件名固定为 messages
>* 空的 messages.properties 也必须存在



#### 通配符方式获取资源

```java
@SpringBootApplication
public class ShowApplication {

    public static void main(String[] args) throws IOException {
        ConfigurableApplicationContext context = SpringApplication.run(ShowApplication.class, args);

        Resource[] resources = context.getResources("classpath:application.properties");
        for (Resource resource : resources) {
            System.out.println(resource);
        }
    }
}
```



#### 获取配置信息

```java
@SpringBootApplication
public class ShowApplication {

    public static void main(String[] args) throws IOException {
        ConfigurableApplicationContext context = SpringApplication.run(ShowApplication.class, args);

        Map<String, Object> systemEnvironment = context.getEnvironment().getSystemEnvironment();
        System.out.println(systemEnvironment);
        System.out.println(context.getEnvironment().getProperty("JAVA_HOME"));
        //下面这个配置在application.properties文件中
        System.out.println(context.getEnvironment().getProperty("server.port"));
    }
}
```



#### 发布事件

> 事件，用于解耦

1. 定义事件对象

   ```java
   public class UserRegisteredEvent extends ApplicationEvent {
   
       //source为事件的发布源
       public UserRegisteredEvent(Object source) {
           super(source);
       }
   }
   ```

2. 发布事件

   ```java
   @SpringBootApplication
   public class ShowApplication {
   
       public static void main(String[] args) throws IOException {
           ConfigurableApplicationContext context = SpringApplication.run(ShowApplication.class, args);
   
           //发布事件
           context.publishEvent(new UserRegisteredEvent(context));
       }
   }
   ```

3. 接收事件

   ```java
   @Component
   public class ListenerComponent {
   
       @EventListener  //没有返回值，方法名任意，形参用于接收事件对象
       public void receive(UserRegisteredEvent event){
           System.out.println(event);
       }
   }
   ```

   

## 小结

1. ApplicationContext接口，并非只是简单继承自BeanFactory接口。而是组合并拓展了BeanFactory接口的功能。

2. 代码解耦的一种方式：ApplicationEventPublisher接口的publishEvent()方法

   即事件发布

   >事件发布还可以异步，查阅 @EnableAsync，@Async 的用法



# 容器的实现类

## BeanFactory的实现类

### DefaultListableBeanFactory类

> 总的来说，BeanFactory功能并不丰富；
>
> 其拓展功能，主要由后处理器提供。



#### 如何将一个类交给容器管理

1. 创建该类的 Definition 对象
2. 将该 Definition 对象，注册到 BeanFactory 中

> Definition 对象描述如下信息：
>
> - scope 是什么
> - 用构造还是工厂创建
> - 初始化方法
> - 销毁方法
> - ...

> ### 💡 注意
>
> 原始的BeanFactory，并没有解析注解的能力。（@Configuration，@Bean等）

```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.support.AbstractBeanDefinition;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

public class TestBeanFactory {

    public static void main(String[] args) {
        DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();

        // 需要添加 Bean的定义：
        // 1. class
        // 2. scope：单例/多例
        // 3. 初始化方法
        // 4. 销毁方法


        // 创建Config类的 Definition
        AbstractBeanDefinition configDefinition = BeanDefinitionBuilder
                .genericBeanDefinition(Config.class).setScope("singleton").getBeanDefinition();
        beanFactory.registerBeanDefinition("config", configDefinition);

        for (String name : beanFactory.getBeanDefinitionNames()) {
            System.out.println(name);   //only：config1。也即@Configuration和@Bean注解未被解析
        }
    }


    @Configuration
    static class Config {

        @Bean
        public Bean1 bean1() {return new Bean1();}

        @Bean
        public Bean2 bean2() {return new Bean2();}
    }

    static class Bean1 {
        private static final Logger log = LoggerFactory.getLogger(Bean1.class);

        @Autowired
        private Bean2 bean2;

        public Bean1() {log.debug("构造 Bean1()");}

        public Bean2 getBean2() {return bean2;}
    }

    static class Bean2 {
        private static final Logger log = LoggerFactory.getLogger(Bean2.class);

        public Bean2() {log.debug("构造 Bean2()");}
    }
}
```



#### 如何解析注解(@Configuration 和 @Bean)

> ***主要由<u>Bean工厂后处理器</u>，提供对Bean Definition的补充***
>
> 例如通过解析 @Bean、@ComponentScan 等注解，来补充一些 bean definition



1. 将配置类（标记@Configuration）交给BeanFactory管理

   1. 创建配置类的 Definition 对象
   2. 将该 Definition 对象，注册到 BeanFactory 中

2. 为BeanFactory对象，注册***注解Bean工厂后处理器***(即：把处理器注册到容器)

   > ```java
   > AnnotationConfigUtils.registerAnnotationConfigProcessors(beanFactory);
   > ```
   >
   > 将添加如下5个处理器：
   >
   > - Bean工厂后处理器
   >
   >   - org.springframework.context.annotation.internalConfigurationAnnotationProcessor：用于处理@Configuration和@Bean
   >   - org.springframework.context.event.internalEventListenerProcessor
   >
   > - Bean后处理器：
   >
   >   - org.springframework.context.annotation.internalAutowiredAnnotationProcessor：@Autowired和@Value
   >
   >   - org.springframework.context.annotation.internalCommonAnnotationProcessor：@Resource
   >
   > - org.springframework.context.event.internalEventListenerFactory

3. 将注解配置处理器，应用于BeanFactory对象



```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.support.AbstractBeanDefinition;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.context.annotation.AnnotationConfigUtils;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

public class TestBeanFactory {

    public static void main(String[] args) {
        DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();
        
        // 创建Config类的 Definition
        AbstractBeanDefinition configDefinition = BeanDefinitionBuilder
                .genericBeanDefinition(Config.class).setScope("singleton").getBeanDefinition();
        beanFactory.registerBeanDefinition("config", configDefinition);

        for (String name : beanFactory.getBeanDefinitionNames()) {
            System.out.println(name);   //only：config1。也即@Configuration和@Bean注解未被解析
        }

        // 将AnnotationConfigProcessors添加到容器
        AnnotationConfigUtils.registerAnnotationConfigProcessors(beanFactory);
        
        // 获取BeanFactoryPostProcessor,并执行它们
        beanFactory.getBeansOfType(BeanFactoryPostProcessor.class).values().forEach(beanFactoryPostProcessor -> {
             beanFactoryPostProcessor.postProcessBeanFactory(beanFactory);
        });

        System.out.println();
        for (String name : beanFactory.getBeanDefinitionNames()) {
            System.out.println(name);
        }
    }
    
    //Config、Bean1、Bean2定义同上
}
```



#### 如何注入属性

> 主要由***<u>Bean后处理器</u>***，对后续 bean 的生命周期（创建、依赖注入、初始化）提供增强
>
> * 例如 @Autowired，@Resource 等注解的解析都是 bean 后处理器完成的
> * bean 后处理的添加顺序会对解析结果有影响，见视频中同时加 @Autowired，@Resource 的例子

 

```java
public static void main(String[] args) {
    DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();

    // 创建Config类的 Definition
    AbstractBeanDefinition configDefinition = BeanDefinitionBuilder
        .genericBeanDefinition(Config.class).setScope("singleton").getBeanDefinition();
    beanFactory.registerBeanDefinition("config", configDefinition);
    // 到目前为止，容器仅有config类

    // 将 AnnotationConfigProcessors 添加到容器
    AnnotationConfigUtils.registerAnnotationConfigProcessors(beanFactory);
    // 到目前为止，容器有：config类 和 五个AnnotationConfigProcessors

    // 获取BeanFactoryPostProcessor,并执行它们
    beanFactory.getBeansOfType(BeanFactoryPostProcessor.class).values().forEach(beanFactoryPostProcessor -> {
        beanFactoryPostProcessor.postProcessBeanFactory(beanFactory);
    });
    // 到目前为止，容器新增：Bean1 和 Bean2。 但未提供属性注入（@Autowired）

    // 执行Bean后处理器：针对 Bean 的生命周期各个阶段提供增强
    beanFactory.getBeansOfType(BeanPostProcessor.class).values().forEach((beanPostProcessor) -> {
        beanFactory.addBeanPostProcessor(beanPostProcessor);
    });

    // 默认情况下：在第一次使用Bean对象之前，BeanFactory中仅保留着描述信息
    // 预实例化单例对象
    beanFactory.preInstantiateSingletons();

    System.out.println("------------------------");
    Bean1 bean1 = beanFactory.getBean(Bean1.class);
    System.out.println(bean1.getBean2());       //not null
}

//Config、Bean1、Bean2定义同上
```





### 小结

- BeanFactory 不会主动做的事：

  1. 不会主动调用 BeanFactory后处理器
  2. 不会主动添加 Bean处理器
  3. 不会主动初始化单例对象（即默认延迟加载）
  4. 不会解析 ${} 和 #{}

  > 上述功能，由BeanFactory后处理器和Bean后处理器提供

- Bean后处理器，存在[排序的逻辑](https://www.bilibili.com/video/BV1P44y1N7QG?p=10&spm_id_from=pageDriver)【4:35】：

  先注册的后处理器，优先级更高



[BeanFactory实现类.xmind](C:\Users\G_xy\Documents\BeanFactory实现类.xmind) 

![BeanFactory实现类](%E5%AE%B9%E5%99%A8%E4%B8%8EBean.assets/BeanFactory%E5%AE%9E%E7%8E%B0%E7%B1%BB-16510771347431.png)



## ApplicationContext的实现类

[黑马程序员Spring视频教程，全面深度讲解spring5底层原理_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1P44y1N7QG?p=11&spm_id_from=pageDriver)
