# 相关资料

- [视频教程](https://www.bilibili.com/video/BV1wy4y1H7wu?p=4&vd_source=be746efb77e979ca275e4f65f2d8cda3)
- 官网：https://mybatis.org/mybatis-3/zh/#



# 快速入门

- 数据库：

  ```sql
  CREATE DATABASE /*!32312 IF NOT EXISTS*/`mybatis_db` /*!40100 DEFAULT CHARACTER SET utf8 */;
  USE `mybatis_db`;
  DROP TABLE IF EXISTS `user`;
  CREATE TABLE `user` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `username` varchar(50) DEFAULT NULL,
    `age` int(11) DEFAULT NULL,
    `address` varchar(50) DEFAULT NULL,
    PRIMARY KEY (`id`)
  ) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
  insert  into `user`(`id`,`username`,`age`,`address`) values (1,'UZI',19,'上海'),(2,'PDD',25,'上海');
  ```

- pom.xml：

  ```xml
  <dependencies>
      <dependency>
          <groupId>org.mybatis</groupId>
          <artifactId>mybatis</artifactId>
          <version>3.5.4</version>
      </dependency>
  
      <dependency>
          <groupId>mysql</groupId>
          <artifactId>mysql-connector-java</artifactId>
          <version>8.0.28</version>
      </dependency>
  
      <dependency>
          <groupId>org.projectlombok</groupId>
          <artifactId>lombok</artifactId>
          <version>1.18.22</version>
          <optional>true</optional>
      </dependency>
  </dependencies>
  ```

- 在resources目录下创建：`mybatis-config.xml`核心配置文件：

  ```xml
  <?xml version="1.0" encoding="UTF-8" ?>
  <!DOCTYPE configuration
          PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
          "http://mybatis.org/dtd/mybatis-3-config.dtd">
  <configuration>
      <environments default="development">
          <environment id="development">
              <transactionManager type="JDBC"/>
              <dataSource type="POOLED">
                  <property name="driver" value="com.mysql.jdbc.Driver"/>
                  <property name="url" value="jdbc:mysql://localhost:3306/mybatis_db"/>
                  <property name="username" value="root"/>
                  <property name="password" value="gg12138."/>
              </dataSource>
          </environment>
      </environments>
  
      <mappers>
          <mapper resource="org/example/dao/UserDao.xml"></mapper>
      </mappers>
  </configuration>
  ```

- User类：

  ```java
  @Data
  public class User {
  
      private Integer id;
      private String username;
      private Integer age;
      private String address;
  }
  ```

- 定义dao接口（org.example.dao）：

  ```java
  public interface UserDao {
      List<User> findAll();
  }
  ```

- 定义对应的xml映射文件（org/example/dao/UserDao.xml）：

  ```xml
  <?xml version="1.0" encoding="UTF-8" ?>
  <!DOCTYPE mapper
          PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
          "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
  <mapper namespace="org.example.dao.UserDao">
  
      <select id="findAll" resultType="org.example.pojo.User">
          select * from user
      </select>
  </mapper>
  ```

- 测试类：

  ```java
  public class Demo {
  
      public static void main(String[] args) throws IOException {
          String resource = "mybatis-config.xml";
          InputStream inputStream = Resources.getResourceAsStream(resource);
          SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
  
          try (SqlSession session = sqlSessionFactory.openSession()) {
              UserDao userDao = session.getMapper(UserDao.class);
  
              List<User> users = userDao.findAll();
              users.forEach(System.out::println);
          }
      }
  }
  ```



# 参数获取

## 一个参数

### 基本参数

- 形参写任意名字，都可以在配置文件中获取到参数。

  但一般最好统一。



例如：

- 接口：

  ```java
  User findById(Integer id);
  ```

- xml：

  ```xml
  <select id="findById" resultType="com.sangeng.pojo.User">  select * from user where id = #{id}</select>
  ```



### POJO

可以使用POJO中的属性名来获取对应的值



例如：

- 接口：

  ```java
  User findByUser(User user);
  ```

- xml：

  ```xml
  <select id="findByUser" resultType="com.sangeng.pojo.User">
      select * from user where id = #{id} and username = #{username} and age = #{age} and address = #{address}
  </select>
  ```



### Map

使用map中的key来获取对应的值



例如：

- 接口：

  ```java
  User findByMap(Map map);
  ```

- xml：

  ```xml
  <select id="findByMap" resultType="com.sangeng.pojo.User">
      select * from user where id = #{id} and username = #{username} and age = #{age} and address = #{address}
  </select>
  ```

- 方法调用：

  ```java
  Map map = new HashMap();
  map.put("id",2);
  map.put("username","PDD");
  map.put("age",25);
  map.put("address","上海");
  userDao.findByMap(map);
  ```

  

## 多个参数

Mybatis会把多个参数放入一个Map集合中，默认的key是argx和paramx这种格式。

例如：

- 接口中方法定义如下：

  ```java
  User findByCondition(Integer id,String username);
  ```

- 最终map中的键值对如下：

  ```
  {arg1=PDD, arg0=2, param1=2, param2=PDD}
  ```



虽然可以使用对应的默认key来获取值，但是这种方式可读性不好。

**我们一般在方法参数前使用@Param来设置参数名**。

例如：

- 接口中方法定义：

  ```java
  User findByCondition(@Param("id") Integer id,@Param("username") String username);
  ```

- 最终map中的键值对如下：

  ```
  {id=2, param1=2, username=PDD, param2=PDD}
  ```

  

# 核心类

## SqlSessionFactory

SqlSessionFactory是一个SqlSession的工厂类。主要用来获取SqlSession对象。、

成员方法如下：

```java
SqlSession openSession();

//获取SqlSession对象，传入的参数代表创建的SqlSession是否自动提交
SqlSession openSession(boolean autoCommit);
```



## SqlSession

SqlSession 提供：

- 在数据库执行 SQL 命令所需的所有方法
- 事务的相关操作



# 核心配置文件详解

## properties

可以在核心配置文件中配置properties选项以读入properties配置文件。

之后可以使用${key}的方法获取配置文件的值。



例如：

- 在resources目录下有jdbc.properties文件，内容如下：

  ```properties
  jdbc.url=jdbc:mysql://localhost:3306/mybatis_db
  jdbc.driver=com.mysql.jdbc.Driver
  jdbc.username=root
  jdbc.password=root
  ```

- 在mybatis-config.xml中：

  ```xml
  <?xml version="1.0" encoding="UTF-8" ?>
  <!DOCTYPE configuration
          PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
          "http://mybatis.org/dtd/mybatis-3-config.dtd">
  <configuration>
      <!--设置配置文件所在的路径-->
      <properties resource="jdbc.properties"></properties>
      <environments default="development">
          <environment id="development">
              <transactionManager type="JDBC"/>
              <dataSource type="POOLED">
                  <!--获取配置文件中配置的对应的值来设置连接相关参数-->
                  <property name="driver" value="${jdbc.driver}"/>
                  <property name="url" value="${jdbc.url}"/>
                  <property name="username" value="${jdbc.username}"/>
                  <property name="password" value="${jdbc.password}"/>
              </dataSource>
          </environment>
      </environments>
  </configuration>
  ```

  

## settings

可以使用该标签来设置进行一些设置。



例如：

```xml
<settings>
    <!--开启自动驼峰命名映射-->
    <setting name="mapUnderscoreToCamelCase" value="true"/>
</settings>
```

> 具体的设置[参考](https://mybatis.org/mybatis-3/zh/configuration.html#settings)



## typeAliases

- 类型别名可为 Java 类型设置一个缩写名字。 

- 它仅用于 XML 配置，意在降低冗余的全限定类名书写。

  

例如：

```xml
<typeAliases>
  <typeAlias alias="Author" type="domain.blog.Author"/>
  <typeAlias alias="Blog" type="domain.blog.Blog"/>
</typeAliases>
```

当这样配置时，`Blog` 可以用在任何使用 `domain.blog.Blog` 的地方。



也可以指定**包名**：
```xml
<typeAliases>
  <package name="domain.blog"/>
</typeAliases>
```

- domain.blog包下的JavaBean，若没有注解，采用Bean的首字母小写作为别名：

  domain.blog.Author->author

- 若有注解，为注解值：

  ```java
  @Alias("author")
  public class Author {
      ...
  }
  ```



## environments

配置数据库相关的环境，例如事物管理器，连接池相关参数等。

```xml
<!--设置默认环境-->
<environments default="development">

    <!--设置该环境的唯一标识-->
    <environment id="development">
        <transactionManager type="JDBC"/>
        <dataSource type="POOLED">
            <!--获取配置文件中配置的对应的值来设置连接相关参数-->
            <property name="driver" value="${jdbc.driver}"/>
            <property name="url" value="${jdbc.url}"/>
            <property name="username" value="${jdbc.username}"/>
            <property name="password" value="${jdbc.password}"/>
        </dataSource>
    </environment>
</environments>
```



## mappers

该标签的作用是加载映射的，加载方式有如下几种(**主要使用第四种**)：

- 使用相对于类路径的资源引用，例如：

  ```xml
  <!-- 使用相对于类路径的资源引用 -->
  <mappers>
    <mapper resource="org/mybatis/builder/AuthorMapper.xml"/>
    <mapper resource="org/mybatis/builder/BlogMapper.xml"/>
    <mapper resource="org/mybatis/builder/PostMapper.xml"/>
  </mappers>
  ```

- 使用完全限定资源定位符（URL），例如：

  ```xml
  <!-- 使用完全限定资源定位符（URL） -->
  <mappers>
    <mapper url="file:///var/mappers/AuthorMapper.xml"/>
    <mapper url="file:///var/mappers/BlogMapper.xml"/>
    <mapper url="file:///var/mappers/PostMapper.xml"/>
  </mappers>
  ```

- 使用映射器接口实现类的完全限定类名，例如：

  ```xml
  <!-- 使用映射器接口实现类的完全限定类名 -->
  <mappers>
    <mapper class="org.mybatis.builder.AuthorMapper"/>
    <mapper class="org.mybatis.builder.BlogMapper"/>
    <mapper class="org.mybatis.builder.PostMapper"/>
  </mappers>
  ```

- 将包内的映射器接口实现全部注册为映射器，例如：

  ```xml
  <!-- 定义dao接口所在的包。要求xml文件存放的路径和dao接口的包名要对应 -->
  <mappers>
    <package name="org.mybatis.builder"/>
  </mappers>
  ```

  

# 打印日志

1. log4j依赖：

   ```xml
   <dependency>
       <groupId>log4j</groupId>
       <artifactId>log4j</artifactId>
       <version>1.2.17</version>
   </dependency>
   ```

2. log4j配置：在resources目录下创建log4j.properties文件：

   ```properties
   ### direct log messages to stdout ###
   log4j.appender.stdout=org.apache.log4j.ConsoleAppender
   log4j.appender.stdout.Target=System.out
   log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
   log4j.appender.stdout.layout.ConversionPattern=%d{ABSOLUTE} %5p %c{1}:%L - %m%n
   
   ### direct messages to file mylog.log ###
   log4j.appender.file=org.apache.log4j.FileAppender
   log4j.appender.file.File=c:/mylog.log
   log4j.appender.file.layout=org.apache.log4j.PatternLayout
   log4j.appender.file.layout.ConversionPattern=%d{ABSOLUTE} %5p %c{1}:%L - %m%n
   
   ### set log levels - for more verbose logging change 'info' to 'debug' ###
   
   log4j.rootLogger=debug, stdout
   ```

   

# 获取参数时 #{} 和 ${} 的区别

[官方文档](https://mybatis.org/mybatis-3/zh/sqlmap-xml.html#%E7%BB%93%E6%9E%9C%E6%98%A0%E5%B0%84)

- `#{}`：
  - 预编译SQL
- `${}`
  - 直接把参数值拿来进行拼接，有SQL注入的危险
  - 可以用于替换：
    - 表名、列名



# 注解开发

1. 在核心配置文件中配置mapper接口所在的包名：

   ```xml
   <mappers>
       <package name="org.example.dao"/>
   </mappers>
   ```

2. 在接口对应方法上使用注解来配置需要执行的sql：

   ```java
   public interface UserDao {
   
       // 此方法用mapper映射文件实现
       List<User> findAll();
   
       @Select("select  * from user where id = #{id}")
       User getUser(Integer id);
   }
   ```

3. 获取Mapper代理对象，执行方法：

   ```java
   public class Demo {
   
       public static void main(String[] args) throws IOException {
           String resource = "mybatis-config.xml";
           InputStream inputStream = Resources.getResourceAsStream(resource);
           SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
   
           try (SqlSession session = sqlSessionFactory.openSession()) {
               UserDao userDao = session.getMapper(UserDao.class);
   
               User user = userDao.getUser(1);
               System.out.println(user);
           }
       }
   }
   ```

   

# ResultMap

## 基本使用

可以使用resultMap标签自定义结果集和实体类属性的映射规则：

```xml
<!--
        resultMap 用来自定义结果集和实体类的映射
            属性：
                id 相当于这个resultMap的唯一标识
                type 用来指定映射到哪个实体类
        id标签  用来指定主键列的映射规则
            属性：
                property 要映射的属性名
                column  对应的列名
        result标签 用来指定普通列的映射规则
            属性：
                property 要映射的属性名
                column 对应的列名
    -->
<resultMap id="orderMap" type="com.sangeng.pojo.Order" >
    <id column="id" property="id"></id>
    <result column="createtime" property="createtime"></result>
    <result column="price" property="price"></result>
    <result column="remark" property="remark"></result>
    <result column="user_id" property="userId"></result>
</resultMap>

<!--使用我们自定义的映射规则-->
<select id="findAll" resultMap="orderMap">
    SELECT id,createtime,price,remark,user_id  FROM ORDERS
</select>
```



## 自动映射

- 默认情况下自动映射是开启状态的。

- 即如果结果集的列名，和属性名相同会自动映射。

  只需写特殊情况的映射关系即可。

```xml
<resultMap id="orderMap" type="com.sangeng.pojo.Order" >
    <result column="user_id" property="userId"></result>
</resultMap>

<!--使用我们自定义的映射规则-->
<select id="findAll" resultMap="orderMap">
    SELECT id,createtime,price,remark,user_id  FROM ORDERS
</select>
```



以选择关闭自动映射可以把resultMap的autoMapping属性设置为false：

```xml
<resultMap id="xxx" type="xxx" autoMapping="false">
    ...
</resultMap>
```



# 结果封装

## Association



## Collection





# Mybatis缓存

Mybatis的缓存其实就是把之前查到的数据存入内存（map）,下次如果还是查相同的东西，就可以直接从缓存中取，从而提高效率。

Mybatis有一级缓存和二级缓存之分：

- 一级缓存（默认开启）是sqlsession级别的缓存。
- 二级缓存相当于mapper级别的缓存。



## 一级缓存

几种不会使用一级缓存的情况：

- 调用相同方法但是传入的参数不同
- 调用相同方法参数也相同，但是使用的是另外一个SqlSession
- 如果查询完后，对同一个表进行了增，删改的操作，都会清空这sqlSession上的缓存
- 手动调用SqlSession的clearCache方法清除缓存



## 二级缓存

> ##### 💡注意
>
> 只有sqlsession调用了close或者commit后，数据才会进入二级缓存。



### 开启二级缓存

- 全局开启：

  在Mybatis核心配置文件中配置：

  ```xml
  <settings>
      <setting name="cacheEnabled" value="true"/>
  </settings>
  ```

- 局部开启：

  ```xml
  <?xml version="1.0" encoding="UTF-8" ?>
  <!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
  <mapper namespace="com.sangeng.dao.RoleDao">
      <cache></cache>
  </mapper>
  ```

  