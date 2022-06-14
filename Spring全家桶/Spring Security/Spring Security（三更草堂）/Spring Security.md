# 相关资料

- [视频教程](https://www.bilibili.com/video/BV1mm4y1X7Hc?spm_id_from=333.337.search-card.all.click&vd_source=be746efb77e979ca275e4f65f2d8cda3)



# 认证与授权

- 认证：验证当前访问用户，是否为本系统用户；并确认是哪个用户。
- 授权：经过认证后，判断当前用户是否具有权限，进行某个操作。



# 快速入门

## 创建Spring boot项目

依赖如下：

```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.5.0</version>
</parent>

<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <optional>true</optional>
    </dependency>
</dependencies>
```



创建启动类：

```java
@SpringBootApplication
public class SecurityApplication {
    public static void main(String[] args) {
        SpringApplication.run(SecurityApplication.class, args);
    }
}
```

创建Controller：

```java
@RestController
public class HelloController {

    @RequestMapping("/hello")
    public String hello() {
        return "hello";
    }
}
```



## 引入Spring Security

添加Spring Security依赖：

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-security</artifactId>
</dependency>
```



在引入依赖后，当尝试去访问之前的hello接口时，将自动跳转至SpringSecurity的默认登陆页面。

> 默认的：
>
> - 用户名：user
> - 密码：输出在控制台

只有在登录之后，才能对接口进行访问。





# 认证

## 登录校验流程

![image-20220614162742025](Spring%20Security.assets/image-20220614162742025.png)



## 原理初探

### Spring Security处理流程

SpringSecurity的原理是一个**过滤器链**，内部包含了提供各种功能的过滤器。



核心的过滤器如下：

![image-20220614163707591](Spring%20Security.assets/image-20220614163707591.png)

- `UsernamePasswordAuthenticationFilter`：

  负责处理我们在登录页面填写了用户名和密码的登录请求。

- `ExceptionTranslationFilter`：

  处理器过滤器链中抛出的任何 AccessDeniedException 和 AuthenticationException

- `FilterSecurityInterceptor`：

  负责校验权限的拦截器



> 调用`容器.getBean(DefaultSecurityFilterChain.class);`

![image-20220614164855465](Spring%20Security.assets/image-20220614164855465.png)



### 认证流程详解

![image-20220614165102472](Spring%20Security.assets/image-20220614165102472.png)

- Authentication接口：它的实现类，表示当前访问系统的用户，封装了用户相关信息
- AuthenticationManager接口：定义了认证Authentication的方法
- UserDetailService接口：加载用户特定数据的核心接口。里面定义了一个根据用户名查询用户信息的方法
- UserDetails接口：提供核心用户信息。通过UserDetailService，根据用户名获取处理的用户信息，将其封装为UserDetails对象返回。然后将这些信息封装到Authentication对象中。



## 改进入门案例

### 思路分析

- 登录：

  ![image-20220614170316989](Spring%20Security.assets/image-20220614170316989.png)

  1. 自定义登录接口：

     1. 调用ProviderMananger的方法进行认证
     2. 如果认证通过，根据用户ID生成JWT，将其存入Redis

  2. 自定义UserDetailServie实现类：

     查询数据库



- 校验：

  ![image-20220614170506519](Spring%20Security.assets/image-20220614170506519.png)

  定义JWT认证过滤器：

  1. 获取Token
  2. 解析Token，得到用户ID
  3. 根据用户ID，从Redis获取用户信息
  4. 将用户信息存入SecurityContextHolder，给其他过滤器使用。



### 准备工作

- 额外添加项目依赖：

  ```xml
  <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-data-redis</artifactId>
  </dependency>
  
  <dependency>
      <groupId>com.alibaba</groupId>
      <artifactId>fastjson</artifactId>
      <version>1.2.33</version>
  </dependency>
  
  <!-- JWT依赖 -->
  <dependency>
      <groupId>io.jsonwebtoken</groupId>
      <artifactId>jjwt</artifactId>
      <version>0.9.0</version>
  </dependency>
  ```

- FastJson序列化器：

  ```java
  import com.alibaba.fastjson.JSON;
  import com.alibaba.fastjson.parser.ParserConfig;
  import com.alibaba.fastjson.serializer.SerializerFeature;
  import com.fasterxml.jackson.databind.JavaType;
  import com.fasterxml.jackson.databind.type.TypeFactory;
  import org.springframework.data.redis.serializer.RedisSerializer;
  import org.springframework.data.redis.serializer.SerializationException;
  
  import java.nio.charset.Charset;
  import java.nio.charset.StandardCharsets;
  
  public class FastJsonRedisSerializer<T> implements RedisSerializer<T> {
  
  
      public static final Charset DEFAULT_CHARSET = StandardCharsets.UTF_8;
  
      private Class<T> clazz;
  
      static {
          ParserConfig.getGlobalInstance().setAutoTypeSupport(true);
      }
  
      public FastJsonRedisSerializer(Class<T> clazz) {
          super();
          this.clazz = clazz;
      }
  
      @Override
      public byte[] serialize(T t) throws SerializationException {
          if (t == null) {
              return new byte[0];
          }
  
          return JSON.toJSONString(t, SerializerFeature.WriteClassName).getBytes(DEFAULT_CHARSET);
      }
  
      @Override
      public T deserialize(byte[] bytes) throws SerializationException {
          if (bytes == null || bytes.length <= 0) {
              return null;
          }
  
          String str = new String(bytes, DEFAULT_CHARSET);
          return JSON.parseObject(str, clazz);
      }
  
      protected JavaType getJavaType(Class<T> clazz) {
          return TypeFactory.defaultInstance().constructType(clazz);
      }
  }
  ```

- 配置Redis：

  ```java
  import org.example.utils.FastJsonRedisSerializer;
  import org.springframework.context.annotation.Bean;
  import org.springframework.context.annotation.Configuration;
  import org.springframework.data.redis.connection.RedisConnectionFactory;
  import org.springframework.data.redis.core.RedisTemplate;
  import org.springframework.data.redis.serializer.StringRedisSerializer;
  
  @Configuration
  public class RedisConfig {
  
      @Bean
      @SuppressWarnings(value = {"unchecked", "rawtypes"})
      public RedisTemplate<Object, Object> redisTemplate(RedisConnectionFactory connectionFactory) {
          RedisTemplate<Object, Object> template = new RedisTemplate<>();
          template.setConnectionFactory(connectionFactory);
  
          FastJsonRedisSerializer serializer = new FastJsonRedisSerializer(Object.class);
  
          // 使用StringRedisSerializer来序列号和反序列化Redis的Key值
          template.setKeySerializer(new StringRedisSerializer());
          template.setValueSerializer(serializer);
  
          // Hash的Key也采用StringRedisSerializer的序列化方式
          template.setHashKeySerializer(new StringRedisSerializer());
          template.setHashValueSerializer(serializer);
  
          template.afterPropertiesSet();
          return template;
      }
  }
  
  ```

- 定义响应类：

  ```java
  @Getter
  @Setter
  @JsonInclude(JsonInclude.Include.NON_NULL)
  public class ResponseResult<T> {
  
      private Integer code; // 状态码
      private String msg; // 提示信息
      private T data; // 查询到的结果数据
  
      public ResponseResult(Integer code, String msg) {
          this.code = code;
          this.msg = msg;
      }
  
      public ResponseResult(Integer code, T data) {
          this.code = code;
          this.data = data;
      }
  
      public ResponseResult(Integer code, String msg, T data) {
          this.code = code;
          this.msg = msg;
          this.data = data;
      }
  }
  ```

- JWT工具类：

  ```java
  import io.jsonwebtoken.Claims;
  import io.jsonwebtoken.JwtBuilder;
  import io.jsonwebtoken.Jwts;
  import io.jsonwebtoken.SignatureAlgorithm;
  
  import javax.crypto.SecretKey;
  import javax.crypto.spec.SecretKeySpec;
  import java.util.Base64;
  import java.util.Date;
  import java.util.UUID;
  
  public class JwtUtil {
  
      public static final Long JWT_TTL = 60 * 60 * 1000L;
  
      // 设置密钥明文
      public static final String JWT_KEY = "sangeng";
  
      public static String getUUID() {
          return UUID.randomUUID().toString().replaceAll("-", "");
      }
  
      /**
       * 生成JWT
       *
       * @param subject token中要存放的数据（JSON格式）
       */
      public static String createJWT(String subject) {
          JwtBuilder builder = getJwtBuilder(subject, null, getUUID());
          return builder.compact();
      }
  
      public static String createJWT(String subject, Long ttlMillis) {
          JwtBuilder builder = getJwtBuilder(subject, ttlMillis, getUUID());
          return builder.compact();
      }
  
      public static String createJwt(String id, String subject, long ttlMills) {
          JwtBuilder builder = getJwtBuilder(subject, ttlMills, id);
          return builder.compact();
      }
  
  
      private static JwtBuilder getJwtBuilder(String subject, Long ttlMillis, String uuid) {
          SignatureAlgorithm signatureAlgorithm = SignatureAlgorithm.HS256;
          SecretKey secretKey = generalKey();
          long nowMillis = System.currentTimeMillis();
          Date now = new Date(nowMillis);
          if (ttlMillis == null) {
              ttlMillis = JwtUtil.JWT_TTL;
          }
          long expMillis = nowMillis + ttlMillis;
          Date expDate = new Date(expMillis);
  
          return Jwts.builder()
                  .setId(uuid)            // 唯一ID
                  .setSubject(subject)    // 主题，可以为JSON数据
                  .setIssuer("sg")        // 签发者
                  .setIssuedAt(now)       // 签发日期
                  .signWith(signatureAlgorithm, secretKey)    //使用HS256对称加密算法签名，第二个参数为密钥
                  .setExpiration(expDate);
      }
  
      public static SecretKey generalKey() {
          byte[] decodedKey = Base64.getDecoder().decode(JwtUtil.JWT_KEY);
          SecretKeySpec key = new SecretKeySpec(decodedKey, 0, decodedKey.length, "AES");
          return key;
      }
  
      /**
       * 解析JWT
       */
      public static Claims parseJWT(String jwt) {
          SecretKey secretKey = generalKey();
          return Jwts.parser()
                  .setSigningKey(secretKey)
                  .parseClaimsJws(jwt)
                  .getBody();
      }
  }
  ```

- RedisCache：封装RedisTemplate的方法

  ```java
  ```

- WebUtils：

  ```java
  ```

- 实体类：

  ```java
  ```

  

### 代码实现

[SpringSecurity框架教程-Spring Security+JWT实现项目级前端分离认证授权-B站最通俗易懂的Spring Security课程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1mm4y1X7Hc?p=12&spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)



