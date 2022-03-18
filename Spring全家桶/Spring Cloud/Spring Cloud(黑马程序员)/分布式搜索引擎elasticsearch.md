# 初识elasticsearch

https://www.bilibili.com/video/BV1LQ4y127n4?p=78



## 了解ES



### elasticsearch的作用

elasticsearch是一款非常强大的开源搜索引擎，具备非常多强大功能，可以帮助我们从海量数据中快速找到需要的内容



### ELK技术栈

>elasticsearch结合kibana，Logstash，Beats，也就是elastic stack(ELK)。
>被广泛应用在日志数据分析、实时监控等领域



而elasticsearch是elastic stack的核心，负责存储、搜索、分析数据。

![image-20220318141346056](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318141346056.png)



### elasticsearch和lucene

elasticsearch底层是基于**lucene**来实现的。

**Lucene**是一个Java语言的搜索引擎==类库==，是Apache公司的顶级项目，由DougCutting于1999年研发。官网地址：https://lucene.apache.org/ 。



Lucene的优势：

- 易拓展
- 高性能（支持倒排索引）

Lucene的缺点：

- 只限于Java语言开发
- 学习曲线陡峭(API设计复杂)
- 不支持水平拓展(不支持高并发、集群拓展的场景)





1. 2004年，Shay Banon基于Lucene开发了Compass

2. 2010年，Shay Banon重写了Compass，取名为Elasticsearch

    > 即Elasticsearch是Lucene的二次开发

相比于lucene，elasticsearch的具备的优势：

- 支持分布式，可水平拓展
- 提供Restful接口，可被任何语言调用



## 倒排索引

### 正向索引

例如给下表（tb_goods）中的id创建索引：

![image-20220318142719045](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318142719045.png)



如果根据id查询，那么直接走索引，查询速度非常快。

但如果是基于title做模糊查询，只能是逐行扫描数据，流程如下：

1）用户搜索数据，条件是title符合`"%手机%"`

2）逐行获取数据，比如id为1的数据

3）判断数据中的title是否符合用户搜索条件

4）如果符合则放入结果集，不符合则丢弃。回到步骤1



逐行扫描，也就是全表扫描，随着数据量增加，其查询效率也会越来越低。当数据量达到数百万时，就是一场灾难。

### 倒排索引

倒排索引中有两个非常重要的概念：

- 文档（`Document`）：用来搜索的数据，其中的每一条数据就是一个文档。例如一个网页、一个商品信息
- 词条（`Term`）：对文档数据或用户搜索数据，利用某种算法分词，得到的具备含义的词语就是词条。例如：我是中国人，就可以分为：我、是、中国人、中国、国人这样的几个词条



**创建倒排索引**是对正向索引的一种特殊处理，流程如下：

1. 将每一个文档的数据利用算法==分词==，得到一个个词条
2. 创建表，每行数据包括词条、词条所在文档id、位置等信息
3. 因为词条唯一性，可以给词条创建索引，例如hash表结构索引



![image-20220318142820440](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318142820440.png)



## es的一些概念

>elasticsearch中有很多独有的概念，与mysql中略有差别，但也有相似之处。



### 文档和字段

elasticsearch是面向**文档（Document）**存储的，可以是数据库中的一条商品数据，一个订单信息。文档数据会被序列化为json格式后存储在elasticsearch中：

![image-20220318143541364](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318143541364.png)

而Json文档中往往包含很多的**字段（Field）**，类似于数据库中的列。



### 索引和映射

**索引（Index）**，就是相同类型的文档的集合。

- 所有用户文档，就可以组织在一起，称为用户的索引；
- 所有商品的文档，可以组织在一起，称为商品的索引；
- 所有订单的文档，可以组织在一起，称为订单的索引；

>因此，我们可以把索引当做是数据库中的表。



数据库的表会有约束信息，用来定义表的结构、字段的名称、类型等信息。因此，索引库中就有**映射（mapping）**，是索引中文档的字段约束信息，类似表的结构约束。



### mysql与elasticsearch

我们统一的把mysql与elasticsearch的概念做一下对比：

| **MySQL** | **Elasticsearch** | **说明**                                                     |
| --------- | ----------------- | ------------------------------------------------------------ |
| Table     | Index             | 索引(index)，就是文档的集合，类似数据库的表(table)           |
| Row       | Document          | 文档（Document），就是一条条的数据，类似数据库中的行（Row），文档都是JSON格式 |
| Column    | Field             | 字段（Field），就是JSON文档中的字段，类似数据库中的列（Column） |
| Schema    | Mapping           | Mapping（映射）是索引中文档的约束，例如字段类型约束。类似数据库的表结构（Schema） |
| SQL       | DSL               | DSL是elasticsearch提供的JSON风格的请求语句，用来操作elasticsearch，实现CRUD |



- Mysql：擅长事务类型操作，可以确保数据的安全和一致性

- Elasticsearch：擅长海量数据的搜索、分析、计算



因此在企业中，往往是两者结合使用：

- 对安全性要求较高的写操作，使用mysql实现
- 对查询性能要求较高的搜索需求，使用elasticsearch实现
- 两者再基于某种方式，实现数据的同步，保证一致性

![image-20220318143655151](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318143655151.png)

## 安装ES、kibana、分词器

- 安装：见"安装elasticsearch.md"

- 分词器：同上

    > 默认分词器，对中文的支持并不友好



IK分词器包含两种模式：

* `ik_smart`：最少切分

* `ik_max_word`：最细切分



# 索引库操作

索引库就类似数据库表，mapping映射就类似表的结构。

我们要向es中存储数据，必须先创建“库”和“表”。



## mapping映射属性

mapping是对索引库中文档的约束，常见的mapping属性包括：

- type：字段数据类型，常见的简单类型有：
    - 字符串：text（可分词的文本）、keyword（精确值，例如：品牌、国家、ip地址，==不进行分词==）
    - 数值：long、integer、short、byte、double、float、
    - 布尔：boolean
    - 日期：date
    - 对象：object
- index：是否创建索引，==默认为true==
- analyzer：使用哪种分词器
- properties：该字段的子字段

> 注意，分词与否(text/keyword)，和建立索引与否(index为true或false)要分别指定



例如下面的json文档：

```json
{
    "age": 21,
    "weight": 52.1,
    "isMarried": false,
    "info": "黑马程序员Java讲师",
    "email": "zy@itcast.cn",
    "score": [99.1, 99.5, 98.9],
    "name": {
        "firstName": "云",
        "lastName": "赵"
    }
}
```

对应的每个字段映射（mapping）：

- age：类型为 integer；参与搜索，因此需要index为true；无需分词器
- weight：类型为float；参与搜索，因此需要index为true；无需分词器
- isMarried：类型为boolean；参与搜索，因此需要index为true；无需分词器
- info：类型为字符串，需要分词，因此是text；参与搜索，因此需要index为true；分词器可以用ik_smart
- email：类型为字符串，但是不需要分词，因此是keyword；不参与搜索，因此需要index为false；无需分词器
- score：虽然是数组，但是我们只看元素的类型，类型为float；参与搜索，因此需要index为true；无需分词器
- name：类型为object，需要定义多个子属性
    - name.firstName；类型为字符串，但是不需要分词，因此是keyword；参与搜索，因此需要index为true；无需分词器
    - name.lastName；类型为字符串，但是不需要分词，因此是keyword；参与搜索，因此需要index为true；无需分词器



## 索引库的CRUD



### 创建索引库和映射(PUT)

- 请求方式：PUT

- 请求路径：/索引库名，可以自定义

    > 无需主机和端口号，在配置Kibana时已经指定

- 请求参数：mapping映射

格式：

```json
PUT /索引库名称
{
  "mappings": {
    "properties": {
      "字段名":{
        "type": "text",
        "analyzer": "ik_smart"
      },
      "字段名2":{
        "type": "keyword",
        "index": "false"
      },
      "字段名3":{
        "properties": {
          "子字段": {
            "type": "keyword"
          }
        }
      },
      // ...略
    }
  }
}
```



例如：

```json
PUT /heima
{
  "mappings": {
    "properties": {
      "info":{
        "type": "text",
        "analyzer": "ik_smart"
      },
      "email":{
        "type": "keyword",
        "index": "falsae"
      },
      "name":{
        "properties": {
          "firstName": {
            "type": "keyword"
          }
        }
      },
      // ... 略
    }
  }
}
```



### 查询索引库(GET)

**基本语法**：

- 请求方式：GET

- 请求路径：/索引库名

- 请求参数：无

**格式**：

```
GET /索引库名
```



### 修改索引库(仅能新增)(PUT)

倒排索引结构虽然不复杂，但是一旦数据结构改变（比如改变了分词器），就需要重新创建倒排索引，这简直是灾难。因此索引库**一旦创建，无法修改mapping**



虽然无法修改mapping中已有的字段，但是却==允许添加新的字段到mapping==中，因为不会对倒排索引产生影响。

**语法说明**：

```json
PUT /索引库名/_mapping
{
  "properties": {
    "新字段名":{
      "type": "integer"
    }
  }
}
```



### 删除索引库(DELETE)

**语法：**

- 请求方式：DELETE
- 请求路径：/索引库名
- 请求参数：无

**格式：**

```
DELETE /索引库名
```



# 文档操作

## 新增文档(POST)

**语法：**

```json
POST /索引库名/_doc/文档id
{
    "字段1": "值1",
    "字段2": "值2",
    "字段3": {
        "子属性1": "值3",
        "子属性2": "值4"
    },
    // ...
}
```

> 新增文档时，记得在路径上带上文档ID，否则将会使用随机生成的ID



**示例：**

```json
POST /heima/_doc/1
{
    "info": "黑马程序员Java讲师",
    "email": "zy@itcast.cn",
    "name": {
        "firstName": "云",
        "lastName": "赵"
    }
}
```

**响应：**

![image-20210720212933362](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210720212933362-16475909059721.png)



## 查询文档(GET)

根据rest风格，新增是post，查询应该是get，不过查询一般都需要条件，这里我们把文档id带上。

**语法：**

```json
GET /{索引库名称}/_doc/{id}
```

**通过kibana查看数据：**

```js
GET /heima/_doc/1
```

**查看结果：**

![image-20210720213345003](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210720213345003.png)



## 删除文档(DELETE)

删除使用DELETE请求，同样，需要根据id进行删除：

**语法：**

```js
DELETE /{索引库名}/_doc/id值
```

**示例：**

```json
# 根据id删除数据
DELETE /heima/_doc/1
```

**结果：**

![image-20210720213634918](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210720213634918.png)



## 修改文档

修改有两种方式：

- 全量修改：直接覆盖原来的文档

- 增量修改：修改文档中的部分字段

    

### 全量修改(PUT)

全量修改是覆盖原来的文档，其本质是：

- 根据指定的id删除文档
- 新增一个相同id的文档

> 如果ID不存在，第二步仍执行。修改操作将变成新增操作



**语法：**

```json
PUT /{索引库名}/_doc/文档id
{
    "字段1": "值1",
    "字段2": "值2",
    // ... 略
}

```



**示例：**

```json
PUT /heima/_doc/1
{
    "info": "黑马程序员高级Java讲师",
    "email": "zy@itcast.cn",
    "name": {
        "firstName": "云",
        "lastName": "赵"
    }
}
```



### 增量修改(POST)

增量修改是只修改指定id匹配的文档中的部分字段。

**语法：**

```json
POST /{索引库名}/_update/文档id
{
    "doc": {
         "字段名": "新的值",
    }
}
```



**示例：**

```json
POST /heima/_update/1
{
  "doc": {
    "email": "ZhaoYun@itcast.cn"
  }
}
```

 

# RestClient

## 什么是RestClient

ES官方提供了各种不同语言的客户端，用来操作ES。

这些客户端的本质就是组装DSL语句，通过http请求发送给ES。官方文档地址：https://www.elastic.co/guide/en/elasticsearch/client/index.html

其中的Java Rest Client又包括两种：

- Java Low Level Rest Client
- Java High Level Rest Client

## JavaRestClient操作索引库

### 导入Demo工程

1. 导入数据库（tb_hotel.sql）(数据库名为heima)
2. 导入项目（hotel-demo)



### mapping映射分析

创建索引库，最关键的是mapping映射，而mapping映射要考虑的信息包括：

- 字段名

- 字段数据类型

    >字段名、字段数据类型，可以参考数据表结构的名称和类型

- 是否参与搜索

    > 根据业务分析

- 是否需要分词

    > 如果内容是一个整体就无需分词，反之则要分词

- 如果分词，分词器是什么？

    >统一使用ik_max_word



最终得到酒店数据的索引库结构:

```json
PUT /hotel
{
  "mappings": {
    "properties": {
      "id": {
        "type": "keyword"
      },
      "name":{
        "type": "text",
        "analyzer": "ik_max_word",
        "copy_to": "all"
      },
      "address":{
        "type": "keyword",
        "index": false
      },
      "price":{
        "type": "integer"
      },
      "score":{
        "type": "integer"
      },
      "brand":{
        "type": "keyword",
        "copy_to": "all"
      },
      "city":{
        "type": "keyword",
        "copy_to": "all"
      },
      "starName":{
        "type": "keyword"
      },
      "business":{
        "type": "keyword"
      },
      "location":{
        "type": "geo_point"
      },
      "pic":{
        "type": "keyword",
        "index": false
      },
      "all":{
        "type": "text",
        "analyzer": "ik_max_word"
      }
    }
  }
}
```

> 特殊字段说明：
>
> - location：地理坐标，里面包含精度、纬度
> - all：一个组合字段，其目的是将多字段的值 利用copy_to合并，提供给用户搜索



![image-20220318164446411](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318164446411.png)

![image-20220318164451180](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318164451180.png)



### 初始化RestHighLevelClient

在elasticsearch提供的API中，与elasticsearch一切交互都封装在一个名为RestHighLevelClient的类中，必须先完成这个对象的初始化，建立与elasticsearch的连接。



步骤：

1. 引入es的RestHighLevelClient依赖：

    ```xml
    <dependency>
        <groupId>org.elasticsearch.client</groupId>
        <artifactId>elasticsearch-rest-high-level-client</artifactId>
    </dependency>
    ```

2. 因为SpringBoot默认的ES版本是7.6.2，所以我们需要覆盖默认的ES版本：

    ```xml
    <properties>
        <java.version>1.8</java.version>
        <elasticsearch.version>7.12.1</elasticsearch.version>
    </properties>
    ```

    > 注意与服务端的版本保持一致

3. 初始化RestHighLevelClient：

    ```java
    RestHighLevelClient client = new RestHighLevelClient(RestClient.builder(
            HttpHost.create("http://192.168.150.101:9200")
    ));
    ```

4. 释放RestHighLevelClient

    ```java
    client.close();
    ```



此处使用测试类HotelIndexTest：

```java
package cn.itcast.hotel;

import org.apache.http.HttpHost;
import org.elasticsearch.client.RestHighLevelClient;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;

public class HotelIndexTest {
    private RestHighLevelClient client;

    @BeforeEach
    void setUp() {
        this.client = new RestHighLevelClient(RestClient.builder(
                HttpHost.create("http://192.168.150.101:9200")
        ));
    }

    @AfterEach
    void tearDown() throws IOException {
        this.client.close();
    }
}
```



### 创建索引库

分为三步：

1. 创建Request对象。因为是创建索引库的操作，因此Request是CreateIndexRequest。
2. 添加请求参数，其实就是DSL的JSON参数部分。因为json字符串很长，这里是定义了静态字符串常量MAPPING_TEMPLATE，让代码看起来更加优雅。
3. 发送请求，client.indices()方法的返回值是IndicesClient类型，封装了所有与索引库操作有关的方法。

![image-20220318165724813](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318165724813.png)



即完整步骤：

1. 定义mapping映射的JSON字符串常量：

    ```json
    package cn.itcast.hotel.constants;
    
    public class HotelConstants {
        public static final String MAPPING_TEMPLATE = "{\n" +
                "  \"mappings\": {\n" +
                "    \"properties\": {\n" +
                "      \"id\": {\n" +
                "        \"type\": \"keyword\"\n" +
                "      },\n" +
                "      \"name\":{\n" +
                "        \"type\": \"text\",\n" +
                "        \"analyzer\": \"ik_max_word\",\n" +
                "        \"copy_to\": \"all\"\n" +
                "      },\n" +
                "      \"address\":{\n" +
                "        \"type\": \"keyword\",\n" +
                "        \"index\": false\n" +
                "      },\n" +
                "      \"price\":{\n" +
                "        \"type\": \"integer\"\n" +
                "      },\n" +
                "      \"score\":{\n" +
                "        \"type\": \"integer\"\n" +
                "      },\n" +
                "      \"brand\":{\n" +
                "        \"type\": \"keyword\",\n" +
                "        \"copy_to\": \"all\"\n" +
                "      },\n" +
                "      \"city\":{\n" +
                "        \"type\": \"keyword\",\n" +
                "        \"copy_to\": \"all\"\n" +
                "      },\n" +
                "      \"starName\":{\n" +
                "        \"type\": \"keyword\"\n" +
                "      },\n" +
                "      \"business\":{\n" +
                "        \"type\": \"keyword\"\n" +
                "      },\n" +
                "      \"location\":{\n" +
                "        \"type\": \"geo_point\"\n" +
                "      },\n" +
                "      \"pic\":{\n" +
                "        \"type\": \"keyword\",\n" +
                "        \"index\": false\n" +
                "      },\n" +
                "      \"all\":{\n" +
                "        \"type\": \"text\",\n" +
                "        \"analyzer\": \"ik_max_word\"\n" +
                "      }\n" +
                "    }\n" +
                "  }\n" +
                "}";
    }
    ```

2. 在HotelIndexTest测试类中，编写单元测试，实现创建索引：

    ```java
    @Test
    void createHotelIndex() throws IOException {
        // 1.创建Request对象
        CreateIndexRequest request = new CreateIndexRequest("hotel");
        // 2.准备请求的参数：DSL语句
        request.source(MAPPING_TEMPLATE, XContentType.JSON);
        // 3.发送请求
        client.indices().create(request, RequestOptions.DEFAULT);
    }
    ```

    

### 删除索引库

删除索引库的DSL语句非常简单：

```json
DELETE /hotel
```

与创建索引库相比：

- ==请求方式从PUT变为DELTE==
- 请求路径不变
- 无请求参数



所以代码的差异，注意体现在Request对象上。依然是三步走：

1. 创建Request对象。这次是==DeleteIndexRequest对象==
2. 准备参数。这里是无参
3. 发送请求。==改用delete方法==

在hotel-demo中的HotelIndexTest测试类中，编写单元测试，实现删除索引：

```java
@Test
void testDeleteHotelIndex() throws IOException {
    // 1.创建Request对象
    DeleteIndexRequest request = new DeleteIndexRequest("hotel");
    // 2.发送请求
    client.indices().delete(request, RequestOptions.DEFAULT);
}
```



### 判断索引库是否存在

判断索引库是否存在，本质就是查询，对应的DSL是：

```json
GET /hotel
```



因此与删除的Java代码流程是类似的。依然是三步走：

1. 创建Request对象。这次是==GetIndexRequest对象==
2. 准备参数。这里是无参
3. 发送请求。==改用exists方法==

```java
@Test
void testExistsHotelIndex() throws IOException {
    // 1.创建Request对象
    GetIndexRequest request = new GetIndexRequest("hotel");
    // 2.发送请求
    boolean exists = client.indices().exists(request, RequestOptions.DEFAULT);
    // 3.输出
    System.err.println(exists ? "索引库已经存在！" : "索引库不存在！");
}
```



###总结

JavaRestClient操作elasticsearch的流程基本类似。

==核心是client.indices()方法来获取索引库的操作对象===。

索引库操作的基本步骤：

1. 初始化RestHighLevelClient
2. 创建XxxIndexRequest。XXX是Create、Get、Delete
3. 准备DSL（ Create时需要，其它是无参）
4. 发送请求。调用RestHighLevelClient#indices().xxx()方法，xxx是create、exists、delete





## JavaRestClient操作文档

与操作索引库类似的：

1. 初始化[RestHighLevelClient](#初始化RestHighLevelClient)

2. 新建测试类RestHighLevelClient：

    ```java
    package cn.itcast.hotel;
    
    import cn.itcast.hotel.pojo.Hotel;
    import cn.itcast.hotel.service.IHotelService;
    import org.junit.jupiter.api.AfterEach;
    import org.junit.jupiter.api.BeforeEach;
    import org.junit.jupiter.api.Test;
    import org.springframework.beans.factory.annotation.Autowired;
    import org.springframework.boot.test.context.SpringBootTest;
    
    import java.io.IOException;
    import java.util.List;
    
    @SpringBootTest
    public class HotelDocumentTest {
        @Autowired
        private IHotelService hotelService;
    
        private RestHighLevelClient client;
    
        @BeforeEach
        void setUp() {
            this.client = new RestHighLevelClient(RestClient.builder(
                    HttpHost.create("http://192.168.150.101:9200")
            ));
        }
    
        @AfterEach
        void tearDown() throws IOException {
            this.client.close();
        }
    }
    ```

    

### 新增文档

需求：将数据库的酒店数据查询出来，写入elasticsearch中



步骤如下：

1. 根据id查询数据库，得到封装酒店数据的Hotel对象

2. 将Hotel对象，转换为HotelDoc对象

    > 因为字段longitude和latitude合并为location，所以额外定义了实体类HotelDoc

3. 将HotelDoc对象序列化为json格式

4. 创建IndexRequest，指定索引库名和id

5. 准备请求参数，也就是JSON文档

6. 发送请求

```java
@Test
void testAddDocument() throws IOException {
    // 1.根据id查询酒店数据
    Hotel hotel = hotelService.getById(61083L);
    // 2.转换为文档类型
    HotelDoc hotelDoc = new HotelDoc(hotel);
    // 3.将HotelDoc转json
    String json = JSON.toJSONString(hotelDoc);

    // 1.准备Request对象
    IndexRequest request = new IndexRequest("hotel").id(hotelDoc.getId().toString());
    // 2.准备Json文档
    request.source(json, XContentType.JSON);
    // 3.发送请求
    client.index(request, RequestOptions.DEFAULT);
}
```



新增文档的DSL语句如下：

```json
POST /{索引库名}/_doc/1
{
    "name": "Jack",
    "age": 21
}
```

Java代码与DSL语句的关系：

![image-20220318172613376](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318172613376.png)



### 查询文档

查询的DSL语句如下：

```json
GET /hotel/_doc/{id}
```



步骤：

1. 准备Request对象。这次是查询，所以是GetRequest
2. 发送请求，得到结果。因为是查询，这里调用client.get()方法
3. 解析结果，就是对JSON做反序列化

```java
@Test
void testGetDocumentById() throws IOException {
    // 1.准备Request
    GetRequest request = new GetRequest("hotel", "61082");
    // 2.发送请求，得到响应
    GetResponse response = client.get(request, RequestOptions.DEFAULT);
    // 3.解析响应结果
    String json = response.getSourceAsString();

    HotelDoc hotelDoc = JSON.parseObject(json, HotelDoc.class);
    System.out.println(hotelDoc);
}
```

> "61802"是文档的ID



![image-20220318173804387](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318173804387.png)



### 删除文档

删除的DSL如下：

```json
DELETE /hotel/_doc/{id}
```



步骤：

1. 准备Request对象，因为是删除，这次是DeleteRequest对象。要指定索引库名和id
2. 准备参数，无参
3. 发送请求。因为是删除，所以是client.delete()方法

```java
@Test
void testDeleteDocument() throws IOException {
    // 1.准备Request
    DeleteRequest request = new DeleteRequest("hotel", "61083");
    // 2.发送请求
    client.delete(request, RequestOptions.DEFAULT);
}
```





### 修改文档

修改有两种方式：

- 全量修改：本质是先根据id删除，再新增
- 增量修改：修改文档中的指定字段值



在RestClient的API中，全量修改与新增的API完全一致。

- 如果新增时，ID已经存在，则修改
- 如果新增时，ID不存在，则新增



步骤：

1. 准备Request对象。这次是修改，所以是UpdateRequest
2. 准备参数。也就是JSON文档，里面包含要修改的字段
3. 更新文档。这里调用client.update()方法

```java
@Test
void testUpdateDocument() throws IOException {
    // 1.准备Request
    UpdateRequest request = new UpdateRequest("hotel", "61083");
    // 2.准备请求参数
    request.doc(
        "price", "952",
        "starName", "四钻"
    );
    // 3.发送请求
    client.update(request, RequestOptions.DEFAULT);
}
```

![image-20220318173817991](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318173817991.png)



### 批量导入文档

案例需求：利用BulkRequest批量将数据库数据导入到索引库中。



步骤如下：

1. 创建Request对象。这里是BulkRequest
2. 准备参数。批处理的参数，就是其它Request对象，这里就是多个IndexRequest
3. 发起请求。这里是批处理，调用的方法为client.bulk()方法

```java
@Test
void testBulkRequest() throws IOException {
    // 批量查询酒店数据
    List<Hotel> hotels = hotelService.list();

    // 1.创建Request
    BulkRequest request = new BulkRequest();
    // 2.准备参数，添加多个新增的Request
    for (Hotel hotel : hotels) {
        // 2.1.转换为文档类型HotelDoc
        HotelDoc hotelDoc = new HotelDoc(hotel);
        // 2.2.创建新增文档的Request对象
        request.add(new IndexRequest("hotel")
                    .id(hotelDoc.getId().toString())
                    .source(JSON.toJSONString(hotelDoc), XContentType.JSON));
    }
    // 3.发送请求
    client.bulk(request, RequestOptions.DEFAULT);
}
```



其中BulkRequest#add()方法，用于添加其他请求：

![image-20220318175624036](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220318175624036.png)

可以看到，能添加的请求包括：

- IndexRequest，也就是新增
- UpdateRequest，也就是修改
- DeleteRequest，也就是删除



### 小结

文档操作的基本步骤：

- 初始化RestHighLevelClient
- 创建XxxRequest。XXX是Index、Get、Update、Delete、Bulk
- 准备参数（Index、Update、Bulk时需要）
- 发送请求。调用RestHighLevelClient#.xxx()方法，xxx是index、get、update、delete、bulk
- 解析结果（Get时需要）





# ES搜索功能

https://www.bilibili.com/video/BV1LQ4y127n4?p=100