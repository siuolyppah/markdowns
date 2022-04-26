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

- 



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

[黑马程序员Spring视频教程，全面深度讲解spring5底层原理_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1P44y1N7QG?p=8&spm_id_from=pageDriver)



