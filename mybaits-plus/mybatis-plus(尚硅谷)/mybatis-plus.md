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

## @TableName注解

> 在默认情况下，由BaseMapper\<T>的T决定要操作的表名，即默认为实体类类名小写



当表名与实体类类名不一致时，可采用`@TableName`注解

```java
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

@TableName("tb_user")
@Data //lombok注解
public class User {
    private Long id;
    private String name;
    private Integer age;
    private String email;
}
```



> 特别的，也可以选择为所有的表名添加前缀：
>
> ```yaml
> mybatis-plus:
>   global-config:
>     db-config:
>       table-prefix: tb_
> ```



## @TableID注解

通过@TableId注解，可以指定哪个POJO属性将来作为数据表的主键字段

- value属性：用于指明属性对应的数据表字段的名称

- type属性：主键的生成策略:

    - IdType.AUTO：主键自增时使用
    - IdType.NONE
    - IdType.ASSIGN_ID：==仅在id属性为空时==，使用雪花算法生成ID（在插入记录前为对象设置属性）

    > 例如，如果想要主键自增：
    >
    > 1. 数据库的主键设置为自增
    > 2. type设置为IdType.AUTO

```java
public @interface TableId {

    /**
     * 字段名（该值可无）
     */
    String value() default "";

    /**
     * 主键类型
     * {@link IdType}
     */
    IdType type() default IdType.NONE;
}
```



使用方法如下：

```java
@Data //lombok注解
public class User {
    
    @TableId(value = "uid") //数据表中的主键名为uid
    private Long id;
    
    private String name;
    private Integer age;
    private String email;
}
```

> 也可以配置全局配置：
>
> ```yaml
> mybatis-plus:
>   global-config:
>     id-type: auto
> ```





### 雪花算法

- 背景：

    需要选择合适的方案去应对数据规模的增长，以应对逐渐增长的访问压力和数据量。

    数据库的扩展方式主要包括：业务分库、主从复制，数据库分表



其中**数据库分表策略**：将不同业务数据分散存储到不同的数据库服务器，能够支撑百万甚至千万用户规模的业务，但如果业务继续发展，同一业务的单表数据也会达到单台数据库服务器的处理瓶颈。

例如，淘宝的几亿用户数据，如果全部存放在一台数据库服务器的一张表中，肯定是无法满足性能要求的，此时就需要对单表数据进行拆分。



![image-20220319173528559](mybatis-plus.assets/image-20220319173528559.png)

- 垂直拆分：垂直分表适合将表中某些不常用且占了大量空间的列拆分出去

- 水平拆分：适合表行数特别大的表

    水平分表相比垂直分表，会引入更多的复杂性。例如要求全局唯一的数据id该如何处理



水平拆分引起的问题，常见的解决策略：

- **为每张表设置主键区间**：

    以最常见的用户 ID 为例，可以按照 1000000 的范围大小进行分段，1 ~ 999999 放到表 1中，1000000 ~ 1999999 放到表2中，以此类推。

    - 复杂点：分段大小的选取。分段太小会导致切分后子表数量过多，增加维护复杂度；分段太大可能会导致单表依然存在性能问题，一般建议分段大小在 100 万至 2000 万之间，具体需要根据业务选取合适的分段大小。
    - 优点：可以随着数据的增加平滑地扩充新的表。例如，现在的用户是 100 万，如果增加到 1000 万，只需要增加新的表就可以了，原有的数据不需要动。
    - 缺点：分布不均匀。假如按照 1000 万来进行分表，有可能某个分段实际存储的数据量只有 1 条，而另外一个分段实际存储的数据量有 1000 万条。

- **取模**：

    同样以用户 ID 为例，假如我们一开始就规划了 10 个数据库表，可以简单地用 user_id % 10 的值来表示数据所属的数据库表编号，ID 为 985 的用户放到编号为 5 的子表中，ID 为 10086 的用户放到编号为 6 的子表中

    - 复杂点：初始表数量的确定。表数量太多维护比较麻烦，表数量太少又可能导致单表性能存在问题。
    - 优点：表分布比较均匀。
    - 缺点：扩充新的表很麻烦，所有数据都要重分布。

- **雪花算法**：

    雪花算法是由Twitter公布的==分布式主键生成算法==，它能够保证不同表的主键的不重复性，以及相同表的主键的有序性。



核心思想：

![image-20220319180257772](mybatis-plus.assets/image-20220319180257772.png)

- 1bit符合位：一般是0，即正数
- 41bit时间戳：存储的是时间截的差值（当前时间截 - 开始时间截)，结果约等于69.73年
- 1obit作为机器的ID（5个bit是数据中心，5个bit的机器ID，可以部署在1024个节点）
- 12bit作为毫秒内的流水号（意味着每个节点在每毫秒可以产生 4096 个 ID）



优点：整体上按照时间自增排序，并且整个分布式系统内不会产生ID碰撞，并且效率较高



## @TableField注解

类似于@TableId注解：

```java
@TableName("user")
@Data //lombok注解
public class User {
    private Long id;
    
    @TableField("username")
    private String name;
    private Integer age;
    private String email;
}
```



## @TableLogic注解

### 逻辑删除

- 物理删除：真实删除，对应的数据将从数据库中被删除

- 逻辑删除：假删除，仅设置“被删除状态”字段。

    从而实现数据恢复

### 实现逻辑删除

1. 在数据库的数据表中，创建逻辑删除状态列，设置默认值为0

    ![image-20220319181437416](mybatis-plus.assets/image-20220319181437416-16476848791311.png)

2. 在实体类中相应的属性，添加@TableLogic注解

    ```java
    @TableName("user")
    @Data //lombok注解
    public class User {
        private Long id;
        private String name;
        private Integer age;
        private String email;
        
        @TableLogic
        private Integer isDeleted;
    }
    ```

    

- 执行删除方法：

    ```java
    @SpringBootTest
    public class UserDaoTest {
    
        @Autowired
        private UserMapper userMapper;
    
        @Test
        public void deleteByID(){
            //UPDATE user SET is_deleted=1 WHERE id=? AND is_deleted=0
            int result = userMapper.deleteById(1505102000320888834L);
            System.out.println(result);
        }
    }
    ```

    > 处于逻辑删除状态的记录，将不会被Mybatis-plus查询到；
    > **需要手动编写SQL语句**


​    

# 条件构造器Wrapper

https://www.bilibili.com/video/BV12R4y157Be?p=29&spm_id_from=pageDriver
