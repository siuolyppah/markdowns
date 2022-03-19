# 相关网址

官方地址: http://mp.baomidou.com

代码发布地址:

Github: https://github.com/baomidou/mybatis-plus

Gitee: https://gitee.com/baomidou/mybatis-plus

文档发布地址: https://baomidou.com/pages/24112f



# Mybatis-plus框架结构

![Screenshot 2022-03-19 at 15-21-47 MyBatisPlus（SpringBoot版）.pdf](mybatis-plus.assets/Screenshot%202022-03-19%20at%2015-21-47%20MyBatisPlus%EF%BC%88SpringBoot%E7%89%88%EF%BC%89.pdf.png)



# 入门Demo

## 开发环境

IDE：idea 2019.2

JDK：JDK8+

构建工具：maven 3.5.4

MySQL版本：MySQL 5.7



## 创建数据库及表

```sql
CREATE DATABASE `mybatis_plus`
/*!40100 DEFAULT CHARACTER SET utf8mb4 */
;

use `mybatis_plus`;

CREATE TABLE `user` (
    `id` bigint(20) NOT NULL COMMENT '主键ID',
    `name` varchar(30) DEFAULT NULL COMMENT '姓名',
    `age` int(11) DEFAULT NULL COMMENT '年龄',
    `email` varchar(50) DEFAULT NULL COMMENT '邮箱',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB DEFAULT CHARSET = utf8;

INSERT INTO
    user (id, name, age, email)
VALUES
    (1, 'Jone', 18, 'test1@baomidou.com'),
    (2, 'Jack', 20, 'test2@baomidou.com'),
    (3, 'Tom', 28, 'test3@baomidou.com'),
    (4, 'Sandy', 21, 'test4@baomidou.com'),
    (5, 'Billie', 24, 'test5@baomidou.com');
```





## 创建SpringBoot工程

1. 使用Spring Initializr创建，Spring Boot版本2.6.3

2. 引入依赖

    ```xml
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>com.baomidou</groupId>
            <artifactId>mybatis-plus-boot-starter</artifactId>
            <version>3.5.1</version>
        </dependency>
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
    </dependencies>
    ```

3. 在IDEA中安装lombok插件



## 编写代码

1. 配置application.yml

    ```yaml
    spring:
      datasource:
        type: com.zaxxer.hikari.HikariDataSource
        driver-class-name: com.mysql.cj.jdbc.Driver
        url: jdbc:mysql://localhost:3306/mybatis_plus?characterEncoding=utf-8&useSSL=false
        username: root
        password: 123456
    ```

    >- spring boot 2.0（内置jdbc5驱动），驱动类使用：
    >
    >    driver-class-name: com.mysql.jdbc.Driver
    >
    >- spring boot 2.1及以上（内置jdbc8驱动），驱动类使用：
    >
    >    driver-class-name: com.mysql.cj.jdbc.Driver
    >
    >- MySQL5.7版本的url：
    >
    >    jdbc:mysql://localhost:3306/mybatis_plus?characterEncoding=utf-8&useSSL=false
    >
    >- MySQL8.0版本的url：
    >
    >    jdbc:mysql://localhost:3306/mybatis_plus?serverTimezone=GMT%2B8&characterEncoding=utf-8&useSSL=false

2. 创建实体类：

    ```java
    import lombok.Data;
    
    @Data //lombok注解
    public class User {
        private Long id;
        private String name;
        private Integer age;
        private String email;
    }
    ```

    > @Data注解，将自动生成Getter/Setter，equals，hashCode，toString，无参构造器

3. 创建Mapper接口：

    ```java
    @Repository
    public interface UserMapper extends BaseMapper<User> {
    }
    ```

    > 添加@Repository注解，即可解决IDEA报错：没有可匹配类型的Bean

4. 指定包扫描

    ```java
    @SpringBootApplication
    @MapperScan("com.example.mybatisplus.mapper")
    public class MybatisplusApplication {
        public static void main(String[] args) {
            SpringApplication.run(MybatisplusApplication.class, args);
        }
    }
    ```

5. 测试：

    ```java
    @SpringBootTest
    public class UserTest {
    
        @Autowired
        private UserMapper userMapper;
    
        @Test
        public void testSelectList(){
            //通过条件构造器queryWrapper进行查询，封装结果为list
            List<User> users = userMapper.selectList(null);
            System.out.println(users);
        }
    }
    ```

    

## 配置日志功能

```yaml
mybatis-plus:
  configuration:
    log-impl: org.apache.ibatis.logging.stdout.StdOutImpl
```



# BaseMapper\<T>接口

## 新增功能

![image-20220319153417591](mybatis-plus.assets/image-20220319153417591.png)



### insert(T entity)

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void testInsert(){
        User user =new User();
        user.setName("张三");
        user.setAge(23);
        user.setEmail("abc@qq.com");

        //INSERT INTO user ( id, name, age, email ) VALUES ( ?, ?, ?, ? )
        int result = userMapper.insert(user);
        System.out.println(result);
        System.out.println(user);
    }
}

```

> 在插入记录前，将根据默认的雪花算法，设置user对象的id属性



##删除功能

![image-20220319153355984](mybatis-plus.assets/image-20220319153355984.png)



### deleteById(Serializable id)方法

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void deleteByID(){
        //DELETE FROM user WHERE id=?
        int result = userMapper.deleteById(1505084180572893186L);
        System.out.println(result);
    }
}
```


### deleteByMap(Map<String, Object> columnMap)法

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void deleteByMap(){
        //DELETE FROM user WHERE name = ? AND age = ?
        Map<String,Object> map = new HashMap<>();
        map.put("name","张三");
        map.put("age",23);

        int result = userMapper.deleteByMap(map);
        System.out.println(result);
    }
}
```





### deleteBatchIds(Collection<?> idList)

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void deleteByBatchIDs(){
        List<Long> idList = Arrays.asList(1L, 2L, 3L);

        // Preparing: DELETE FROM user WHERE id IN ( ? , ? , ? )
        //Parameters: 1(Long), 2(Long), 3(Long)
        int result = userMapper.deleteBatchIds(idList);
        System.out.println(result);
    }
}
```



## 修改功能

![image-20220319154348397](mybatis-plus.assets/image-20220319154348397.png)



### updateById(T entity)

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;
    
    @Test
    public void testUpdateByID(){
        User user = new User();
        user.setId(4L);
        user.setName("lisi");
        user.setEmail("lisi@qq.com");

        //UPDATE user SET name=?, email=? WHERE id=?
        int result = userMapper.updateById(user);
        System.out.println(result);
    }
}
```



## 查询功能

![image-20220319155247824](mybatis-plus.assets/image-20220319155247824.png)



### T selectById(Serializable id)

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void selectByID(){
        //SELECT id,name,age,email FROM user WHERE id=?
        User user = userMapper.selectById(1L);
        System.out.println(user);
    }
}
```



### List\<T> selectBatchIds(Collection<? extends Serializable> idList)

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void selectByBatchIDs(){
        List<Long> idList = Arrays.asList(1L, 2L, 3L);
        
        // Preparing: SELECT id,name,age,email FROM user WHERE id IN ( ? , ? , ? )
        //Parameters: 1(Long), 2(Long), 3(Long)
        List<User> users = userMapper.selectBatchIds(idList);
        users.forEach(System.out::println);
    }
}
```



### List\<T> selectByMap(Map<String, Object> columnMap)

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;

    @Test
    public void selectByMap(){
        Map<String,Object> map = new HashMap<>();
        map.put("name","张三");
        map.put("age",23);
        
        // Preparing: SELECT id,name,age,email FROM user WHERE name = ? AND age = ?
        //Parameters: 张三(String), 23(Integer)
        List<User> users = userMapper.selectByMap(map);
        users.forEach(System.out::println);
    }
}
```



### List\<T> selectList(Wrapper\<T> queryWrapper)

```java
@SpringBootTest
public class UserTest {

    @Autowired
    private UserMapper userMapper;
    
    @Test
    public void selectByList(){
        //SELECT id,name,age,email FROM user
        List<User> users = userMapper.selectList(null);
        users.forEach(System.out::println);
    }
}
```



# 自定义功能

1. 配置SQL映射文件的扫描位置：

    ```yaml
    mybatis-plus:
      configuration:
        log-impl: org.apache.ibatis.logging.stdout.StdOutImpl
      mapper-locations: 
        - classpath*:/mapper/**/*.xml #即默认值
    
    ```

    >- 启动类上的@MapperScan()，是用于扫描Mapper接口的
    >
    >- mapper-locations配置，是用于扫描SQL映射文件的

2. 编写SQL映射文件：

    ```xml
    <?xml version="1.0" encoding="utf-8" ?>
    <!DOCTYPE mapper
            PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
            "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
    <mapper namespace="com.example.mybatisplus.mapper.UserMapper">
        <select id="selectMapByID" resultType="map">
            SELECT id,name,age,email FROM user WHERE id = #{id}
        </select>
    </mapper>
    ```

3. 测试：

    ```java
    @SpringBootTest
    public class UserTest {
    
        @Autowired
        private UserMapper userMapper;
    
        @Test
        public void testCustomMapper(){
            Map<String, Object> map = userMapper.selectMapByID(1L);
            System.out.println(map);
        }
    }
    ```

    

# IService\<T>接口

> 该接口的实现类：
>
> ```java
> public class ServiceImpl<M extends BaseMapper<T>, T> implements IService<T>{}
> ```
>
> - M：Mapper接口，通常应该继承BaseMapper\<T>
> - T：实例类类型



步骤如下：

1. 创建Service接口：

    ```java
    public interface UserService extends IService<User> {
    }
    ```

2. 创建Service接口的实现类：

    ```java
    @Service
    public class UserServiceImpl  extends ServiceImpl<UserMapper, User>implements UserService{
    }
    ```

    > 其中UserMapper：
    >
    > ```java
    > public interface UserMapper extends BaseMapper<User> {}
    > ```

3. 编写自定义Service层业务



## 查询记录数量

### long count()

```java
@SpringBootTest
public class UserServiceTest {

    @Autowired
    private UserService userService;

    @Test
    public void testRecordsCount(){
        //SELECT COUNT( * ) FROM user
        long count = userService.count();
        System.out.println(count);
    }
}
```



## 批量添加

### boolean saveBatch(Collection<T> entityList)

```java
@SpringBootTest
public class UserServiceTest {

    @Autowired
    private UserService userService;

    @Test
    public void testBatchInsert(){
        List<User> list = new ArrayList<>();
        for (int i = 1;i <= 10;i++){
            User user = new User();
            user.setName("name_"+i);
            user.setAge(20+i);
            list.add(user);
        }

        // Preparing: INSERT INTO user ( id, name, age ) VALUES ( ?, ?, ? )
        //Parameters: 1505102000320888834(Long), name_1(String), 21(Integer)
        //Parameters: 1505102000392192001(Long), name_2(String), 22(Integer)
        //...
        boolean b = userService.saveBatch(list);
        System.out.println(b);
    }
}
```



# 常用注解

https://www.bilibili.com/video/BV12R4y157Be?p=21&spm_id_from=pageDriver