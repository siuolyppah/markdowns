# AOP的一般做法

- 将切面类，加入Spring容器



- 待增强类：

  ```java
  @Service
  public class MyService {
  
      private static final Logger log = LoggerFactory.getLogger(MyService.class);
      
      public void foo() {
          log.debug("foo()");
      }
  }
  ```

- 切面类：

  ```java
  @Aspect
  @Component
  public class MyAspect {
  
      private static final Logger log = LoggerFactory.getLogger(MyAspect.class);
  
  
      @Before("execution(* com.example.show.lecture9.service.MyService.foo())")
      public void before() {
          log.debug("before()");
      }
  }
  ```

- 测试：

  ```java
  @SpringBootApplication
  public class A09Application {
  
      public static final Logger log = LoggerFactory.getLogger(A09Application.class);
  
      public static void main(String[] args) {
          ConfigurableApplicationContext context = SpringApplication.run(A09Application.class, args);
  
          MyService myService = context.getBean(MyService.class);
          log.debug("Actual Service Class:{}", myService.getClass());
          myService.foo();
  
          context.close();
      }
  }
  ```

  

# AOP实现-ajc增强

- 使用Aspectj提供的**编译器**，提供增强。
- 能对静态方法进行增强。（动态代理方式无法做到）





- pom.xml中，**配置aspectj的编译插件**：

  ```xml
  <plugin>
      <groupId>org.codehaus.mojo</groupId>
      <artifactId>aspectj-maven-plugin</artifactId>
      <version>1.14.0</version>
      <configuration>
          <complianceLevel>1.8</complianceLevel>
          <source>8</source>
          <target>8</target>
          <showWeaveInfo>true</showWeaveInfo>
          <verbose>true</verbose>
          <Xlint>ignore</Xlint>
          <encoding>UTF-8</encoding>
      </configuration>
      <executions>
          <execution>
              <goals>
                  <!-- use this goal to weave all your main classes -->
                  <goal>compile</goal>
                  <!-- use this goal to weave all your test classes -->
                  <goal>test-compile</goal>
              </goals>
          </execution>
      </executions>
  </plugin>
  ```

- 待增强类：

  ```java
  @Service
  public class MyService {
  
      private static final Logger log = LoggerFactory.getLogger(MyService.class);
  
      public void foo() {
          log.debug("foo()");
      }
  }
  ```

- 切面类：

  ```JAVA
  @Aspect // ⬅️注意此切面并未被 Spring 管理
  public class MyAspect {
  
      private static final Logger log = LoggerFactory.getLogger(MyAspect.class);
  
      @Before("execution(* com.itheima.service.MyService.foo())")
      public void before() {
          log.debug("before()");
      }
  }
  ```

- 测试类：

  ```JAVA
  public class A09 {
  
      public static void main(String[] args) {
  
          new MyService().foo();
      }
  }
  ```

  

**编译后的结果**：

- 待增强类：

  ```java
  @Service
  public class MyService {
      private static final Logger log = LoggerFactory.getLogger(MyService.class);
  
      public MyService() {
      }
  
      public static void foo() {
          MyAspect.aspectOf().before();
          log.debug("foo()");
      }
  }
  ```

- 切面类：

  ```java
  @Aspect
  public class MyAspect {
      private static final Logger log = LoggerFactory.getLogger(MyAspect.class);
  
      static {
          try {
              ajc$postClinit();
          } catch (Throwable var1) {
              ajc$initFailureCause = var1;
          }
  
      }
  
      public MyAspect() {
      }
  
      @Before("execution(* com.itheima.service.MyService.foo())")
      public void before() {
          log.debug("before()");
      }
  
      public static MyAspect aspectOf() {
          if (ajc$perSingletonInstance == null) {
              throw new NoAspectBoundException("com.itheima.aop.MyAspect", ajc$initFailureCause);
          } else {
              return ajc$perSingletonInstance;
          }
      }
  
      public static boolean hasAspect() {
          return ajc$perSingletonInstance != null;
      }
  }
  ```



# AOP实现-agent类加载

- 添加JVM参数，在类加载时，修改class字节码
- 使用动态代理方式时，无法对 方法内部被调用的方法 进行增强
- 观察效果演示：使用的阿里的**arthas**工具（[视频演示](https://www.bilibili.com/video/BV1P44y1N7QG?p=33&t=316.5)）





- 待增强类：

  ```java
  @Service
  public class MyService {
  
      private static final Logger log = LoggerFactory.getLogger(MyService.class);
  
      final public void foo() {
          log.debug("foo()");
          this.bar();
      }
  
      public void bar() {
          log.debug("bar()");
      }
  }
  ```

- 切面类：

  ```java
  @Aspect // ⬅️注意此切面并未被 Spring 管理
  public class MyAspect {
  
      private static final Logger log = LoggerFactory.getLogger(MyAspect.class);
  
      @Before("execution(* com.itheima.service.MyService.*())")
      public void before() {
          log.debug("before()");
      }
  }
  ```

- 测试类：

  ```java
  /*
      注意几点
      1. 版本选择了 java 8, 因为目前的 aspectj-maven-plugin 1.14.0 最高只支持到 java 16
      2. 运行时需要在 VM options 里加入 -javaagent:C:/Users/manyh/.m2/repository/org/aspectj/aspectjweaver/1.9.7/aspectjweaver-1.9.7.jar
          把其中 C:/Users/manyh/.m2/repository 改为你自己 maven 仓库起始地址
   */
  @SpringBootApplication
  public class A10 {
  
      private static final Logger log = LoggerFactory.getLogger(A10.class);
  
      public static void main(String[] args) {
          ConfigurableApplicationContext context = SpringApplication.run(A10.class, args);
          MyService service = context.getBean(MyService.class);
  
          log.debug("service class: {}", service.getClass());
          service.foo();
  
  //        context.close();
      }
  }
  ```



# AOP实现-proxy-JDK

- 限制：**只能针对接口**进行代理



## Demo

```java
public class JdkProxyDemo {

    interface Foo {
        void foo();
    }

    static class Target implements Foo {

        @Override
        public void foo() {
            System.out.println("target Foo()");
        }
    }

    public static void main(String[] args) {
        Target target = new Target();

        ClassLoader classLoader = JdkProxyDemo.class.getClassLoader();
        Foo proxyObj = (Foo) Proxy.newProxyInstance(classLoader, new Class[]{Foo.class}, (proxy, method, args1) -> {
            System.out.println("before");
            Object result = method.invoke(target, args1);
            System.out.println("after");

            return result;
        });

        proxyObj.foo();
    }
}
```



## jdk代理原理（模拟）

- TestJdkProxy.java：

  ```java
  public class TestJdkProxy {
  
      interface Foo {
          void foo();
  
          int bar();
      }
  
      static class Target implements Foo {
  
          @Override
          public void foo() {
              System.out.println("target foo()");
          }
  
          @Override
          public int bar() {
              System.out.println("target bar()");
              return 100;
          }
      }
  
      interface InvocationHandler {
          Object invoke(Object proxy, Method method, Object[] args) throws InvocationTargetException, IllegalAccessException;
      }
  
      public static void main(String[] args) {
          Foo proxyObj = new $Proxy0((proxy, method, args1) -> {
              System.out.println("before");
              Object invoke = method.invoke(new Target(), args1);
              System.out.println("after");
  
              return invoke;
          });
  
          proxyObj.foo();
          proxyObj.bar();
      }
  }
  ```

- $Proxy0.java：

  ```java
  public class $Proxy0 implements TestJdkProxy.Foo {
  
      private TestJdkProxy.InvocationHandler invocationHandler;
  
      private static final Method foo;
      private static final Method bar;
  
      static {
          try {
              foo = TestJdkProxy.Foo.class.getMethod("foo");
              bar = TestJdkProxy.Foo.class.getMethod("bar");
          } catch (NoSuchMethodException e) {
              throw new NoSuchMethodError(e.getMessage());
          }
      }
  
      public $Proxy0(TestJdkProxy.InvocationHandler invocationHandler) {
          this.invocationHandler = invocationHandler;
      }
  
      @Override
      public void foo() {
          try {
              invocationHandler.invoke(this, foo, new Object[0]);
          } catch (RuntimeException | Error e) {
              throw new RuntimeException(e);
          } catch (Throwable e) {
              throw new UndeclaredThrowableException(e);
          }
      }
  
      @Override
      public int bar() {
          try {
              Object invoke = invocationHandler.invoke(this, bar, new Object[0]);
  
              return (int) invoke;
          } catch (RuntimeException | Error e) {
              throw new RuntimeException(e);
          } catch (Throwable e) {
              throw new UndeclaredThrowableException(e);
          }
      }
  }
  ```

  

## JDK如何实现

- [视频演示](https://www.bilibili.com/video/BV1P44y1N7QG?p=38)

- 代理类，是利用ASM生成的

  > ASM用于：在运行时期，动态生成字节码（不经过源码编译为字节码的阶段）



## JDK反射的优化

- 第1次~第16次：实现类调用Native方法

- 17次及以后：生成代理类，直接通过Java代码方式调用。

  > 一个方法，生成一个代理类。



# AOP实现-proxy-cglib

- 生成的代理类，是目标类的**子类**

  > 这意味着：
  >
  > - 不能对final的类进行代理
  > - 不能被声明为final的方法进行代理增强



## Demo

```java
public class CglibProxyDemo {

    static class Target {
        public void foo() {
            System.out.println("target foo()");
        }
    }

    public static void main(String[] args) {
        Target target = new Target();

        Target proxyObj = (Target) Enhancer.create(Target.class, (MethodInterceptor) (proxy, method, args1, methodProxy) -> {
            System.out.println("before()");
            Object result = method.invoke(target, args1);		// 使用反射
//            Object result = methodProxy.invoke(target, args1);		// 内部没有使用反射，需要目标对象
//            Object result = methodProxy.invokeSuper(proxy, args1);	// 内部没有使用反射

            System.out.println("after()");

            return result;
        });

        proxyObj.foo();
    }
}
```

> 特点：可利用MethodProxy对象，避免使用Method对象反射调用目标



## cglib代理原理

[黑马程序员Spring视频教程，全面深度讲解spring5底层原理_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1P44y1N7QG?p=41&vd_source=be746efb77e979ca275e4f65f2d8cda3)