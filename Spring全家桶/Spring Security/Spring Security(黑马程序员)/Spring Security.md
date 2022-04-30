# 相关资料

- [视频链接](https://www.bilibili.com/video/BV1VE411h7aL?spm_id_from=333.999.0.0)



# 基本概念

## 什么是认证

- 认证是为了保护系统的隐私数据与资源，用户的身份合法方可访问该系统的资源。
- ==认证==：用户认证就是判断一个用户的身份是否合法的过程，用户去访问系统资源时系统要求验证用户的身份信息，身份合法方可继续访问，不合法则拒绝访问。
- 常见的用户身份认证方式有：
  - 用户名密码登录；
  - 二维码登录；
  - 手机短信登录；



## 什么是会话

- 用户认证通过后，为了避免用户的每次操作都需要进行认证，可将用户的信息保证在会话中。

- ***会话就是系统为了保持当前用户的登录状态所提供的机制***，常见的有基于session方式、基于token方式等。

- 基于session的认证方式如下图：

  ![image-20220429220459970](Spring%20Security.assets/image-20220429220459970.png)

  它的交互流程为：

  1. 用户认证成功后，在服务端生成用户相关的数据保存在session(当前会话)中
  2. 发给客户端的 sesssion_id <u>存放到 cookie 中</u>
  3. 客户端请求时带上 session_id 就可以验证服务器端是否存在 session 数据，以此完成用户的合法校验
  4. 当用户退出系统或session过期销毁时,客户端的session_id也就无效了

- 基于token方式如下图：

  ![image-20220429220638756](Spring%20Security.assets/image-20220429220638756.png)

  它的交互流程为：

  1. 用户认证成功后，服务端生成一个token发给客户端
  2. 客户端<u>可以放到 cookie 或 localStorage 等存储中</u>，每次请求时带上 token
  3. 服务端收到token通过验证后即可确认用户身份



- 基于session的认证方式由Servlet规范定制，服务端要存储session信息需要占用内存资源，客户端需要支持 cookie；

- 基于token的方式则一般不需要服务端存储token，并且不限制客户端的存储方式。

  如今移动互联网时代，更多类型的客户端需要接入系统，系统多是采用前后端分离的架构进行实现，所以基于token的方式更适合。



## 什么是授权

- 认证是为了<u>保证用户身份的合法性</u>，授权则是为了<u>更细粒度的对隐私数据进行划分</u>，授权是在认证通过后发生的， 控制不同的用户能够访问不同的资源

- 授权： 授权是用户认证通过后，根据用户的权限来***控制用户访问资源***的过程，拥有资源的访问权限则正常访问，没有 权限则拒绝访问。

  

拿微信来举例子，微信登录成功后用户即可使用微信的功能，比如，发红包、发朋友圈、添加好友等，没有绑定 银行卡的用户是无法发送红包的，绑定银行卡的用户才可以发红包，发红包功能、发朋友圈功能都是微信的资源即 功能资源，用户拥有发红包功能的权限才可以正常使用发送红包功能，拥有发朋友圈功能的权限才可以使用发朋友 圈功能，这个根据用户的权限来控制用户使用资源的过程就是授权。



## 授权的数据模型

如何进行授权即如何对用户访问资源进行控制，首先需要学习授权相关的数据模型。



授权可简单理解为Who对What(which)进行How操作，包括如下：

- Who，即主体（Subject），主体一般是指用户，也可以是程序，需要访问系统中的资源。
- What，即资源 （Resource）。如系统菜单、页面、按钮、代码方法、系统商品信息、系统订单信息等。
  - 系统菜单、页面、按钮、代码方法都属于<u>**系统功能资源**</u>，对于web系统每个功能资源通常<u>**对应一个URL**</u>；
  - 系统商品信息、系统订单信息都属于<u>**实体资源（数据资源）**</u>，实体资源由<u>资源类型</u>和<u>资源实例</u>组成，比如商品信息为资源类型，商品编号 为001 的商品为资源实例。
- How，权限/许可（Permission），规定了用户对资源的操作许可，权限离开资源没有意义， 如用户查询权限、用户添加权限、某个代码方法的调用权限、编号为001的用户的修改权限等，通过权限可知用户对哪些资源都有哪些操作许可。



主体、资源、权限关系如下图：

![image-20220429221838234](Spring%20Security.assets/image-20220429221838234.png)



------

***主体、资源、权限相关的数据模型如下：***

- 主体（用户id、账号、密码、...）
- 资源（资源id、资源名称、访问地址、...）
- 权限（权限id、权限标识、权限名称、资源id、...）

- 角色（角色id、角色名称、...）

- 角色和权限关系（角色id、权限id、...）

  >角色是对多个权限的打包

- 主体（用户）和角色关系（用户id、角色id、...）

主体（用户）、资源、权限关系如下图：

![image-20220429221923773](Spring%20Security.assets/image-20220429221923773.png)



------

但通常企业开发中，***将资源和权限表合并为一张权限表***，如下：

- 资源（资源id、资源名称、访问地址、...）

- 权限（权限id、权限标识、权限名称、资源id、...）

合并为：

权限（权限id、权限标识、权限名称、<u>资源名称、资源访问地址</u>、...）



修改后数据模型之间的关系如下图：

![image-20220429221959206](Spring%20Security.assets/image-20220429221959206.png)



## RBAC

如何实现授权？业界通常基于RBAC实现授权。

### 基于角色的访问控制

RBAC基于角色的访问控制（Role-Based Access Control）是按角色进行授权，比如：

- 主体的角色为总经理可以查询企业运营报表，查询员工工资信息等，访问控制流程如下：

  ![image-20220429223957843](Spring%20Security.assets/image-20220429223957843.png)

  根据上图中的判断逻辑，授权代码可表示如下：

  ```java
  if(主体.hasRole("总经理角色id")){
  	查询工资
  }
  ```

- 如果上图中查询工资所需要的角色<u>变化为总经理和部门经理</u>，此时就需要修改判断逻辑为“判断用户的角色是否是总经理或部门经理”,修改代码如下：

  ```java
  if(主体.hasRole("总经理角色id") || 主体.hasRole("部门经理角色id")){
  	查询工资
  }
  ```

根据上边的例子发现，当需要修改角色的权限时就需要修改授权的相关代码，**<u>*系统可扩展性差*</u>**。



### 基于资源的访问控制

RBAC基于资源的访问控制（Resource-Based Access Control）是按资源（或权限）进行授权权，比如：

- 用户必须 具有查询工资权限才可以查询员工工资信息等，访问控制流程如下：

  ![image-20220429224234734](Spring%20Security.assets/image-20220429224234734.png)

  ```java
  if(主体.hasPermission("查询工资权限标识")){
      查询工资
  }
  ```

优点：系统设计时定义好查询工资的权限标识，即使查询工资所需要的角色变化为总经理和部门经理也不需要修改授权代码，**<u>*系统可扩展性强*</u>**。





# 基于Session的认证方式

## 认证流程

流程如下：

1. 用户发起请求
2. 在服务端生成用户相关的数据保存在session(当前会话)，并将SessionID发给以Cookie方式发给客户端
3. 用客户端请求时带上 session_id，服务器端验证是否存在对应的 session 数据

![image-20220429231032999](Spring%20Security.assets/image-20220429231032999.png)



基于Session的认证机制由Servlet规范定制，Servlet容器已实现，用户通过HttpSession的操作方法即可实现，如下是HttpSession相关的操作API：

|                    方法                     |          作用           |
| :-----------------------------------------: | :---------------------: |
|   HttpSession getSession(Boolean create)    | 获取当前HttpSession对象 |
| void setAttribute(String name,Object value) |   向session中存放对象   |
|      object getAttribute(String name)       |   从session中获取对象   |
|      void removeAttribute(String name)      |    移除session中对象    |
|              void invalidate()              |    使HttpSession失效    |
|                     ...                     |                         |



## 创建Demo工程

### 创建Maven工程

基于Maven构建，基于：

- SpringMVC
- Servlet3.0



创建maven工程 security-springmvc，工程结构如下：

![image-20220429231640946](Spring%20Security.assets/image-20220429231640946.png)



依赖：

- 由于是web工程，packaging设置为war
- 使用tomcat7-maven-plugin插件来运行工程

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.itheima.security</groupId>
    <artifactId>security-springmvc</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>war</packaging>

    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.compiler.source>11</maven.compiler.source>
        <maven.compiler.target>11</maven.compiler.target>
    </properties>

    <dependencies>
        <!-- https://mvnrepository.com/artifact/org.springframework/spring-webmvc -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>5.1.5.RELEASE</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/javax.servlet/javax.servlet-api -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.1.0</version>
            <scope>provided</scope>
        </dependency>

        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>1.18.8</version>
        </dependency>
    </dependencies>
    <build>
        <finalName>security-springmvc</finalName>
        <pluginManagement>
            <plugins>
                <plugin>
                    <artifactId>maven-resources-plugin</artifactId>
                    <configuration>
                        <encoding>utf-8</encoding>
                        <useDefaultDelimiters>true</useDefaultDelimiters>
                        <resources>
                            <resource>
                                <directory>src/main/resources</directory>
                                <filtering>true</filtering>
                                <includes>
                                    <include>**/*</include>
                                </includes>
                            </resource>
                            <resource>
                                <directory>src/main/java</directory>
                                <includes>
                                    <include>**/*.xml</include>
                                </includes>
                            </resource>
                        </resources>
                    </configuration>
                </plugin>
            </plugins>
        </pluginManagement>
    </build>
</project>
```



### 配置Spring容器

在config包下定义ApplicationConfig.java，它对应web.xml中ContextLoaderListener的配置

```java
package org.example.security.springmvc.config;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;
import org.springframework.stereotype.Controller;

@Configuration
@ComponentScan(basePackages = "org.example.security.springmvc",
        excludeFilters = {@ComponentScan.Filter(type = FilterType.ANNOTATION, value = Controller.class)})
// 相当于applicationContext.xml
public class ApplicationConfig {
    //在此配置除了Controller的其它bean，比如：数据库链接池、事务管理器、业务bean等。

}
```



### 配置servletContext

```java
package org.example.security.springmvc.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;
import org.springframework.stereotype.Controller;
import org.springframework.web.servlet.config.annotation.EnableWebMvc;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
import org.springframework.web.servlet.view.InternalResourceViewResolver;

@Configuration
@EnableWebMvc
@ComponentScan(basePackages = "org.example.security.springmvc", includeFilters = {@ComponentScan.Filter(type = FilterType.ANNOTATION, value = Controller.class)})
// 此类相当于springmvc.xml文件
public class WebConfig implements WebMvcConfigurer {

    // 视图解析器
    @Bean
    public InternalResourceViewResolver viewResolver() {
        InternalResourceViewResolver viewResolver = new InternalResourceViewResolver();
        viewResolver.setPrefix("/WEB-INF/view/");
        viewResolver.setSuffix(".jsp");
        return viewResolver;
    }
}
```



### 加载 Spring容器

在init包下定义Spring容器初始化类SpringApplicationInitializer，此类实现WebApplicationInitializer接口， ***Spring容器启动时加载WebApplicationInitializer接口的所有实现类。***

```java
package org.example.security.springmvc.init;

import org.example.security.springmvc.config.ApplicationConfig;
import org.example.security.springmvc.config.WebConfig;
import org.springframework.web.servlet.support.AbstractAnnotationConfigDispatcherServletInitializer;

public class SpringApplicationInitializer extends AbstractAnnotationConfigDispatcherServletInitializer {

    // Spring容器，相当于加载applicationContext.xml
    @Override
    protected Class<?>[] getRootConfigClasses() {
        return new Class[]{ApplicationConfig.class};
    }

    // ServletContext，相当于加载springmvc.xml
    @Override
    protected Class<?>[] getServletConfigClasses() {
        return new Class[]{WebConfig.class};
    }

    // url-mapping
    @Override
    protected String[] getServletMappings() {
        return new String[]{"/"};
    }
}
```



SpringApplicationInitializer相当于web.xml，使用了servlet3.0开发则不需要再定义web.xml， ApplicationConfig.class对应以下配置的application-context.xml，WebConfig.class对应以下配置的springmvc.xml，web.xml的内容参考：

```xml
<web-app>
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>/WEB-INF/application-context.xml</param-value>
    </context-param>
    <servlet>
        <servlet-name>springmvc</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>/WEB-INF/spring-mvc.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>springmvc</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>
</web-app>
```



## 实现认证功能

### 认证页面

在webapp/WEB-INF/views下定义认证页面login.jsp，本案例只是测试认证流程，页面没有添加css样式，页面实 现可填入用户名，密码，触发登录将提交表单信息至/login，内容如下：

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>用户登录</title>
</head>
<body>
<form action="login" method="post">
    用户名：<input type="text" name="username"><br>
    密&nbsp;&nbsp;&nbsp;码:
    <input type="password" name="password"><br>
    <input type="submit" value="登录">
</form>
</body>
</html>
```



在WebConfig中新增如下配置，将/直接导向login.jsp页面：

```java
@Override
public void addViewControllers(ViewControllerRegistry registry) {
    registry.addViewController("/").setViewName("login");
}
```



### 认证接口

用户进入认证页面，输入账号和密码，点击登录，请求/login进行身份认证。

1. 定义认证接口，此接口用于对传来的用户名、密码校验，若成功则返回该用户的详细信息，否则抛出错误异常：

   ```java
   /**
   * 认证服务
   */
   public interface AuthenticationService {
       /**
   * 用户认证
   * @param authenticationRequest 用户认证请求
   * @return 认证成功的用户信息
   */
       UserDto authentication(AuthenticationRequest authenticationRequest);
   }
   
   ```

2. AuthenticationRequest类如下：

   ```java
   @Data
   public class AuthenticationRequest {
       /**
   * 用户名
   */
       private String username;
       /**
   * 密码
   */
       private String password;
   }
   ```

3. 认证成功后返回的用户详细信息，也就是当前登录用户的信息：

   ```java
   /**
   * 当前登录用户信息
   */
   @Data
   @AllArgsConstructor
   public class UserDto {
       private String id;
       private String username;
       private String password;
       private String fullname;
       private String mobile;
   }
   ```

4. 认证实现类：根据用户名查找用户信息，并校验密码，这里模拟了两个用户：

   ```java
   @Service
   public class AuthenticationServiceImpl implements AuthenticationService{
       @Override
       public UserDto authentication(AuthenticationRequest authenticationRequest) {
           if(authenticationRequest == null
              || StringUtils.isEmpty(authenticationRequest.getUsername())
              || StringUtils.isEmpty(authenticationRequest.getPassword())){
               throw new RuntimeException("账号或密码为空");
           }
           UserDto userDto = getUserDto(authenticationRequest.getUsername());
           if(userDto == null){
               throw new RuntimeException("查询不到该用户");
           }
           if(!authenticationRequest.getPassword().equals(userDto.getPassword())){
               throw new RuntimeException("账号或密码错误");
           }
           return userDto;
       }
       //模拟用户查询
       public UserDto getUserDto(String username){
           return userMap.get(username);
       }
       //用户信息
       private Map<String,UserDto> userMap = new HashMap<>();
       {
           userMap.put("zhangsan",new UserDto("1010","zhangsan","123","张三","133443"));
           userMap.put("lisi",new UserDto("1011","lisi","456","李四","144553"));
       }
   }
   ```

5. 登录Controller，对/login请求处理，它调用AuthenticationService完成认证并返回登录结果提示信息：

   ```java
   @RestController
   public class LoginController {
       @Autowired
       private AuthenticationService authenticationService;
       /**
   * 用户登录
   * @param authenticationRequest 登录请求
   * @return
   */
       @PostMapping(value = "/login",produces = {"text/plain;charset=UTF‐8"})
       public String login(AuthenticationRequest authenticationRequest){
           UserDetails userDetails = authenticationService.authentication(authenticationRequest);
           return userDetails.getFullname() + " 登录成功";
       }
   }
   ```



## 实现会话功能

会话是指用户登入系统后，系统会记住该用户的登录状态，他可以在系统连续操作直到退出系统的过程。



认证的目的是对系统资源的保护，每次对资源的访问，系统必须得知道是谁在访问资源，才能对该请求进行合法性拦截。因此，在认证成功后，一般会把认证成功的用户信息放入Session中，在后续的请求中，系统能够从Session 中获取到当前用户，用这样的方式来实现会话机制。



1. 增加会话控制

   1. 首先在UserDto中定义一个SESSION_USER_KEY，作为Session中存放登录用户信息的key。

      ```java
      public static final String SESSION_USER_KEY = "_user";
      ```

   2. 然后修改LoginController，认证成功后，将用户信息放入当前会话。并增加用户登出方法，登出时将session置为失效。

      ```java
      /**
      * 用户登录
      * @param authenticationRequest 登录请求
      * @param session http会话
      * @return
      */
      @PostMapping(value = "/login",produces = "text/plain;charset=utf‐8")
      public String login(AuthenticationRequest authenticationRequest, HttpSession session){
          UserDto userDto = authenticationService.authentication(authenticationRequest);
          //用户信息存入session
          session.setAttribute(UserDto.SESSION_USER_KEY,userDto);
          return userDto.getUsername() + "登录成功";
      }
      @GetMapping(value = "logout",produces = "text/plain;charset=utf‐8")
      public String logout(HttpSession session){
          session.invalidate();
          return "退出成功";
      }
      ```

2. 增加测试资源：

   修改LoginController，增加测试资源1，它从当前会话session中获取当前登录用户，并返回提示信息给前台。

   ```java
   /**
   * 测试资源1
   * @param session
   * @return
   */
   @GetMapping(value = "/r/r1",produces = {"text/plain;charset=UTF‐8"})
   public String r1(HttpSession session){
       String fullname = null;
       Object userObj = session.getAttribute(UserDto.SESSION_USER_KEY);
       if(userObj != null){
           fullname = ((UserDto)userObj).getFullname();
       }else{
           fullname = "匿名";
       }
       return fullname + " 访问资源1";
   }
   ```

3. 测试：访问测试资源/r/r1：

   

   

## 实现授权功能

现在我们已经完成了用户身份凭证的校验以及登录的状态保持，并且我们也知道了如何获取当前登录用户(从 Session中获取)的信息，接下来，用户访问系统需要经过授权，即需要完成如下功能：

- 匿名用户（未登录用户）访问拦截：禁止匿名用户访问某些资源。
- 登录用户访问拦截：根据用户的权限决定是否能访问某些资源。



1. 增加权限数据

   为了实现这样的功能，我们需要在UserDto里增加权限属性，用于表示该登录用户所拥有的权限，同时修改 UserDto的构造方法。

   ```java
   @Data
   @AllArgsConstructor
   public class UserDto {
       public static final String SESSION_USER_KEY = "_user";
       private String id;
       private String username;
       private String password;
       private String fullname;
       private String mobile;
       /**
   * 用户权限
   */
       private Set<String> authorities;
   }
   ```

   并在AuthenticationServiceImpl中为模拟用户初始化权限，其中张三给了p1权限，李四给了p2权限。

   ```java
   //用户信息
   private Map<String, UserDto> userMap = new HashMap<>();
   
   {
       Set<String> authorities1 = new HashSet<>();
       authorities1.add("p1");
       Set<String> authorities2 = new HashSet<>();
       authorities2.add("p2");
       userMap.put("zhangsan", new UserDto("1010", "zhangsan", "123", "张 三", "133443", authorities1));
       userMap.put("lisi", new UserDto("1011", "lisi", "456", "李四", "144553", authorities2));
   }
   ```

2. 增加测试资源

   在LoginController中增加测试资源2

   ```java
   /**
   * 测试资源2
   * @param session
   * @return
   */
   @GetMapping(value = "/r/r2",produces = {"text/plain;charset=UTF‐8"})
   public String r2(HttpSession session){
       String fullname = null;
       Object userObj = session.getAttribute(UserDto.SESSION_USER_KEY);
       if(userObj != null){
           fullname = ((UserDto)userObj).getFullname();
       }else{
           fullname = "匿名";
       }
       return fullname + " 访问资源2";
   }
   ```

3. 实现授权拦截器

   在interceptor包下定义 拦截器，实现授权拦截：

   1. 校验用户是否登录
   2. 校验用户是否拥有操作权限

   ```java
   @Component
   public class SimpleAuthenticationInterceptor implements HandlerInterceptor {
       //请求拦截方法
       @Override
       public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object
                                handler) throws Exception {
           //读取会话信息
           Object object = request.getSession().getAttribute(UserDto.SESSION_USER_KEY);
           if(object == null){
               writeContent(response,"请登录");
           }
           UserDto user = (UserDto) object;
           //请求的url
           String requestURI = request.getRequestURI();
           if(user.getAuthorities().contains("p1") && requestURI.contains("/r1")){
               return true;
           }
           if(user.getAuthorities().contains("p2") && requestURI.contains("/r2")){
               return true;
           }
           writeContent(response,"权限不足，拒绝访问");
           return false;
       }
       //响应输出
       private void writeContent(HttpServletResponse response, String msg) throws IOException {
           response.setContentType("text/html;charset=utf‐8");
           PrintWriter writer = response.getWriter();
           writer.print(msg);
           writer.close();
           response.resetBuffer();
       }
   }
   ```

   在WebConfig中配置拦截器，匹配/r/**的资源为受保护的系统资源，访问该资源的请求进入 SimpleAuthenticationInterceptor拦截器。

   ```java
   @Autowired
   private SimpleAuthenticationInterceptor simpleAuthenticationInterceptor;
   @Override
   public void addInterceptors(InterceptorRegistry registry) {
       registry.addInterceptor(simpleAuthenticationInterceptor).addPathPatterns("/r/**");
   }
   ```

4. 测试

   - 未登录情况下，/r/r1与/r/r2均提示 “请先登录”。
   - 张三登录情况下，由于张三有p1权限，因此可以访问/r/r1，张三没有p2权限，访问/r/r2时提示 “权限不足 “。
   - 李四登录情况下，由于李四有p2权限，因此可以访问/r/r2，李四没有p1权限，访问/r/r1时提示 “权限不足 “。



# Spring Security快速上手

## Spring Security介绍

Spring Security是一个能够为基于Spring的企业应用系统提供声明式的安全访问控制解决方案的安全框架。由于它 是Spring生态系统中的一员，因此它伴随着整个Spring生态系统不断修正、升级，在spring boot项目中加入spring security更是十分简单，使用Spring Security 减少了为企业系统安全控制编写大量重复代码的工作。



## 创建工程

### 创建maven工程

创建maven工程 security-spring-security，工程结构如下：

![image-20220430140150032](Spring%20Security.assets/image-20220430140150032.png)

引入以下依赖：

在security-springmvc的基础上增加spring-security的依赖：

```xml
<dependency>
    <groupId>org.springframework.security</groupId>
    <artifactId>spring‐security‐web</artifactId>
    <version>5.1.4.RELEASE</version>
</dependency>
<dependency>
    <groupId>org.springframework.security</groupId>
    <artifactId>spring‐security‐config</artifactId>
    <version>5.1.4.RELEASE</version>
</dependency>
```



### Spring容器配置

```java
package org.example.security.springmvc.config;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;
import org.springframework.stereotype.Controller;

@Configuration
@ComponentScan(basePackages = "org.example.security.springmvc",
        excludeFilters = {@ComponentScan.Filter(type = FilterType.ANNOTATION, value = Controller.class)})
// 相当于applicationContext.xml
public class ApplicationConfig {
    //在此配置除了Controller的其它bean，比如：数据库链接池、事务管理器、业务bean等。

}
```



### Servlet Context配置

```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;
import org.springframework.stereotype.Controller;
import org.springframework.web.servlet.config.annotation.EnableWebMvc;
import org.springframework.web.servlet.config.annotation.ViewControllerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
import org.springframework.web.servlet.view.InternalResourceViewResolver;

@Configuration
@EnableWebMvc
@ComponentScan(basePackages = "org.example.springmvc", includeFilters = {@ComponentScan.Filter(type = FilterType.ANNOTATION, value = Controller.class)})
// 此类相当于springmvc.xml文件
public class WebConfig implements WebMvcConfigurer {

    // 视图解析器
    @Bean
    public InternalResourceViewResolver viewResolver() {
        InternalResourceViewResolver viewResolver = new InternalResourceViewResolver();
        viewResolver.setPrefix("/WEB-INF/view/");
        viewResolver.setSuffix(".jsp");
        return viewResolver;
    }

    @Override
    public void addViewControllers(ViewControllerRegistry registry) {
        registry.addViewController("/").setViewName("login");
    }
}
```



### 加载 Spring 容器

在init包下定义Spring容器初始化类SpringApplicationInitializer，此类实现WebApplicationInitializer接口， Spring容器启动时加载WebApplicationInitializer接口的所有实现类。

```java
package org.example.security.springmvc.init;

import org.example.security.springmvc.config.ApplicationConfig;
import org.example.security.springmvc.config.WebConfig;
import org.springframework.web.servlet.support.AbstractAnnotationConfigDispatcherServletInitializer;

public class SpringApplicationInitializer extends AbstractAnnotationConfigDispatcherServletInitializer {

    // Spring容器，相当于加载applicationContext.xml
    @Override
    protected Class<?>[] getRootConfigClasses() {
        return new Class[]{ApplicationConfig.class};
    }

    // ServletContext，相当于加载springmvc.xml
    @Override
    protected Class<?>[] getServletConfigClasses() {
        return new Class[]{WebConfig.class};
    }

    // url-mapping
    @Override
    protected String[] getServletMappings() {
        return new String[]{"/"};
    }
}
```





## 认证

### 认证页面

springSecurity默认提供认证页面，不需要额外开发。

![image-20220430141430334](Spring%20Security.assets/image-20220430141430334.png)





### 安全配置

spring security提供了用户名密码登录、退出、会话管理等认证功能，只需要配置即可使用。



1. 在config包下定义WebSecurityConfig，安全配置的内容包括：用户信息、密码编码器、安全拦截机制。

   ```java
   import org.springframework.context.annotation.Bean;
   import org.springframework.security.config.annotation.web.builders.HttpSecurity;
   import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
   import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
   import org.springframework.security.core.userdetails.User;
   import org.springframework.security.core.userdetails.UserDetailsService;
   import org.springframework.security.crypto.password.NoOpPasswordEncoder;
   import org.springframework.security.crypto.password.PasswordEncoder;
   import org.springframework.security.provisioning.InMemoryUserDetailsManager;
   
   @EnableWebSecurity
   public class WebSecurityConfig extends WebSecurityConfigurerAdapter {
   
       // 定义用户信息服务（查询用户信息）
       @Bean
       public UserDetailsService userDetailsService() {
           InMemoryUserDetailsManager manager = new InMemoryUserDetailsManager();
   
           manager.createUser(User.withUsername("zhangsan").password("123").authorities("p1").build());
           manager.createUser(User.withUsername("lisi").password("123").authorities("p1").build());
   
           return manager;
       }
   
       // 密码编码器
       @Bean
       public PasswordEncoder passwordEncoder() {
           return NoOpPasswordEncoder.getInstance();
       }
   
       // 配置安全拦截机制
       @Override
       protected void configure(HttpSecurity http) throws Exception {
           http.authorizeRequests()
                   .antMatchers("/r/**").authenticated()   // 所有/r/**的请求必须认证通过
                   .anyRequest().permitAll()      // 除了/r/**的请求都可以访问
                   .and()
                   .formLogin()    //允许表单登录
                   .successForwardUrl("/login-success");   // 自定义登录成功的页面地址
           super.configure(http);
       }
   }
   ```

   在userDetailsService()方法中，我们返回了一个UserDetailsService给spring容器，Spring Security会使用它来 获取用户信息。我们暂时使用InMemoryUserDetailsManager实现类，并在其中分别创建了zhangsan、lisi两个用 户，并设置密码和权限。

   而在configure()中，我们通过HttpSecurity设置了安全拦截规则，其中包含了以下内容：

   （1）url匹配/r/**的资源，经过认证后才能访问。

   （2）其他url完全开放。

   （3）支持form表单认证，认证成功后转向/login-success。

   关于HttpSecurity的配置清单请参考附录 HttpSecurity。

2. 加载 WebSecurityConfig

   修改SpringApplicationInitializer的getRootConfigClasses()方法，添加WebSecurityConfig.class：

   ```java
   import org.example.springmvc.config.ApplicationConfig;
   import org.example.springmvc.config.WebConfig;
   import org.example.springmvc.config.WebSecurityConfig;
   import org.springframework.web.servlet.support.AbstractAnnotationConfigDispatcherServletInitializer;
   
   public class SpringApplicationInitializer extends AbstractAnnotationConfigDispatcherServletInitializer {
   
       // Spring容器，相当于加载applicationContext.xml
       @Override
       protected Class<?>[] getRootConfigClasses() {
           return new Class[]{ApplicationConfig.class, WebSecurityConfig.class};
       }
   
       // ServletContext，相当于加载springmvc.xml
       @Override
       protected Class<?>[] getServletConfigClasses() {
           return new Class[]{WebConfig.class};
       }
   
       // url-mapping
       @Override
       protected String[] getServletMappings() {
           return new String[]{"/"};
       }
   }
   ```

   

### Spring Security初始化

Spring Security初始化，这里有两种情况：

- 若当前环境没有使用Spring或Spring MVC，则需要将 WebSecurityConfig(Spring Security配置类) 传入超类，以确保获取配置，并创建spring context。
- 相反，若当前环境已经使用spring，我们应该在现有的springContext中注册Spring Security(上一步已经做将WebSecurityConfig加载至rootcontext)，此方法可以什么都不做。



在init包下定义SpringSecurityApplicationInitializer：

```java
public class SpringSecurityApplicationInitializer
    extends AbstractSecurityWebApplicationInitializer {
    public SpringSecurityApplicationInitializer() {
        //super(WebSecurityConfig.class);
    }
}
```



### 默认根路径请求

在WebConfig.java中添加默认请求根路径跳转到/login，此url为spring security提供：

```java
//默认Url根路径跳转到/login，此url为spring security提供
@Override
public void addViewControllers(ViewControllerRegistry registry) {
    registry.addViewController("/").setViewName("redirect:/login");
}
```

spring security默认提供的登录页面。



### 认证成功页面

在安全配置中，认证成功将跳转到/login-success，代码如下：

```java
//配置安全拦截机制
@Override
protected void configure(HttpSecurity http) throws Exception {
    http
        .authorizeRequests()
        .antMatchers("/r/**").authenticated() （1）
        .anyRequest().permitAll() （2）
        .and()
        .formLogin().successForwardUrl("/login‐success"); （3）
}
```

spring security支持form表单认证，认证成功后转向/login-success。

在LoginController中定义/login-success:

```java
@RequestMapping(value = "/login‐success",produces = {"text/plain;charset=UTF‐8"})
public String loginSuccess(){
    return " 登录成功";
}
```



## 授权

实现授权需要对用户的访问进行拦截校验，校验用户的权限是否可以操作指定的资源，Spring Security默认提供授权实现方法。

1. 在LoginController添加/r/r1或/r/r2

   ```java
   @RestController
   public class LoginController {
   
       @RequestMapping(value = "/login-success", produces = "text/plain;charset=utf-8")
       public String loginSuccess() {
           return "登录成功";
       }
   
       @GetMapping(value = "/r/r1",produces = {"text/plain;charset=utf-8"})
       public String r1(HttpSession session){
           return "访问资源1";
       }
   
       @GetMapping(value = "/r/r2",produces = {"text/plain;charset=utf-8"})
       public String r2(HttpSession session){
           return "访问资源2";
       }
   }
   ```

2. 在安全配置类WebSecurityConfig.java中配置授权规则：

   ```java
   // 配置安全拦截机制
   @Override
   protected void configure(HttpSecurity http) throws Exception {
       http.authorizeRequests()
           .antMatchers("/r/r1").hasAnyAuthority("p1")
           .antMatchers("/r/r2").hasAnyAuthority("p2")
           .antMatchers("/r/**").authenticated()   // 所有/r/**的请求必须认证通过
           .anyRequest().permitAll()      // 除了/r/**的请求都可以访问
           .and()
           .formLogin()    //允许表单登录
           .successForwardUrl("/login-success");   // 自定义登录成功的页面地址
       super.configure(http);
   }
   ```

   - .antMatchers("/r/r1").hasAuthority("p1")表示：访问/r/r1资源的 url需要拥有p1权限。 

   - .antMatchers("/r/r2").hasAuthority("p2")表示：访问/r/r2资源的 url需要拥有p2权限。



# Spring Security 应用详解

[黑马程序员Java进阶教程快速入门Spring Security OAuth2.0认证授权_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1VE411h7aL?p=12)
