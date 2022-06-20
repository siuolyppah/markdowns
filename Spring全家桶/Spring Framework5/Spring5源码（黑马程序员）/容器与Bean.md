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