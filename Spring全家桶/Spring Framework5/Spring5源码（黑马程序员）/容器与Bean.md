# 容器接口

- BeanFactory接口：

  - 典型功能：getBean()
  - 控制反转、基本的依赖注入、Bean的生命周期
  - 主要的 ApplicationContext 实现都【组合】了它的功能（即成员变量）

- ApplicationContext接口：

  - 是BeanFactory的子接口。

    ApplicationContext的实现类组合并扩展了 BeanFactory 的功能

  - 拓展了BeanFactory的功能，如：

    - 国际化
    - 通配符方式获取一组Resource资源
    - 整合Environment环境（能通过它获取各种来源的配置信息）
    - 事件发布与监听，实现组件之间的解耦合



# 容器实现类

## BeanFactory

### DefaultListableBeanFactory

- 可以通过调用`registerBeanDefinition()`方法，**注册BeanDefinition对象**：

  ```java
  public class MyTest {
  
      static class OneBean {}
  
      public static void main(String[] args) {
          DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();
  
          AbstractBeanDefinition beanDefinition =
                  BeanDefinitionBuilder.genericBeanDefinition(OneBean.class).setScope("singleton").getBeanDefinition();
          beanFactory.registerBeanDefinition("oneBean", beanDefinition);
  
          OneBean bean = beanFactory.getBean(OneBean.class);
          System.out.println(bean);
      }
  }
  ```

  - 配置类、XML、组件扫描等的本质，即为生成BeanDefinition对象
  - BeanDefinition描述此Bean的创建蓝图：scope、用构造还是工厂创建、初始化销毁方法等

- BeanFactory，需要调用BeanFactory后处理器，对其进行增强：

  从而解析`@Bean`,`@ComponentScan`等，用于**补充BeanDefinition的注解**：

  ```java
  public class MyTest {
  
      static class OneBean {
      }
  
      @Configuration
      static class Config {
  
          @Bean
          public OneBean oneBean() {
              return new OneBean();
          }
      }
  
      public static void main(String[] args) {
          DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();
  
          AbstractBeanDefinition beanDefinition = BeanDefinitionBuilder.genericBeanDefinition(Config.class).setScope("singleton").getBeanDefinition();
          beanFactory.registerBeanDefinition("config", beanDefinition);
  
          Arrays.stream(beanFactory.getBeanDefinitionNames()).forEach(System.out::println);   // only: "config"
  
          // 添加后处理器（包含BeanFactory后处理器和Bean后处理器）
          AnnotationConfigUtils.registerAnnotationConfigProcessors(beanFactory);
  
          beanFactory.getBeansOfType(BeanFactoryPostProcessor.class).values().forEach(
                  beanFactoryPostProcessor -> beanFactoryPostProcessor.postProcessBeanFactory(beanFactory));
  
          Arrays.stream(beanFactory.getBeanDefinitionNames()).forEach(System.out::println);   // have "oneBean"
      }
  }
  ```

- BeanFactory，需要调用Bean后处理器，对其进行增强：

  从而对后续 bean 的创建过程提供增强

  - 例如对`@Autowired`、`@Resource`等注解的支持：

    ```java
    public class MyTest {
    
        static class BeanA {
        }
    
        static class BeanB {
            @Autowired
            private BeanA beanA;
        }
    
    
        @Configuration
        static class Config {
            @Bean
            public BeanA beanA() {
                return new BeanA();
            }
    
            @Bean
            public BeanB beanB() {
                return new BeanB();
            }
        }
    
        public static void main(String[] args) {
            DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();
    
            AbstractBeanDefinition beanDefinition = BeanDefinitionBuilder.genericBeanDefinition(Config.class).setScope("singleton").getBeanDefinition();
            beanFactory.registerBeanDefinition("config", beanDefinition);
    
            // 添加后处理器（包含BeanFactory后处理器和Bean后处理器）
            AnnotationConfigUtils.registerAnnotationConfigProcessors(beanFactory);
    
            beanFactory.getBeansOfType(BeanFactoryPostProcessor.class).values().forEach(
                    beanFactoryPostProcessor -> beanFactoryPostProcessor.postProcessBeanFactory(beanFactory)
            );  // 上述用于添加BeanDefinition
    
            beanFactory.getBeansOfType(BeanPostProcessor.class).values().forEach(
                    beanFactory::addBeanPostProcessor
            );
    
            System.out.println(beanFactory.getBean(BeanB.class).beanA); // not null
        }
    }
    ```

  - beanFactory需要手动调用方法来提前初始化单例对象（否则懒惰创建）

    ```java
    beanFactory.preInstantiateSingletons();
    ```

  - Bean后处理器的添加顺序，将影响注解的解析次序（例如同时注有@Resource和@Autowired的情况）。

  - beanFactory 需要额外设置才能解析 ${} 与 #{}



## ApplicationContext

[黑马程序员Spring视频教程，全面深度讲解spring5底层原理_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1P44y1N7QG?p=11&spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)

用作准备：

- Bean1.java：

  ```java
  public class Bean1 {
  }
  ```

- Bean2.java：

  ```java
  @Getter
  @Setter
  public class Bean2 {
  
      private Bean1 bean1;
  }
  ```

- b01.xml：

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
  
      <bean id="bean1" class="com.example.show.lecture2.Bean1"/>
      <bean id="bean2" class="com.example.show.lecture2.Bean2">
          <property name="bean1" ref="bean1"/>
      </bean>
  
  </beans>
  ```



### ClassPathXmlApplicationContext

作用：从类路径下，读取xml配置文件。



测试类：
```java
public class ClassPathXmlApplicationContextTest {

    public static void main(String[] args) {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("lecture2/b01.xml");

        Arrays.stream(context.getBeanDefinitionNames()).forEach(System.out::println);   // Bean1,Bean2

        System.out.println(context.getBean(Bean2.class).getBean1());    // com.example.show.lecture2.Bean1@58cbafc2
    }
}
```



### FileSystemXmlApplicationContext

从磁盘路径下，读取xml配置文件。



测试类：

```java
public class FileSystemXmlApplicationContextTest {

    public static void main(String[] args) {
        FileSystemXmlApplicationContext context = new FileSystemXmlApplicationContext("D:\\idea_workspace\\b01.xml");

        Arrays.stream(context.getBeanDefinitionNames()).forEach(System.out::println);   // Bean1,Bean2

        System.out.println(context.getBean(Bean2.class).getBean1());    // com.example.show.lecture2.Bean1@75d3a5e0
    }
}
```



#### 基于XML的ApplicationContenxt原理

```java
public class XMLApplicationContextPrinciple {

    public static void main(String[] args) {
        DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();

        XmlBeanDefinitionReader reader = new XmlBeanDefinitionReader(beanFactory);
        reader.loadBeanDefinitions(new ClassPathResource("lecture2/b01.xml"));  // 或FileSystemResource
        
        Arrays.stream(beanFactory.getBeanDefinitionNames()).forEach(System.out::println);   // bean1,bean2
        System.out.println(beanFactory.getBean(Bean2.class).getBean1());    // com.example.show.lecture2.Bean1@41fecb8b
    }
}
```



> 注意：
>
> 基于XML的Spring容器，需要在配置文件中添加`<context:annotation-config />`标签，从而添加Bean后处理器和Bean工厂后处理器



### AnnotationConfigApplicationContext

测试类：

```java
public class AnnotationConfigApplicationContextTest {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context =
                new AnnotationConfigApplicationContext(Config.class);

        Arrays.stream(context.getBeanDefinitionNames()).forEach(System.out::println);   // Config,Bean1,Bean2

        System.out.println(context.getBean(Bean2.class).getBean1());    // com.example.show.lecture2.Bean1@25df00a0
    }

    @Configuration
    static class Config {
        @Bean
        public Bean1 bean1() {
            return new Bean1();
        }

        @Bean
        Bean2 bean2(Bean1 bean1) {
            Bean2 bean2 = new Bean2();
            bean2.setBean1(bean1);
            return bean2;
        }
    }
}
```



### AnnotationConfigServletWebServerApplicationContext

测试类：

```java
public class AnnotationConfigServletWebServerApplicationContextTest {

    public static void main(String[] args) {
        AnnotationConfigServletWebServerApplicationContext context =
                new AnnotationConfigServletWebServerApplicationContext(WebConfig.class);
    }

    @Configuration
    static class WebConfig {
        // 相较于单纯的注解容器，需要配置一些额外信息：
        // WebServer：配置它的工厂类
        // DispatcherServlet
        // 将DispatcherServlet注册到WebServer中
        @Bean
        public ServletWebServerFactory servletWebServerFactory() {
            return new TomcatServletWebServerFactory();
        }

        @Bean
        public DispatcherServlet dispatcherServlet() {
            return new DispatcherServlet();
        }

        @Bean
        public DispatcherServletRegistrationBean registrationBean(DispatcherServlet dispatcherServlet) {
            return new DispatcherServletRegistrationBean(dispatcherServlet, "/");
        }

        @Bean("/hello") // 控制器Demo，其访问路径为/hello
        public Controller controller1() {
            return (request, response) -> {
                response.getWriter().print("hello");
                return null;
            };
        }
    }
}
```



# Bean的生命周期

[黑马程序员Spring视频教程，全面深度讲解spring5底层原理_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1P44y1N7QG?p=13&spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)