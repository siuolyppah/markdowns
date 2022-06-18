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

![image-20220615203130507](Spring%20Security.assets/image-20220615203130507.png)



## 原理初探

### Spring Security处理流程

SpringSecurity的原理是一个**过滤器链**，内部包含了提供各种功能的过滤器。



核心的过滤器如下：

![image-20220615203125102](Spring%20Security.assets/image-20220615203125102.png)

- `UsernamePasswordAuthenticationFilter`：

  负责处理我们在登录页面填写了用户名和密码的登录请求。

- `ExceptionTranslationFilter`：

  处理器过滤器链中抛出的任何 AccessDeniedException 和 AuthenticationException

- `FilterSecurityInterceptor`：

  负责校验权限的拦截器



> 调用`容器.getBean(DefaultSecurityFilterChain.class);`

![image-20220614164855465](Spring%20Security.assets/image-20220614164855465.png)



### 认证流程详解

![image-20220615203113386](Spring%20Security.assets/image-20220615203113386.png)

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
  
  <dependency>
      <groupId>com.baomidou</groupId>
      <artifactId>mybatis-plus-boot-starter</artifactId>
      <version>3.4.3</version>
  </dependency>
  <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
  </dependency>
  
  <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
  </dependency>
  ```


- 配置：

  ```yaml
  spring:
    datasource:
      url: jdbc:mysql://localhost:3306/sg_security?characterEncoding=utf-8&serverTimezone=UTC
      username: root
      password: root
      driver-class-name: com.mysql.cj.jdbc.Driver
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
  
  import java.util.*;
  import java.util.concurrent.TimeUnit;
  
  @SuppressWarnings(value = { "unchecked", "rawtypes" })
  @Component
  public class RedisCache
  {
      @Autowired
      public RedisTemplate redisTemplate;
  
      /**
       * 缓存基本的对象，Integer、String、实体类等
       *
       * @param key 缓存的键值
       * @param value 缓存的值
       */
      public <T> void setCacheObject(final String key, final T value)
      {
          redisTemplate.opsForValue().set(key, value);
      }
  
      /**
       * 缓存基本的对象，Integer、String、实体类等
       *
       * @param key 缓存的键值
       * @param value 缓存的值
       * @param timeout 时间
       * @param timeUnit 时间颗粒度
       */
      public <T> void setCacheObject(final String key, final T value, final Integer timeout, final TimeUnit timeUnit)
      {
          redisTemplate.opsForValue().set(key, value, timeout, timeUnit);
      }
  
      /**
       * 设置有效时间
       *
       * @param key Redis键
       * @param timeout 超时时间
       * @return true=设置成功；false=设置失败
       */
      public boolean expire(final String key, final long timeout)
      {
          return expire(key, timeout, TimeUnit.SECONDS);
      }
  
      /**
       * 设置有效时间
       *
       * @param key Redis键
       * @param timeout 超时时间
       * @param unit 时间单位
       * @return true=设置成功；false=设置失败
       */
      public boolean expire(final String key, final long timeout, final TimeUnit unit)
      {
          return redisTemplate.expire(key, timeout, unit);
      }
  
      /**
       * 获得缓存的基本对象。
       *
       * @param key 缓存键值
       * @return 缓存键值对应的数据
       */
      public <T> T getCacheObject(final String key)
      {
          ValueOperations<String, T> operation = redisTemplate.opsForValue();
          return operation.get(key);
      }
  
      /**
       * 删除单个对象
       *
       * @param key
       */
      public boolean deleteObject(final String key)
      {
          return redisTemplate.delete(key);
      }
  
      /**
       * 删除集合对象
       *
       * @param collection 多个对象
       * @return
       */
      public long deleteObject(final Collection collection)
      {
          return redisTemplate.delete(collection);
      }
  
      /**
       * 缓存List数据
       *
       * @param key 缓存的键值
       * @param dataList 待缓存的List数据
       * @return 缓存的对象
       */
      public <T> long setCacheList(final String key, final List<T> dataList)
      {
          Long count = redisTemplate.opsForList().rightPushAll(key, dataList);
          return count == null ? 0 : count;
      }
  
      /**
       * 获得缓存的list对象
       *
       * @param key 缓存的键值
       * @return 缓存键值对应的数据
       */
      public <T> List<T> getCacheList(final String key)
      {
          return redisTemplate.opsForList().range(key, 0, -1);
      }
  
      /**
       * 缓存Set
       *
       * @param key 缓存键值
       * @param dataSet 缓存的数据
       * @return 缓存数据的对象
       */
      public <T> BoundSetOperations<String, T> setCacheSet(final String key, final Set<T> dataSet)
      {
          BoundSetOperations<String, T> setOperation = redisTemplate.boundSetOps(key);
          Iterator<T> it = dataSet.iterator();
          while (it.hasNext())
          {
              setOperation.add(it.next());
          }
          return setOperation;
      }
  
      /**
       * 获得缓存的set
       *
       * @param key
       * @return
       */
      public <T> Set<T> getCacheSet(final String key)
      {
          return redisTemplate.opsForSet().members(key);
      }
  
      /**
       * 缓存Map
       *
       * @param key
       * @param dataMap
       */
      public <T> void setCacheMap(final String key, final Map<String, T> dataMap)
      {
          if (dataMap != null) {
              redisTemplate.opsForHash().putAll(key, dataMap);
          }
      }
  
      /**
       * 获得缓存的Map
       *
       * @param key
       * @return
       */
      public <T> Map<String, T> getCacheMap(final String key)
      {
          return redisTemplate.opsForHash().entries(key);
      }
  
      /**
       * 往Hash中存入数据
       *
       * @param key Redis键
       * @param hKey Hash键
       * @param value 值
       */
      public <T> void setCacheMapValue(final String key, final String hKey, final T value)
      {
          redisTemplate.opsForHash().put(key, hKey, value);
      }
  
      /**
       * 获取Hash中的数据
       *
       * @param key Redis键
       * @param hKey Hash键
       * @return Hash中的对象
       */
      public <T> T getCacheMapValue(final String key, final String hKey)
      {
          HashOperations<String, String, T> opsForHash = redisTemplate.opsForHash();
          return opsForHash.get(key, hKey);
      }
  
      /**
       * 删除Hash中的数据
       * 
       * @param key
       * @param hkey
       */
      public void delCacheMapValue(final String key, final String hkey)
      {
          HashOperations hashOperations = redisTemplate.opsForHash();
          hashOperations.delete(key, hkey);
      }
  
      /**
       * 获取多个Hash中的数据
       *
       * @param key Redis键
       * @param hKeys Hash键集合
       * @return Hash对象集合
       */
      public <T> List<T> getMultiCacheMapValue(final String key, final Collection<Object> hKeys)
      {
          return redisTemplate.opsForHash().multiGet(key, hKeys);
      }
  
      /**
       * 获得缓存的基本对象列表
       *
       * @param pattern 字符串前缀
       * @return 对象列表
       */
      public Collection<String> keys(final String pattern)
      {
          return redisTemplate.keys(pattern);
      }
  }
  ```

- WebUtils：

  ```java
  
  import javax.servlet.http.HttpServletResponse;
  import java.io.IOException;
  
  public class WebUtils
  {
      /**
       * 将字符串渲染到客户端
       * 
       * @param response 渲染对象
       * @param string 待渲染的字符串
       * @return null
       */
      public static String renderString(HttpServletResponse response, String string) {
          try
          {
              response.setStatus(200);
              response.setContentType("application/json");
              response.setCharacterEncoding("utf-8");
              response.getWriter().print(string);
          }
          catch (IOException e)
          {
              e.printStackTrace();
          }
          return null;
      }
  }
  ```

- 实体类：

  ```java
  import java.io.Serializable;
  import java.util.Date;
  
  
  /**
   * 用户表(User)实体类
   *
   * @author 三更
   */
  @Data
  @AllArgsConstructor
  @NoArgsConstructor
  public class User implements Serializable {
      private static final long serialVersionUID = -40356785423868312L;
      
      /**
      * 主键
      */
      private Long id;
      /**
      * 用户名
      */
      private String userName;
      /**
      * 昵称
      */
      private String nickName;
      /**
      * 密码
      */
      private String password;
      /**
      * 账号状态（0正常 1停用）
      */
      private String status;
      /**
      * 邮箱
      */
      private String email;
      /**
      * 手机号
      */
      private String phonenumber;
      /**
      * 用户性别（0男，1女，2未知）
      */
      private String sex;
      /**
      * 头像
      */
      private String avatar;
      /**
      * 用户类型（0管理员，1普通用户）
      */
      private String userType;
      /**
      * 创建人的用户id
      */
      private Long createBy;
      /**
      * 创建时间
      */
      private Date createTime;
      /**
      * 更新人
      */
      private Long updateBy;
      /**
      * 更新时间
      */
      private Date updateTime;
      /**
      * 删除标志（0代表未删除，1代表已删除）
      */
      private Integer delFlag;
  }
  ```




- 数据库：

  ```sql
  CREATE TABLE `sys_user` (
    `id` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '主键',
    `user_name` VARCHAR(64) NOT NULL DEFAULT 'NULL' COMMENT '用户名',
    `nick_name` VARCHAR(64) NOT NULL DEFAULT 'NULL' COMMENT '昵称',
    `password` VARCHAR(64) NOT NULL DEFAULT 'NULL' COMMENT '密码',
    `status` CHAR(1) DEFAULT '0' COMMENT '账号状态（0正常 1停用）',
    `email` VARCHAR(64) DEFAULT NULL COMMENT '邮箱',
    `phonenumber` VARCHAR(32) DEFAULT NULL COMMENT '手机号',
    `sex` CHAR(1) DEFAULT NULL COMMENT '用户性别（0男，1女，2未知）',
    `avatar` VARCHAR(128) DEFAULT NULL COMMENT '头像',
    `user_type` CHAR(1) NOT NULL DEFAULT '1' COMMENT '用户类型（0管理员，1普通用户）',
    `create_by` BIGINT(20) DEFAULT NULL COMMENT '创建人的用户id',
    `create_time` DATETIME DEFAULT NULL COMMENT '创建时间',
    `update_by` BIGINT(20) DEFAULT NULL COMMENT '更新人',
    `update_time` DATETIME DEFAULT NULL COMMENT '更新时间',
    `del_flag` INT(11) DEFAULT '0' COMMENT '删除标志（0代表未删除，1代表已删除）',
    PRIMARY KEY (`id`)
  ) ENGINE=INNODB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COMMENT='用户表'
  ```

  

- 定义Mapper接口

  ```java
  public interface UserMapper extends BaseMapper<User> {
  }
  ```

- 修改User实体类

  ```java
  类名上加@TableName(value = "sys_user") ,id字段上加 @TableId
  ```

- 配置Mapper扫描

  ```java
  @SpringBootApplication
  @MapperScan("com.sangeng.mapper")
  public class SimpleSecurityApplication {
      public static void main(String[] args) {
          ConfigurableApplicationContext run = SpringApplication.run(SimpleSecurityApplication.class);
          System.out.println(run);
      }
  }
  ```

  

### 数据库校验用户

创建一个类实现UserDetailsService接口，重写其中的方法。更加用户名从数据库中查询用户信息

```java
/**
 * @Author 三更  B站： https://space.bilibili.com/663528522
 */
@Service
public class UserDetailsServiceImpl implements UserDetailsService {

    @Autowired
    private UserMapper userMapper;

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        //根据用户名查询用户信息
        LambdaQueryWrapper<User> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(User::getUserName,username);
        User user = userMapper.selectOne(wrapper);
        //如果查询不到数据就通过抛出异常来给出提示
        if(Objects.isNull(user)){
            throw new RuntimeException("用户名或密码错误");
        }
        //TODO 根据用户查询权限信息 添加到LoginUser中
        
        //封装成UserDetails对象返回 
        return new LoginUser(user);
    }
}
```

因为UserDetailsService方法的返回值是UserDetails类型，所以需要定义一个类，实现该接口，把用户信息封装在其中。

```java
/**
 * @Author 三更  B站： https://space.bilibili.com/663528522
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class LoginUser implements UserDetails {

    private User user;


    @Override
    public Collection<? extends GrantedAuthority> getAuthorities() {
        return null;
    }

    @Override
    public String getPassword() {
        return user.getPassword();
    }

    @Override
    public String getUsername() {
        return user.getUserName();
    }

    @Override
    public boolean isAccountNonExpired() {
        return true;
    }

    @Override
    public boolean isAccountNonLocked() {
        return true;
    }

    @Override
    public boolean isCredentialsNonExpired() {
        return true;
    }

    @Override
    public boolean isEnabled() {
        return true;
    }
}
```

>注意：如果要测试，需要往用户表中写入用户数据，并且如果你想让用户的密码是明文存储，需要在密码前加{noop}。例如 {noop}123



### 密码加密存储

- 实际项目中我们不会把密码明文存储在数据库中。

- 默认使用的PasswordEncoder要求数据库中的密码格式为：{id}password 。它会根据id去判断密码的加密方式。但是我们一般不会采用这种方式。所以就需要替换PasswordEncoder。

- 我们一般使用SpringSecurity为我们提供的`BCryptPasswordEncoder`。

  > 我们只需要使用把BCryptPasswordEncoder对象注入Spring容器中，SpringSecurity就会使用该PasswordEncoder来进行密码校验。



- 我们可以定义一个SpringSecurity的配置类，SpringSecurity要求这个配置类要继承WebSecurityConfigurerAdapter。

  ```java
  /**
   * @Author 三更  B站： https://space.bilibili.com/663528522
   */
  @Configuration
  public class SecurityConfig extends WebSecurityConfigurerAdapter {
  
  
      @Bean
      public PasswordEncoder passwordEncoder(){
          return new BCryptPasswordEncoder();
      }
  
  }
  ```



### 登录接口

- 接下来我们需要自定义登陆接口，然后**让SpringSecurity对这个接口放行**,让用户访问这个接口的时候不用登录也能访问
- 在接口中我们通过AuthenticationManager的authenticate方法来进行用户认证,所以需要在SecurityConfig中配置把AuthenticationManager注入容器。
- 认证成功的话要生成一个jwt，放入响应中返回。并且为了让用户下回请求时能通过jwt识别出具体的是哪个用户，我们需要把用户信息存入redis，可以把用户id作为key。



~~~~java
@RestController
public class LoginController {

    @Autowired
    private LoginServcie loginServcie;

    @PostMapping("/user/login")
    public ResponseResult login(@RequestBody User user){
        return loginServcie.login(user);
    }
}
~~~~

~~~~java
/**
 * @Author 三更  B站： https://space.bilibili.com/663528522
 */
@Configuration
public class SecurityConfig extends WebSecurityConfigurerAdapter {


    @Bean
    public PasswordEncoder passwordEncoder(){
        return new BCryptPasswordEncoder();
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
                //关闭csrf
                .csrf().disable()
                //不通过Session获取SecurityContext
                .sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS)
                .and()
                .authorizeRequests()
                // 对于登录接口 允许匿名访问
                .antMatchers("/user/login").anonymous()
                // 除上面外的所有请求全部需要鉴权认证
                .anyRequest().authenticated();
    }

    @Bean
    @Override
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return super.authenticationManagerBean();
    }
}
~~~~

​	

~~~~java
@Service
public class LoginServiceImpl implements LoginServcie {

    @Autowired
    private AuthenticationManager authenticationManager;
    @Autowired
    private RedisCache redisCache;

    @Override
    public ResponseResult login(User user) {
        UsernamePasswordAuthenticationToken authenticationToken = new UsernamePasswordAuthenticationToken(user.getUserName(),user.getPassword());
        Authentication authenticate = authenticationManager.authenticate(authenticationToken);
        if(Objects.isNull(authenticate)){
            throw new RuntimeException("用户名或密码错误");
        }
        //使用userid生成token
        LoginUser loginUser = (LoginUser) authenticate.getPrincipal();
        String userId = loginUser.getUser().getId().toString();
        String jwt = JwtUtil.createJWT(userId);
        //authenticate存入redis
        redisCache.setCacheObject("login:"+userId,loginUser);
        //把token响应给前端
        HashMap<String,String> map = new HashMap<>();
        map.put("token",jwt);
        return new ResponseResult(200,"登陆成功",map);
    }
}

~~~~



> 关于Redis：
>
> - 启动服务：redis-server.exe



### 认证过滤器

- 我们需要自定义一个过滤器，这个过滤器会：
  1. 去获取请求头中的token，对token进行解析取出其中的userid。
  2. 使用userid去redis中获取对应的LoginUser对象。
  3. 然后封装Authentication对象存入SecurityContextHolder





~~~~java
@Component
public class JwtAuthenticationTokenFilter extends OncePerRequestFilter {

    @Autowired
    private RedisCache redisCache;

    @Override
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain) throws ServletException, IOException {
        //获取token
        String token = request.getHeader("token");
        if (!StringUtils.hasText(token)) {
            //放行
            filterChain.doFilter(request, response);
            return;
        }
        //解析token
        String userid;
        try {
            Claims claims = JwtUtil.parseJWT(token);
            userid = claims.getSubject();
        } catch (Exception e) {
            e.printStackTrace();
            throw new RuntimeException("token非法");
        }
        //从redis中获取用户信息
        String redisKey = "login:" + userid;
        LoginUser loginUser = redisCache.getCacheObject(redisKey);
        if(Objects.isNull(loginUser)){
            throw new RuntimeException("用户未登录");
        }
        //存入SecurityContextHolder
        //TODO 获取权限信息封装到Authentication中
        UsernamePasswordAuthenticationToken authenticationToken =
                new UsernamePasswordAuthenticationToken(loginUser,null,null);
        SecurityContextHolder.getContext().setAuthentication(authenticationToken);
        //放行
        filterChain.doFilter(request, response);
    }
}
~~~~

~~~~java
/**
 * @Author 三更  B站： https://space.bilibili.com/663528522
 */
@Configuration
public class SecurityConfig extends WebSecurityConfigurerAdapter {


    @Bean
    public PasswordEncoder passwordEncoder(){
        return new BCryptPasswordEncoder();
    }


    @Autowired
    JwtAuthenticationTokenFilter jwtAuthenticationTokenFilter;

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
                //关闭csrf
                .csrf().disable()
                //不通过Session获取SecurityContext
                .sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS)
                .and()
                .authorizeRequests()
                // 对于登录接口 允许匿名访问
                .antMatchers("/user/login").anonymous()
                // 除上面外的所有请求全部需要鉴权认证
                .anyRequest().authenticated();

        //把token校验过滤器添加到过滤器链中
        http.addFilterBefore(jwtAuthenticationTokenFilter, UsernamePasswordAuthenticationFilter.class);
    }

    @Bean
    @Override
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return super.authenticationManagerBean();
    }
}

~~~~



### 退出登录

```java
/**
 * @Author 三更  B站： https://space.bilibili.com/663528522
 */
@Service
public class LoginServiceImpl implements LoginServcie {

    @Autowired
    private AuthenticationManager authenticationManager;
    @Autowired
    private RedisCache redisCache;

    @Override
    public ResponseResult login(User user) {
        UsernamePasswordAuthenticationToken authenticationToken = new UsernamePasswordAuthenticationToken(user.getUserName(),user.getPassword());
        Authentication authenticate = authenticationManager.authenticate(authenticationToken);
        if(Objects.isNull(authenticate)){
            throw new RuntimeException("用户名或密码错误");
        }
        //使用userid生成token
        LoginUser loginUser = (LoginUser) authenticate.getPrincipal();
        String userId = loginUser.getUser().getId().toString();
        String jwt = JwtUtil.createJWT(userId);
        //authenticate存入redis
        redisCache.setCacheObject("login:"+userId,loginUser);
        //把token响应给前端
        HashMap<String,String> map = new HashMap<>();
        map.put("token",jwt);
        return new ResponseResult(200,"登陆成功",map);
    }

    @Override
    public ResponseResult logout() {
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        LoginUser loginUser = (LoginUser) authentication.getPrincipal();
        Long userid = loginUser.getUser().getId();
        redisCache.deleteObject("login:"+userid);
        return new ResponseResult(200,"退出成功");
    }
}
```



### 总结

![登陆与访问资源的执行流](Spring%20Security.assets/%E7%99%BB%E9%99%86%E4%B8%8E%E8%AE%BF%E9%97%AE%E8%B5%84%E6%BA%90%E7%9A%84%E6%89%A7%E8%A1%8C%E6%B5%81.jpg)



# 授权

## 授权基本流程

- 在SpringSecurity中，会使用默认的`FilterSecurityInterceptor`来进行权限校验。

  FilterSecurityInterceptor会从SecurityContextHolder获取其中的Authentication，然后获取其中的权限信息，并判断当前用户是否拥有访问当前资源所需的权限。



步骤如下：

1. 把当前登录用户的权限信息存入Authentication
2. 设置资源所需要的权限



## 限制访问资源所需权限

- SpringSecurity为我们提供了基于注解的权限控制方案，这也是我们项目中主要采用的方式。我们可以使用注解去指定访问对应的资源所需的权限。



1. 开启相关配置

   ```java
   @EnableGlobalMethodSecurity(prePostEnabled = true)
   ```

2. 使用对应的注解。@PreAuthorize

   ```java
   @RestController
   public class HelloController {
   
       @RequestMapping("/hello")
       @PreAuthorize("hasAuthority('test')")
       public String hello(){
           return "hello";
       }
   }
   ```

   

## 封装权限信息

- 先直接把权限信息写死封装到UserDetails中进行测试。

  合理的做法：从数据库查询用户及对应的权限信息



1. 修改LoginUser：

   ```java
   package com.sangeng.domain;
   
   import com.alibaba.fastjson.annotation.JSONField;
   import lombok.AllArgsConstructor;
   import lombok.Data;
   import lombok.NoArgsConstructor;
   import org.springframework.security.core.GrantedAuthority;
   import org.springframework.security.core.authority.SimpleGrantedAuthority;
   import org.springframework.security.core.userdetails.UserDetails;
   
   import java.util.Collection;
   import java.util.List;
   import java.util.stream.Collectors;
   
   /**
    * @Author 三更  B站： https://space.bilibili.com/663528522
    */
   @Data
   @NoArgsConstructor
   public class LoginUser implements UserDetails {
   
       private User user;
           
       //存储权限信息
       private List<String> permissions;
       
       
       public LoginUser(User user,List<String> permissions) {
           this.user = user;
           this.permissions = permissions;
       }
   
   
       //存储SpringSecurity所需要的权限信息的集合
       @JSONField(serialize = false)
       private List<GrantedAuthority> authorities;
   
       @Override
       public  Collection<? extends GrantedAuthority> getAuthorities() {
           if(authorities!=null){
               return authorities;
           }
           //把permissions中字符串类型的权限信息转换成GrantedAuthority对象存入authorities中
           authorities = permissions.stream().
                   map(SimpleGrantedAuthority::new)
                   .collect(Collectors.toList());
           return authorities;
       }
   
       @Override
       public String getPassword() {
           return user.getPassword();
       }
   
       @Override
       public String getUsername() {
           return user.getUserName();
       }
   
       @Override
       public boolean isAccountNonExpired() {
           return true;
       }
   
       @Override
       public boolean isAccountNonLocked() {
           return true;
       }
   
       @Override
       public boolean isCredentialsNonExpired() {
           return true;
       }
   
       @Override
       public boolean isEnabled() {
           return true;
       }
   }
   ```

2. 在UserDetailsServiceImpl中，将权限信息封装到LoginUser

   ```java
   package com.sangeng.service.impl;
   
   import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
   import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
   import com.baomidou.mybatisplus.extension.conditions.query.LambdaQueryChainWrapper;
   import com.sangeng.domain.LoginUser;
   import com.sangeng.domain.User;
   import com.sangeng.mapper.UserMapper;
   import org.springframework.beans.factory.annotation.Autowired;
   import org.springframework.security.core.userdetails.UserDetails;
   import org.springframework.security.core.userdetails.UserDetailsService;
   import org.springframework.security.core.userdetails.UsernameNotFoundException;
   import org.springframework.stereotype.Service;
   
   import java.util.ArrayList;
   import java.util.Arrays;
   import java.util.List;
   import java.util.Objects;
   
   /**
    * @Author 三更  B站： https://space.bilibili.com/663528522
    */
   @Service
   public class UserDetailsServiceImpl implements UserDetailsService {
   
       @Autowired
       private UserMapper userMapper;
   
       @Override
       public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
           LambdaQueryWrapper<User> wrapper = new LambdaQueryWrapper<>();
           wrapper.eq(User::getUserName,username);
           User user = userMapper.selectOne(wrapper);
           if(Objects.isNull(user)){
               throw new RuntimeException("用户名或密码错误");
           }
           //TODO 根据用户查询权限信息 添加到LoginUser中
           List<String> list = new ArrayList<>(Arrays.asList("test"));
           return new LoginUser(user,list);
       }
   }
   
   ```

   

## 从数据库查询权限信息

### RBAC 权限模型

RBAC权限模型（Role-Based Access Control）即：**基于角色的权限控制**。这是目前最常被开发者使用也是相对易用、通用权限模型。

![image-20220617121318783](Spring%20Security.assets/image-20220617121318783.png)



### 数据库

- 建表语句：

  ```sql
  
  CREATE DATABASE /*!32312 IF NOT EXISTS*/`sg_security` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;
  
  USE `sg_security`;
  
  /*Table structure for table `sys_menu` */
  
  DROP TABLE IF EXISTS `sys_menu`;
  
  CREATE TABLE `sys_menu` (
    `id` bigint(20) NOT NULL AUTO_INCREMENT,
    `menu_name` varchar(64) NOT NULL DEFAULT 'NULL' COMMENT '菜单名',
    `path` varchar(200) DEFAULT NULL COMMENT '路由地址',
    `component` varchar(255) DEFAULT NULL COMMENT '组件路径',
    `visible` char(1) DEFAULT '0' COMMENT '菜单状态（0显示 1隐藏）',
    `status` char(1) DEFAULT '0' COMMENT '菜单状态（0正常 1停用）',
    `perms` varchar(100) DEFAULT NULL COMMENT '权限标识',
    `icon` varchar(100) DEFAULT '#' COMMENT '菜单图标',
    `create_by` bigint(20) DEFAULT NULL,
    `create_time` datetime DEFAULT NULL,
    `update_by` bigint(20) DEFAULT NULL,
    `update_time` datetime DEFAULT NULL,
    `del_flag` int(11) DEFAULT '0' COMMENT '是否删除（0未删除 1已删除）',
    `remark` varchar(500) DEFAULT NULL COMMENT '备注',
    PRIMARY KEY (`id`)
  ) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COMMENT='菜单表';
  
  /*Table structure for table `sys_role` */
  
  DROP TABLE IF EXISTS `sys_role`;
  
  CREATE TABLE `sys_role` (
    `id` bigint(20) NOT NULL AUTO_INCREMENT,
    `name` varchar(128) DEFAULT NULL,
    `role_key` varchar(100) DEFAULT NULL COMMENT '角色权限字符串',
    `status` char(1) DEFAULT '0' COMMENT '角色状态（0正常 1停用）',
    `del_flag` int(1) DEFAULT '0' COMMENT 'del_flag',
    `create_by` bigint(200) DEFAULT NULL,
    `create_time` datetime DEFAULT NULL,
    `update_by` bigint(200) DEFAULT NULL,
    `update_time` datetime DEFAULT NULL,
    `remark` varchar(500) DEFAULT NULL COMMENT '备注',
    PRIMARY KEY (`id`)
  ) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COMMENT='角色表';
  
  /*Table structure for table `sys_role_menu` */
  
  DROP TABLE IF EXISTS `sys_role_menu`;
  
  CREATE TABLE `sys_role_menu` (
    `role_id` bigint(200) NOT NULL AUTO_INCREMENT COMMENT '角色ID',
    `menu_id` bigint(200) NOT NULL DEFAULT '0' COMMENT '菜单id',
    PRIMARY KEY (`role_id`,`menu_id`)
  ) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;
  
  /*Table structure for table `sys_user` */
  
  DROP TABLE IF EXISTS `sys_user`;
  
  CREATE TABLE `sys_user` (
    `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '主键',
    `user_name` varchar(64) NOT NULL DEFAULT 'NULL' COMMENT '用户名',
    `nick_name` varchar(64) NOT NULL DEFAULT 'NULL' COMMENT '昵称',
    `password` varchar(64) NOT NULL DEFAULT 'NULL' COMMENT '密码',
    `status` char(1) DEFAULT '0' COMMENT '账号状态（0正常 1停用）',
    `email` varchar(64) DEFAULT NULL COMMENT '邮箱',
    `phonenumber` varchar(32) DEFAULT NULL COMMENT '手机号',
    `sex` char(1) DEFAULT NULL COMMENT '用户性别（0男，1女，2未知）',
    `avatar` varchar(128) DEFAULT NULL COMMENT '头像',
    `user_type` char(1) NOT NULL DEFAULT '1' COMMENT '用户类型（0管理员，1普通用户）',
    `create_by` bigint(20) DEFAULT NULL COMMENT '创建人的用户id',
    `create_time` datetime DEFAULT NULL COMMENT '创建时间',
    `update_by` bigint(20) DEFAULT NULL COMMENT '更新人',
    `update_time` datetime DEFAULT NULL COMMENT '更新时间',
    `del_flag` int(11) DEFAULT '0' COMMENT '删除标志（0代表未删除，1代表已删除）',
    PRIMARY KEY (`id`)
  ) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COMMENT='用户表';
  
  /*Table structure for table `sys_user_role` */
  
  DROP TABLE IF EXISTS `sys_user_role`;
  
  CREATE TABLE `sys_user_role` (
    `user_id` bigint(200) NOT NULL AUTO_INCREMENT COMMENT '用户id',
    `role_id` bigint(200) NOT NULL DEFAULT '0' COMMENT '角色id',
    PRIMARY KEY (`user_id`,`role_id`)
  ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
  ```

- 查询SQL：

  ```sql
  SELECT 
  	DISTINCT m.`perms`
  FROM
  	sys_user_role ur
  	LEFT JOIN `sys_role` r ON ur.`role_id` = r.`id`
  	LEFT JOIN `sys_role_menu` rm ON ur.`role_id` = rm.`role_id`
  	LEFT JOIN `sys_menu` m ON m.`id` = rm.`menu_id`
  WHERE
  	user_id = 2
  	AND r.`status` = 0
  	AND m.`status` = 0
  ```

  

### 代码实现

- Menu类：

  ```java
  package com.sangeng.domain;
  
  import com.baomidou.mybatisplus.annotation.TableId;
  import com.baomidou.mybatisplus.annotation.TableName;
  import com.fasterxml.jackson.annotation.JsonInclude;
  import lombok.AllArgsConstructor;
  import lombok.Data;
  import lombok.NoArgsConstructor;
  
  import java.io.Serializable;
  import java.util.Date;
  
  /**
   * 菜单表(Menu)实体类
   *
   * @author makejava
   * @since 2021-11-24 15:30:08
   */
  @TableName(value="sys_menu")
  @Data
  @AllArgsConstructor
  @NoArgsConstructor
  @JsonInclude(JsonInclude.Include.NON_NULL)
  public class Menu implements Serializable {
      private static final long serialVersionUID = -54979041104113736L;
      
          @TableId
      private Long id;
      /**
      * 菜单名
      */
      private String menuName;
      /**
      * 路由地址
      */
      private String path;
      /**
      * 组件路径
      */
      private String component;
      /**
      * 菜单状态（0显示 1隐藏）
      */
      private String visible;
      /**
      * 菜单状态（0正常 1停用）
      */
      private String status;
      /**
      * 权限标识
      */
      private String perms;
      /**
      * 菜单图标
      */
      private String icon;
      
      private Long createBy;
      
      private Date createTime;
      
      private Long updateBy;
      
      private Date updateTime;
      /**
      * 是否删除（0未删除 1已删除）
      */
      private Integer delFlag;
      /**
      * 备注
      */
      private String remark;
  }
  ```

- Mapper接口

  ```java
  import com.baomidou.mybatisplus.core.mapper.BaseMapper;
  import com.sangeng.domain.Menu;
  
  import java.util.List;
  
  /**
   * @Author 三更  B站： https://space.bilibili.com/663528522
   */
  public interface MenuMapper extends BaseMapper<Menu> {
      List<String> selectPermsByUserId(Long id);
  }
  ```

- 对应的Mapper文件：

  ```xml
  <?xml version="1.0" encoding="UTF-8" ?>
  <!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
  <mapper namespace="com.sangeng.mapper.MenuMapper">
  
  
      <select id="selectPermsByUserId" resultType="java.lang.String">
          SELECT
              DISTINCT m.`perms`
          FROM
              sys_user_role ur
              LEFT JOIN `sys_role` r ON ur.`role_id` = r.`id`
              LEFT JOIN `sys_role_menu` rm ON ur.`role_id` = rm.`role_id`
              LEFT JOIN `sys_menu` m ON m.`id` = rm.`menu_id`
          WHERE
              user_id = #{userid}
              AND r.`status` = 0
              AND m.`status` = 0
      </select>
  </mapper>
  ```

- 在application.yml中配置mapperXML文件的位置：

  ```yaml
  spring:
    datasource:
      url: jdbc:mysql://localhost:3306/sg_security?characterEncoding=utf-8&serverTimezone=UTC
      username: root
      password: root
      driver-class-name: com.mysql.cj.jdbc.Driver
    redis:
      host: localhost
      port: 6379
  mybatis-plus:
    mapper-locations: classpath*:/mapper/**/*.xml 
  ```

- 在UserDetailsServiceImpl中去调用该mapper的方法查询权限信息封装到LoginUser对象中：

  ```java
  /**
   * @Author 三更  B站： https://space.bilibili.com/663528522
   */
  @Service
  public class UserDetailsServiceImpl implements UserDetailsService {
  
      @Autowired
      private UserMapper userMapper;
  
      @Autowired
      private MenuMapper menuMapper;
  
      @Override
      public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
          LambdaQueryWrapper<User> wrapper = new LambdaQueryWrapper<>();
          wrapper.eq(User::getUserName,username);
          User user = userMapper.selectOne(wrapper);
          if(Objects.isNull(user)){
              throw new RuntimeException("用户名或密码错误");
          }
          List<String> permissionKeyList =  menuMapper.selectPermsByUserId(user.getId());
  //        //测试写法
  //        List<String> list = new ArrayList<>(Arrays.asList("test"));
          return new LoginUser(user,permissionKeyList);
      }
  }
  ```



# 自定义失败处理

[SpringSecurity框架教程-Spring Security+JWT实现项目级前端分离认证授权-B站最通俗易懂的Spring Security课程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1mm4y1X7Hc?p=31&vd_source=be746efb77e979ca275e4f65f2d8cda3)
