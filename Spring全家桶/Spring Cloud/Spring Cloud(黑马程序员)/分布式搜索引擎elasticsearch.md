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





# DSL查询文档

elasticsearch的查询依然是基于JSON风格的DSL来实现的。



## DSL查询分类

Elasticsearch提供了基于JSON的DSL（[Domain Specific Language](https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl.html)）来定义查询。常见的查询类型包括：

- **查询所有**：查询出所有数据，一般测试用。例如：match_all

- **全文检索（full text）查询**：利用分词器对用户输入内容分词，然后去倒排索引库中匹配。例如：
    - match_query
    - multi_match_query
- **精确查询**：根据精确词条值查找数据，一般是查找keyword、数值、日期、boolean等类型字段。例如：
    - ids
    - range
    - term
- **地理（geo）查询**：根据经纬度查询。例如：
    - geo_distance
    - geo_bounding_box
- **复合（compound）查询**：复合查询可以将上述各种查询条件组合起来，合并查询条件。例如：
    - bool
    - function_score



查询的语法基本一致：

```json
GET /indexName/_search
{
  "query": {
    "查询类型": {
      "查询条件": "条件值"
    }
  }
}
```



以查询所有为例，其中：

- 查询类型为match_all
- 没有查询条件

```json
// 查询所有
GET /indexName/_search
{
  "query": {
    "match_all": {
    }
  }
}
```

其它查询无非就是**查询类型**、**查询条件**的变化。



## 全文检索查询

### 检索流程

全文检索查询的基本流程如下：

1. 对用户搜索的内容做分词，得到词条
2. 根据词条去倒排索引库中匹配，得到文档id
3. 根据文档id找到文档，返回给用户

>因为是拿着词条去匹配，因此参与搜索的字段也必须是可分词的text类型的字段。
>



### 基本语法

常见的全文检索查询包括：

- match查询：==单字段查询==

    ```json
    GET /indexName/_search
    {
      "query": {
        "match": {
          "FIELD": "TEXT"
        }
      }
    }
    ```

- multi_match查询：==多字段查询==，任意一个字段符合条件就算符合查询条件

    ```json
    GET /indexName/_search
    {
      "query": {
        "multi_match": {
          "query": "TEXT",
          "fields": ["FIELD1", " FIELD12"]
        }
      }
    }
    ```

    > 但是，搜索字段越多，对查询性能影响越大，因此建议采用copy_to，从而使用单字段查询。



### 示例

- match查询：

    ```json
    GET /hotel/_search
    {
      "query": {
        "match": {
          "all": "如家酒店"
        }
      }
    }
    ```

- multi_match查询：

    ```json
    GET /hotel/_search
    {
      "query": {
        "multi_match": {
          "query": "如家酒店",
          "fields": ["brand","name","business"];
        }
      }
    }
    ```

    



## 精准查询

精确查询一般是查找keyword、数值、日期、boolean等类型字段。所以**不会**对搜索条件分词。常见的有：

- term：根据词条精确值查询
- range：根据值的范围查询



### term查询

语法说明：

```json
// term查询
GET /indexName/_search
{
  "query": {
    "term": {
      "FIELD": {
        "value": "VALUE"
      }
    }
  }
}
```



### range查询

基本语法：

```json
// range查询
GET /indexName/_search
{
  "query": {
    "range": {
      "FIELD": {
        "gte": 10, // 这里的gte代表大于等于，gt则代表大于
        "lte": 20 // lte代表小于等于，lt则代表小于
      }
    }
  }
}
```



## 地理坐标查询

### 矩形范围查询

矩形范围查询，也就是geo_bounding_box查询，查询坐标落在某个矩形范围的所有文档：

![DKV9HZbVS6](file://E:\BaiduNetdiskDownload\1%E3%80%81%E5%BE%AE%E6%9C%8D%E5%8A%A1%E5%BC%80%E5%8F%91%E6%A1%86%E6%9E%B6SpringCloud+RabbitMQ+Docker+Redis+%E6%90%9C%E7%B4%A2+%E5%88%86%E5%B8%83%E5%BC%8F%E5%8F%B2%E4%B8%8A%E6%9C%80%E5%85%A8%E9%9D%A2%E7%9A%84%E5%BE%AE%E6%9C%8D%E5%8A%A1%E5%85%A8%E6%8A%80%E6%9C%AF%E6%A0%88%E8%AF%BE%E7%A8%8B\%E5%AE%9E%E7%94%A8%E7%AF%87\%E5%AD%A6%E4%B9%A0%E8%B5%84%E6%96%99\day06-Elasticsearch02\%E8%AE%B2%E4%B9%89\assets\DKV9HZbVS6.gif?lastModify=1647930781)

查询时，需要指定矩形的**左上**、**右下**两个点的坐标，然后画出一个矩形，落在该矩形内的都是符合条件的点。



语法如下：

```json
// geo_bounding_box查询
GET /indexName/_search
{
  "query": {
    "geo_bounding_box": {
      "FIELD": {
        "top_left": { // 左上点
          "lat": 31.1,
          "lon": 121.5
        },
        "bottom_right": { // 右下点
          "lat": 30.9,
          "lon": 121.7
        }
      }
    }
  }
}
```



### 附近查询

附近查询，也叫做距离查询（geo_distance）：查询到指定中心点小于某个距离值的所有文档。

![vZrdKAh19C](file://E:\BaiduNetdiskDownload\1%E3%80%81%E5%BE%AE%E6%9C%8D%E5%8A%A1%E5%BC%80%E5%8F%91%E6%A1%86%E6%9E%B6SpringCloud+RabbitMQ+Docker+Redis+%E6%90%9C%E7%B4%A2+%E5%88%86%E5%B8%83%E5%BC%8F%E5%8F%B2%E4%B8%8A%E6%9C%80%E5%85%A8%E9%9D%A2%E7%9A%84%E5%BE%AE%E6%9C%8D%E5%8A%A1%E5%85%A8%E6%8A%80%E6%9C%AF%E6%A0%88%E8%AF%BE%E7%A8%8B\%E5%AE%9E%E7%94%A8%E7%AF%87\%E5%AD%A6%E4%B9%A0%E8%B5%84%E6%96%99\day06-Elasticsearch02\%E8%AE%B2%E4%B9%89\assets\vZrdKAh19C.gif?lastModify=1647930811)



语法说明：

```json
// geo_distance 查询
GET /indexName/_search
{
  "query": {
    "geo_distance": {
      "distance": "15km", // 半径
      "FIELD": "31.21,121.5" // 圆心
    }
  }
}
```



## 复合查询

复合（compound）查询：复合查询可以将其它简单查询组合起来，实现更复杂的搜索逻辑。常见的有两种：

- fuction score：算分函数查询，可以控制文档相关性算分，控制文档排名
- bool query：布尔查询，利用逻辑关系组合多个其它的查询，实现复杂搜索



### 相关性算分的算法

当我们利用match查询时，文档结果会根据与搜索词条的关联度打分（_score），返回结果时按照分值降序排列。



在elasticsearch中，早期使用的打分算法是TF-IDF算法，公式如下：

![image-20220322143445261](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322143445261.png)

在后来的5.1版本升级中，elasticsearch将算法改进为BM25算法，公式如下：

![image-20220322143452416](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322143452416.png)



TF-IDF算法有一个缺陷，就是词条频率越高，文档得分也会越高，单个词条对文档影响较大。而BM25则会让单个词条的算分有一个上限，曲线更加平滑：

![image-20220322143501143](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322143501143.png)



### 算分函数查询

#### 语法说明

![image-20220322143559609](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322143559609.png)

function score 查询中包含四部分内容：

- **原始查询**条件：query部分，基于这个条件搜索文档，并且基于BM25算法给文档打分，**原始算分**（query score)
- **过滤条件**：filter部分，符合该条件的文档才会重新算分
- **算分函数**：符合filter条件的文档要根据这个函数做运算，得到的**函数算分**（function score），有四种函数
    - weight：函数结果是常量
    - field_value_factor：以文档中的某个字段值作为函数结果
    - random_score：以随机数作为函数结果
    - script_score：自定义算分函数算法
- **运算模式**：算分函数的结果、原始查询的相关性算分，两者之间的运算方式，包括：
    - multiply：相乘
    - replace：用function score替换query score
    - 其它，例如：sum、avg、max、min



function score的运行流程如下：

- 1）根据**原始条件**查询搜索文档，并且计算相关性算分，称为**原始算分**（query score）
- 2）根据**过滤条件**，过滤文档
- 3）符合**过滤条件**的文档，基于**算分函数**运算，得到**函数算分**（function score）
- 4）将**原始算分**（query score）和**函数算分**（function score）基于**运算模式**做运算，得到最终结果，作为相关性算分。

>- 过滤条件：决定哪些文档的算分被修改
>- 算分函数：决定函数算分的算法
>- 运算模式：决定最终算分结果



#### 示例

```json
GET /hotel/_search
{
  "query": {
    "function_score": {
      "query": {  .... }, // 原始查询，可以是任意条件
      "functions": [ // 算分函数
        {
          "filter": { // 满足的条件，品牌必须是如家
            "term": {
              "brand": "如家"
            }
          },
          "weight": 2 // 算分权重为2
        }
      ],
      "boost_mode": "sum" // 加权模式，求和
    }
  }
}
```

- 原始条件：不确定，可以任意变化
- 过滤条件：brand = "如家"
- 算分函数：可以简单粗暴，直接给固定的算分结果，weight
- 运算模式：比如求和



### 布尔查询

布尔查询是一个或多个查询子句的组合，每一个子句就是一个**子查询**。子查询的组合方式有：

- must：必须匹配每个子查询，类似“与”
- should：选择性匹配子查询，类似“或”
- must_not：必须不匹配，**不参与算分**，类似“非”
- filter：必须匹配，**不参与算分**

> 在搜索时，参与**打分的字段越多，查询的性能也越差**



#### 语法示例

```json
GET /hotel/_search
{
  "query": {
    "bool": {
      "must": [
        {"term": {"city": "上海" }}
      ],
      "should": [
        {"term": {"brand": "皇冠假日" }},
        {"term": {"brand": "华美达" }}
      ],
      "must_not": [
        { "range": { "price": { "lte": 500 } }}
      ],
      "filter": [
        { "range": {"score": { "gte": 45 } }}
      ]
    }
  }
}
```



# 搜索结果处理

## 排序

elasticsearch默认是根据相关度算分（_score）来排序，但是也支持自定义方式对搜索[结果排序](https://www.elastic.co/guide/en/elasticsearch/reference/current/sort-search-results.html)。可以排序字段类型有：keyword类型、数值类型、地理坐标类型、日期类型等。



### 普通字段排序

keyword、数值、日期类型排序的语法基本一致。

**语法**：

```json
GET /indexName/_search
{
  "query": {
    "match_all": {}
  },
  "sort": [
    {
      "FIELD": "desc"  // 排序字段、排序方式ASC、DESC
    }
  ]
}
```

> 排序条件是一个数组，也就是可以写多个排序条件。按照声明的顺序，当第一个条件相等时，再按照第二个条件排序，以此类推



### 地理坐标排序

地理坐标排序略有不同。

**语法说明**：

```json
GET /indexName/_search
{
  "query": {
    "match_all": {}
  },
  "sort": [
    {
      "_geo_distance" : {
          "FIELD" : "纬度，经度", // 文档中geo_point类型的字段名、目标坐标点
          "order" : "asc", // 排序方式
          "unit" : "km" // 排序的距离单位
      }
    }
  ]
}
```

>指定字段必须是geo_point类型



## 分页

elasticsearch 默认情况下只返回top10的数据。而如果要查询更多数据就需要修改分页参数了。elasticsearch中通过修改from、size参数来控制要返回的分页结果：

- from：从第几个文档开始
- size：总共查询几个文档



### 分页语法

```json
GET /hotel/_search
{
  "query": {
    "match_all": {}
  },
  "from": 0, // 分页开始的位置，默认为0
  "size": 10, // 期望获取的文档总数
  "sort": [
    {"price": "asc"}
  ]
}
```



### 深度分页问题

对于查询990~1000的数据，查询语句为：

```json
GET /hotel/_search
{
  "query": {
    "match_all": {}
  },
  "from": 990, // 分页开始的位置，默认为0
  "size": 10, // 期望获取的文档总数
  "sort": [
    {"price": "asc"}
  ]
}
```

但在ES的内部分页逻辑：

1. 先排序出前1000条数据
2. 再截取出990~1000的文档



这意味着，在集群模式下：

1. 每个ES节点，先排序出前1000条数据
2. 将所有ES节点的结果进行聚合，再次进行排序
3. 再截取出990~1000的文档

当汇总后的数据过多时，将会带来比较大的压力。

> 因此elasticsearch会禁止from+ size 超过10000的请求。



针对深度分页，ES提供了两种解决方案，[官方文档](https://www.elastic.co/guide/en/elasticsearch/reference/current/paginate-search-results.html)：

- search after：分页时需要排序，原理是从上一次的排序值开始，查询下一页数据。官方推荐使用的方式。
- scroll：原理将排序后的文档id形成快照，保存在内存。官方已经不推荐使用。



分页查询的常见实现方案以及优缺点：

- `from + size`：
    - 优点：支持随机翻页
    - 缺点：深度分页问题，默认查询上限（from + size）是10000
    - 场景：百度、京东、谷歌、淘宝这样的随机翻页搜索
- `after search`：
    - 优点：没有查询上限（单次查询的size不超过10000）
    - 缺点：只能向后逐页查询，不支持随机翻页
    - 场景：没有随机翻页需求的搜索，例如手机向下滚动翻页

- `scroll`：
    - 优点：没有查询上限（单次查询的size不超过10000）
    - 缺点：会有额外内存消耗，并且搜索结果是非实时的
    - 场景：海量数据的获取和迁移。从ES7.1开始不推荐，建议用 after search方案。



## 高亮

高亮显示的实现分为两步：

- 1）为查询结果中的关键字添加一个标签，例如`<em>`标签
- 2）为`<em>`标签编写CSS样式

### 高亮语法

```json
GET /hotel/_search
{
  "query": {
    "match": {
      "FIELD": "TEXT" // 查询条件，高亮一定要使用全文检索查询
    }
  },
  "highlight": {
    "fields": { // 指定要高亮的字段
      "FIELD": {
        "pre_tags": "<em>",  // 用来标记高亮字段的前置标签
        "post_tags": "</em>" // 用来标记高亮字段的后置标签
      }
    }
  }
}
```

**注意：**

- 高亮是对关键字高亮，因此**搜索条件必须带有关键字**，而不能是范围这样的查询。
- 默认情况下，**高亮的字段，必须与搜索指定的字段一致**，否则无法高亮
- 如果要对非搜索字段高亮，则需要添加一个属性：required_field_match=false



## 总结

查询的DSL是一个大的JSON对象，包含下列属性：

- query：查询条件
- from和size：分页条件
- sort：排序条件
- highlight：高亮条件

示例：

![image-20210721203657850](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210721203657850.png)







# RestClient查询文档

基本步骤包括：

1. 准备Request对象
2. 准备请求参数
3. 发起请求
4. 解析响应



## 快速入门

以match_all查询为例



### 发起查询请求

![image-20220322154756915](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322154756915.png)

代码解读：

- 第一步，创建`SearchRequest`对象，指定索引库名

- 第二步，利用`request.source()`构建DSL，DSL中可以包含查询、分页、排序、高亮等
    - `query()`：代表查询条件，利用`QueryBuilders.matchAllQuery()`构建一个match_all查询的DSL
- 第三步，利用client.search()发送请求，得到响应



关键API：

- `request.source()`，其中包含了查询、排序、分页、高亮等所有功能：

    <img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322154843200.png" alt="image-20220322154843200" style="zoom:67%;" />

- 工具类`QueryBuilders`：

    <img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322154958566.png" alt="image-20220322154958566" style="zoom:80%;" />



###解析响应

![image-20220322155058182](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322155058182.png)



elasticsearch返回的结果是一个JSON字符串，结构包含：

- `hits`：命中的结果
    - `total`：总条数，其中的value是具体的总条数值
    - `max_score`：所有结果中得分最高的文档的相关性算分
    - `hits`：搜索结果的文档数组，其中的每个文档都是一个json对象
        - `_source`：文档中的原始数据，也是json对象

因此，我们解析响应结果，就是逐层解析JSON字符串，流程如下：

- `SearchHits`：通过response.getHits()获取，就是JSON中的最外层的hits，代表命中的结果
    - `SearchHits#getTotalHits().value`：获取总条数信息
    - `SearchHits#getHits()`：获取SearchHit数组，也就是文档数组
        - `SearchHit#getSourceAsString()`：获取文档结果中的_source，也就是原始的json文档数据

### 完整代码如下

```java
@Test
void testMatchAll() throws IOException {
    // 1.准备Request
    SearchRequest request = new SearchRequest("hotel");
    // 2.准备DSL
    request.source()
        .query(QueryBuilders.matchAllQuery());
    // 3.发送请求
    SearchResponse response = client.search(request, RequestOptions.DEFAULT);

    // 4.解析响应
    handleResponse(response);
}

private void handleResponse(SearchResponse response) {
    // 4.解析响应
    SearchHits searchHits = response.getHits();
    // 4.1.获取总条数
    long total = searchHits.getTotalHits().value;
    System.out.println("共搜索到" + total + "条数据");
    // 4.2.文档数组
    SearchHit[] hits = searchHits.getHits();
    // 4.3.遍历
    for (SearchHit hit : hits) {
        // 获取文档source
        String json = hit.getSourceAsString();
        // 反序列化
        HotelDoc hotelDoc = JSON.parseObject(json, HotelDoc.class);
        System.out.println("hotelDoc = " + hotelDoc);
    }
}
```



## match查询

全文检索的match和multi_match查询与match_all的API基本一致。差别是查询条件，也就是query的部分。

<img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322155935915.png" alt="image-20220322155935915" style="zoom:50%;" />

因此，Java代码上的差异主要是request.source().query()中的参数了。同样是利用QueryBuilders提供的方法：

<img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322155949365.png" alt="image-20220322155949365" style="zoom:67%;" />



```java
@Test
void testMatch() throws IOException {
    // 1.准备Request
    SearchRequest request = new SearchRequest("hotel");
    // 2.准备DSL
    request.source()
        .query(QueryBuilders.matchQuery("all", "如家"));
    // 3.发送请求
    SearchResponse response = client.search(request, RequestOptions.DEFAULT);
    // 4.解析响应
    handleResponse(response);

}
```



## 精确查询

精确查询主要是两者：

- term：词条精确匹配
- range：范围查询

与之前的查询相比，差异同样在查询条件，其它都一样。



查询条件构造的API如下：

<img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322160048625.png" alt="image-20220322160048625" style="zoom:67%;" />



## 布尔查询

布尔查询是用must、must_not、filter等方式组合其它查询，代码示例如下：

<img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322160111665.png" alt="image-20220322160111665" style="zoom:67%;" />



```java
@Test
void testBool() throws IOException {
    SearchRequest request = new SearchRequest("hotel");

    BoolQueryBuilder boolQuery = QueryBuilders.boolQuery();
    boolQuery.must(QueryBuilders.termQuery("city", "杭州"));
    boolQuery.filter(QueryBuilders.rangeQuery("price").lte(250));
    request.source().query(boolQuery);
    
    SearchResponse response = client.search(request, RequestOptions.DEFAULT);
    handleResponse(response);
}
```



## 排序、分页

搜索结果的排序和分页是与query同级的参数，因此同样是使用request.source()来设置。

对应的API如下：

<img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322162216201.png" alt="image-20220322162216201" style="zoom:80%;" />



完整代码示例：

```java
@Test
void testPageAndSort() throws IOException {
    // 页码，每页大小
    int page = 1, size = 5;

    // 1.准备Request
    SearchRequest request = new SearchRequest("hotel");
    // 2.准备DSL
    // 2.1.query
    request.source().query(QueryBuilders.matchAllQuery());
    // 2.2.排序 sort
    request.source().sort("price", SortOrder.ASC);
    // 2.3.分页 from、size
    request.source().from((page - 1) * size).size(5);
    // 3.发送请求
    SearchResponse response = client.search(request, RequestOptions.DEFAULT);
    // 4.解析响应
    handleResponse(response);
}
```



## 高亮

高亮的代码与之前代码差异较大，有两点：

- 查询的DSL：其中除了查询条件，还需要添加高亮条件，同样是与query同级。
- 结果解析：结果除了要解析_source文档数据，还要解析高亮结果



### 高亮请求构建

高亮请求的构建API如下：

![image-20220322162302574](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322162302574.png)

> 上述代码省略了查询条件部分，但注意：高亮查询必须使用全文检索查询，并且要有搜索关键字，将来才可以对关键字高亮。



```java
@Test
void testHighlight() throws IOException {
    // 1.准备Request
    SearchRequest request = new SearchRequest("hotel");
    // 2.准备DSL
    // 2.1.query
    request.source().query(QueryBuilders.matchQuery("all", "如家"));
    // 2.2.高亮
    request.source().highlighter(new HighlightBuilder().field("name").requireFieldMatch(false));
    // 3.发送请求
    SearchResponse response = client.search(request, RequestOptions.DEFAULT);
    // 4.解析响应
    handleResponse(response);

}
```



### 高亮结果解析

高亮的结果与查询的文档结果默认是分离的，并不在一起。

因此解析高亮的代码需要额外处理：

![image-20220322162350537](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220322162350537.png)



```java
private void handleResponse(SearchResponse response) {
    // 4.解析响应
    SearchHits searchHits = response.getHits();
    // 4.1.获取总条数
    long total = searchHits.getTotalHits().value;
    System.out.println("共搜索到" + total + "条数据");
    // 4.2.文档数组
    SearchHit[] hits = searchHits.getHits();
    // 4.3.遍历
    for (SearchHit hit : hits) {
        // 获取文档source
        String json = hit.getSourceAsString();
        // 反序列化
        HotelDoc hotelDoc = JSON.parseObject(json, HotelDoc.class);
        // 获取高亮结果
        Map<String, HighlightField> highlightFields = hit.getHighlightFields();
        if (!CollectionUtils.isEmpty(highlightFields)) {
            // 根据字段名获取高亮结果
            HighlightField highlightField = highlightFields.get("name");
            if (highlightField != null) {
                // 获取高亮值
                String name = highlightField.getFragments()[0].string();
                // 覆盖非高亮结果
                hotelDoc.setName(name);
            }
        }
        System.out.println("hotelDoc = " + hotelDoc);
    }
}
```

- 第一步：从结果中获取source。hit.getSourceAsString()，这部分是非高亮结果，json字符串。还需要反序列为HotelDoc对象
- 第二步：获取高亮结果。hit.getHighlightFields()，返回值是一个Map，key是高亮字段名称，值是HighlightField对象，代表高亮值
- 第三步：从map中根据高亮字段名称，获取高亮字段值对象HighlightField
- 第四步：从HighlightField中获取Fragments，并且转为字符串。这部分就是真正的高亮字符串了
- 第五步：用高亮的结果替换HotelDoc中的非高亮结果





# 数据聚合

https://www.bilibili.com/video/BV1LQ4y127n4?p=120



## 聚合的种类

聚合常见的有三类：

- **桶（Bucket）**聚合：用来对文档做分组
    - TermAggregation：按照文档字段值分组，例如按照品牌值分组、按照国家分组
    - Date Histogram：按照日期阶梯分组，例如一周为一组，或者一月为一组

- **度量（Metric）**聚合：用以计算一些值，比如：最大值、最小值、平均值等
    - Avg：求平均值
    - Max：求最大值
    - Min：求最小值
    - Stats：同时求max、min、avg、sum等
- **管道（pipeline）**聚合：其它聚合的结果为基础做聚合

>**注意：**参加聚合的字段必须是keyword、日期、数值、布尔类型





## DSL实现聚合

- aggs代表聚合，与query同级。此时query的作用是限定聚合的文档范围
- 聚合必须的三要素：
    - 聚合名称
    - 聚合类型
    - 聚合字段
- 聚合可选配置属性：
    - size：指定聚合结果的数量
    - order：指定聚合结果的排序方式
    - field：指定聚合的字段



### Bucket聚合语法

语法如下：

```json
GET /hotel/_search
{
  "size": 0,  // 设置size为0，结果中不包含文档，只包含聚合结果
  "aggs": { // 定义聚合
    "brandAgg": { //给聚合起个名字
      "terms": { // 聚合的类型，按照品牌值聚合，所以选择term
        "field": "brand", // 参与聚合的字段
        "size": 20 // 希望获取的聚合结果数量
      }
    }
  }
}
```



### 聚合结果排序

> 默认情况下，Bucket聚合会统计Bucket内的文档数量，记为\_count，并且按照\_count降序排序。



我们可以指定order属性，自定义聚合的排序方式：

```json
GET /hotel/_search
{
  "size": 0, 
  "aggs": {
    "brandAgg": {
      "terms": {
        "field": "brand",
        "order": {
          "_count": "asc" // 按照_count升序排列
        },
        "size": 20
      }
    }
  }
}
```



### 限定聚合范围

>默认情况下，Bucket聚合是对索引库的所有文档做聚合，但真实场景下，用户会输入搜索条件，因此聚合必须是对搜索结果聚合。那么聚合必须添加限定条件。



我们可以限定要聚合的文档范围，只要添加query条件即可：

```json
GET /hotel/_search
{
  "query": {
    "range": {
      "price": {
        "lte": 200 // 只对200元以下的文档聚合
      }
    }
  }, 
  "size": 0, 
  "aggs": {
    "brandAgg": {
      "terms": {
        "field": "brand",
        "size": 20
      }
    }
  }
}
```



### Metric聚合语法

现在我们需要对桶内的酒店做运算，获取每个品牌的用户评分的min、max、avg等值。

这就要用到Metric聚合了，例如stat聚合：就可以获取min、max、avg等结果。

```json
GET /hotel/_search
{
  "size": 0, 
  "aggs": {
    "brandAgg": { 
      "terms": { 
        "field": "brand", 
        "size": 20
      },
      "aggs": { // 是brands聚合的子聚合，也就是分组后对每组分别计算
        "score_stats": { // 聚合名称
          "stats": { // 聚合类型，这里stats可以计算min、max、avg等
            "field": "score" // 聚合字段，这里是score
          }
        }
      }
    }
  }
}
```

这次的score_stats聚合是在brandAgg的聚合内部嵌套的子聚合。因为我们需要在每个桶分别计算。



## RestAPI实现聚合

### API语法

聚合条件与query条件同级别，因此需要使用request.source()来指定聚合条件。

- 聚合条件的语法：

    <img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323182819417.png" alt="image-20220323182819417" style="zoom: 67%;" />

- 聚合的结果也与查询结果不同，API也比较特殊。不过同样是JSON逐层解析：

    <img src="%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323182831539.png" alt="image-20220323182831539" style="zoom:67%;" />

```java
@SpringBootTest
class HotelSearchTest {

    private RestHighLevelClient client;

    @Test
    public void restAggregation() throws IOException {
        SearchRequest request = new SearchRequest("hotel");

        request.source().size(0);//不需要返回document数据
        request.source().aggregation(AggregationBuilders.terms("brandAgg")//为聚合起一个名称：brandAgg
                .field("brand") //指定需要聚合的字段：brand
                .size(20)       //希望得到最多20个bucket的数据
        );

        SearchResponse response = client.search(request, RequestOptions.DEFAULT);
        Aggregations aggregations = response.getAggregations();
        Terms brandTerms = aggregations.get("brandAgg");
        List<? extends Terms.Bucket> buckets = brandTerms.getBuckets();
        buckets.forEach(bucket -> {
            String key = bucket.getKeyAsString();
            long docCount = bucket.getDocCount();
            System.out.println(key+" "+docCount);
        });
    }
}
```



### 聚合的案例

需求：在搜索页面中，品牌、城市等信息应是通过聚合索引库中的酒店数据得来的，而非是耦合在静态页面中。即实现如下接口：
```java
/*
	查询城市、星级、品牌的聚合结果
	@return 聚合结果，格式：{"城市"：["上海","北京"],"品牌":["如家","希尔顿"]}
*/
Map<String,List<String>> filters();
```

![image-20220323184435999](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323184435999.png)



```java
@Service
public class HotelService extends ServiceImpl<HotelMapper, Hotel> implements IHotelService {

    @Autowired
    private RestHighLevelClient restHighLevelClient;


    @Override
    public Map<String, List<String>> filters() {
        SearchRequest request = new SearchRequest("hotel");

        request.source().size(0);//不需要返回document数据
        buildAggregation(request,"brandAgg","brand",20);
        buildAggregation(request,"cityAgg","city",20);
        buildAggregation(request,"starAgg","starName",20);

        SearchResponse response = null;
        try {
            response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        assert response != null;
        Aggregations aggregations = response.getAggregations();
        Map<String, List<String>> result = new HashMap<>();
        
        List<String> brandList = getAggByName(aggregations,"brandAgg");
        result.put("品牌",brandList);
        List<String> cityList = getAggByName(aggregations,"cityAgg");
        result.put("城市",cityList);
        List<String> starList = getAggByName(aggregations,"starAgg");
        result.put("星级",starList);

        return result;
    }

    private void buildAggregation(SearchRequest request,String name, String field ,int size) {
        request.source().aggregation(AggregationBuilders.terms(name)//为聚合起一个名称：brandAgg
                .field(field) //指定需要聚合的字段：brand
                .size(size)       //希望得到最多20个bucket的数据
        );
    }

    private List<String> getAggByName(Aggregations aggregations,String aggName) {
        List<String> brandList = new ArrayList<>();
        Terms brandTerms = aggregations.get(aggName);
        List<? extends Terms.Bucket> buckets = brandTerms.getBuckets();
        buckets.forEach(bucket -> {
            brandList.add(bucket.getKeyAsString());
        });
        return brandList;
    }
}
```



# 自动补全

## 安装拼音分词器

步骤：

1. 解压

2. 上传到虚拟机中，elasticsearch的plugin目录

3. 重启elasticsearch

4. 测试：

    ```json
    POST /_analyze
    {
      "text": "如家酒店还不错",
      "analyzer": "pinyin"
    }
    ```

    

## 自定义分词器

默认的拼音分词器会将每个汉字单独分为拼音，而我们希望的是每个词条形成一组拼音，需要对拼音分词器做个性化定制，形成自定义分词器。



elasticsearch中分词器（analyzer）的组成包含三部分：

- character filters：在tokenizer之前对文本进行处理。例如删除字符、替换字符
- tokenizer：将文本按照一定的规则切割成词条（term）。例如keyword，就是不分词；还有ik_smart
- tokenizer filter：将tokenizer输出的词条做进一步处理。例如大小写转换、同义词处理、拼音处理等

![image-20220323192149250](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323192149250.png)



声明自定义分词器的语法如下：

```json
PUT /test
{
  "settings": {
    "analysis": {
      "analyzer": { // 自定义分词器
        "my_analyzer": {  // 分词器名称
          "tokenizer": "ik_max_word",
          "filter": "py"
        }
      },
      "filter": { // 自定义tokenizer filter
        "py": { // 过滤器名称
          "type": "pinyin", // 过滤器类型，这里是pinyin
		  "keep_full_pinyin": false,
          "keep_joined_full_pinyin": true,
          "keep_original": true,
          "limit_first_letter_length": 16,
          "remove_duplicated_term": true,
          "none_chinese_pinyin_tokenize": false
        }
      }
    }
  },
  "mappings": {
    "properties": {
      "name": {
        "type": "text",
        "analyzer": "my_analyzer",//创建倒排索引用分词器
        "search_analyzer": "ik_smart"//搜索用分词器
      }
    }
  }
}
```

> 此配置在创建索引库时指定。故只对当前索引库有效



> 注意，拼音分词器适合在创建倒排索引时使用；但不要在搜索时使用

![image-20220323192910562](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323192910562.png)



## 自动补全查询

elasticsearch提供了[Completion Suggester](https://www.elastic.co/guide/en/elasticsearch/reference/7.6/search-suggesters.html)查询来实现自动补全功能。这个查询会匹配以用户输入内容开头的词条并返回。为了提高补全查询的效率，对于文档中字段的类型有一些约束：

- 参与补全查询的字段必须是completion类型。
- 字段的内容一般是用来补全的多个词条形成的数组。



例如：

- 对如下的索引库：

    ```json
    // 创建索引库
    PUT test
    {
      "mappings": {
        "properties": {
          "title":{
            "type": "completion"
          }
        }
      }
    }
    ```

- 插入下面的数据：

    ```json
    // 示例数据
    POST test/_doc
    {
      "title": ["Sony", "WH-1000XM3"]
    }
    POST test/_doc
    {
      "title": ["SK-II", "PITERA"]
    }
    POST test/_doc
    {
      "title": ["Nintendo", "switch"]
    }
    ```

其对应的DSL语句如下：

```json
// 自动补全查询
GET /test/_search
{
  "suggest": {
    "title_suggest": {
      "text": "s", // 关键字
      "completion": {
        "field": "title", // 补全查询的字段
        "skip_duplicates": true, // 跳过重复的
        "size": 10 // 获取前10条结果
      }
    }
  }
}
```



## 自动补全的RestAPI

- 查询：

    ![image-20220323195250076](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323195250076.png)

- 解析：

    ![image-20220323195254496](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323195254496.png)

```json
@Override
public List<String> getSuggestions(String prefix) {
    try {
        // 1.准备Request
        SearchRequest request = new SearchRequest("hotel");
        // 2.准备DSL
        request.source().suggest(new SuggestBuilder().addSuggestion(
            "suggestions",
            SuggestBuilders.completionSuggestion("suggestion")
            .prefix(prefix)
            .skipDuplicates(true)
            .size(10)
        ));
        // 3.发起请求
        SearchResponse response = client.search(request, RequestOptions.DEFAULT);
        // 4.解析结果
        Suggest suggest = response.getSuggest();
        // 4.1.根据补全查询名称，获取补全结果
        CompletionSuggestion suggestions = suggest.getSuggestion("suggestions");
        // 4.2.获取options
        List<CompletionSuggestion.Entry.Option> options = suggestions.getOptions();
        // 4.3.遍历
        List<String> list = new ArrayList<>(options.size());
        for (CompletionSuggestion.Entry.Option option : options) {
            String text = option.getText().toString();
            list.add(text);
        }
        return list;
    } catch (IOException e) {
        throw new RuntimeException(e);
    }
}
```





# 数据同步

## 问题分析

elasticsearch中的酒店数据来自于mysql数据库，因此mysql数据发生改变时，elasticsearch也必须跟着改变，这个就是elasticsearch与mysql之间的**数据同步**。

但在微服务中，负责酒店管理(操作MySQL)的业务，与负责酒店搜索(操作elasticsearch)的业务==往往会在两个不同的微服务上==。

## 解决方案

常见的数据同步解决方案有三种：

- 同步调用：

    ![image-20220323205136346](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323205136346.png)

    hotel-demo对外提供接口，用来修改elasticsearch中的数据

    酒店管理服务在完成数据库操作后，直接调用hotel-demo提供的接口

- 异步通知：

    ![image-20220323205208536](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323205208536.png)

    hotel-admin对mysql数据库数据完成增、删、改后，发送MQ消息

    hotel-demo监听MQ，接收到消息后完成elasticsearch数据修改

- 监听MySQL的binlog：

    ![image-20220323205234967](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323205234967.png)

    给mysql开启binlog功能

    mysql完成增、删、改操作都会记录在binlog中

    hotel-demo基于canal监听binlog变化，实时更新elasticsearch中的内容

    

方式一：同步调用

- 优点：实现简单，粗暴
- 缺点：业务耦合度高

方式二：异步通知

- 优点：低耦合，实现难度一般
- 缺点：依赖mq的可靠性

方式三：监听binlog

- 优点：完全解除服务间耦合
- 缺点：开启binlog增加数据库负担、实现复杂度高



## 异步通知方式

步骤：

1. 导入课前资料提供的hotel-admin项目，启动并测试酒店数据的CRUD

    >访问 http://localhost:8099

2. 声明exchange、queue、RoutingKey

3. 在hotel-admin中的增、删、改业务中完成消息发送

4. 在hotel-demo中完成消息监听，并更新elasticsearch中数据

5. 启动并测试数据同步功能



### 声明交换机、队列

MQ结构如图：

![image-20220323210559789](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220323210559789.png)



步骤如下：

1. 引入依赖

    ```xml
    <!--amqp-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-amqp</artifactId>
    </dependency>
    ```

2. 配置AMQP

    ```yaml
    spring:
      rabbitmq:
        host: 192.168.88.128
        port: 5672
        username: itcast
        password: 123321
        virtual-host: /
    ```

3. 声明交换机、队列

    ```java
    package cn.itcast.hotel.constatnts;
    
    public class MqConstants {
        /**
         * 交换机
         */
        public final static String HOTEL_EXCHANGE = "hotel.topic";
        /**
         * 监听新增和修改的队列
         */
        public final static String HOTEL_INSERT_QUEUE = "hotel.insert.queue";
        /**
         * 监听删除的队列
         */
        public final static String HOTEL_DELETE_QUEUE = "hotel.delete.queue";
        /**
         * 新增或修改的RoutingKey
         */
        public final static String HOTEL_INSERT_KEY = "hotel.insert";
        /**
         * 删除的RoutingKey
         */
        public final static String HOTEL_DELETE_KEY = "hotel.delete";
    }
    ```

    ```java
    package cn.itcast.hotel.config;
    
    import cn.itcast.hotel.constants.MqConstants;
    import org.springframework.amqp.core.Binding;
    import org.springframework.amqp.core.BindingBuilder;
    import org.springframework.amqp.core.Queue;
    import org.springframework.amqp.core.TopicExchange;
    import org.springframework.context.annotation.Bean;
    import org.springframework.context.annotation.Configuration;
    
    @Configuration
    public class MqConfig {
        @Bean
        public TopicExchange topicExchange(){
            return new TopicExchange(MqConstants.HOTEL_EXCHANGE, true, false);
        }
    
        @Bean
        public Queue insertQueue(){
            return new Queue(MqConstants.HOTEL_INSERT_QUEUE, true);
        }
    
        @Bean
        public Queue deleteQueue(){
            return new Queue(MqConstants.HOTEL_DELETE_QUEUE, true);
        }
    
        @Bean
        public Binding insertQueueBinding(){
            return BindingBuilder.bind(insertQueue()).to(topicExchange()).with(MqConstants.HOTEL_INSERT_KEY);
        }
    
        @Bean
        public Binding deleteQueueBinding(){
            return BindingBuilder.bind(deleteQueue()).to(topicExchange()).with(MqConstants.HOTEL_DELETE_KEY);
        }
    }
    ```

    



### 发送MQ消息

在hotel-admin中的增、删、改业务中分别发送MQ消息：

```java
@RestController
@RequestMapping("hotel")
public class HotelController {

    @Autowired
    private IHotelService hotelService;

    @Autowired
    private RabbitTemplate rabbitTemplate;

    @PostMapping
    public void saveHotel(@RequestBody Hotel hotel){
        hotelService.save(hotel);

        rabbitTemplate.convertAndSend(MqConstants.HOTEL_EXCHANGE,MqConstants.HOTEL_INSERT_KEY,hotel.getId());
    }

    @PutMapping()
    public void updateById(@RequestBody Hotel hotel){
        if (hotel.getId() == null) {
            throw new InvalidParameterException("id不能为空");
        }
        hotelService.updateById(hotel);

        rabbitTemplate.convertAndSend(MqConstants.HOTEL_EXCHANGE,MqConstants.HOTEL_INSERT_KEY,hotel.getId());

    }

    @DeleteMapping("/{id}")
    public void deleteById(@PathVariable("id") Long id) {
        hotelService.removeById(id);

        rabbitTemplate.convertAndSend(MqConstants.HOTEL_EXCHANGE,MqConstants.HOTEL_DELETE_KEY,id);
    }
}
```



### 监听MQ消息

当hotel-demo接收到

- 新增消息：根据id查询数据库，然后向索引库增加数据
- 删除消息：从索引库中删除数据

> hotel-demo是专门操作es的微服务项目



监听器如下：

```java
package cn.itcast.hotel.mq;

import cn.itcast.hotel.constants.MqConstants;
import cn.itcast.hotel.service.IHotelService;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class HotelListener {

    @Autowired
    private IHotelService hotelService;

    /**
     * 监听酒店新增或修改的业务
     * @param id 酒店id
     */
    @RabbitListener(queues = MqConstants.HOTEL_INSERT_QUEUE)
    public void listenHotelInsertOrUpdate(Long id){
        hotelService.insertById(id);
    }

    /**
     * 监听酒店删除的业务
     * @param id 酒店id
     */
    @RabbitListener(queues = MqConstants.HOTEL_DELETE_QUEUE)
    public void listenHotelDelete(Long id){
        hotelService.deleteById(id);
    }
}
```



业务层代码如下：

```java
public class HotelService extends ServiceImpl<HotelMapper, Hotel> implements IHotelService {

    @Autowired
    private RestHighLevelClient restHighLevelClient;
    
    @Override
    public void deleteById(Long id) {
        try {
            // 1.准备Request
            DeleteRequest request = new DeleteRequest("hotel", id.toString());
            // 2.发送请求
            client.delete(request, RequestOptions.DEFAULT);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void insertById(Long id) {
        try {
            // 0.根据id查询酒店数据
            Hotel hotel = getById(id);
            // 转换为文档类型
            HotelDoc hotelDoc = new HotelDoc(hotel);

            // 1.准备Request对象
            IndexRequest request = new IndexRequest("hotel").id(hotel.getId().toString());
            // 2.准备Json文档
            request.source(JSON.toJSONString(hotelDoc), XContentType.JSON);
            // 3.发送请求
            client.index(request, RequestOptions.DEFAULT);
        } catch (IOException e) {
            throw new RuntimeException(e);
	}
}
```





# ES集群

https://www.bilibili.com/video/BV1LQ4y127n4?p=138

## 集群职责划分

elasticsearch中集群节点有不同的职责划分：

![image-20220324200306136](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220324200306136.png)

>默认情况下，集群中的任何一个节点都同时具备上述四种角色。



但是真实的集群一定要将集群职责分离：

- master节点：对CPU要求高，但是内存要求低
- data节点：对CPU和内存要求都高
- coordinating节点：对网络带宽、CPU要求高



## 脑裂问题

脑裂是因为集群中的节点失联导致的。



例如一个集群中，主节点与其它节点失联：

![image-20220324200355762](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220324200355762.png)

此时，node2和node3认为node1宕机，就会重新选主：

![image-20220324200402220](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220324200402220.png)

当node3当选后，集群继续对外提供服务，node2和node3自成集群，node1自成集群，两个集群数据不同步，出现数据差异。

当网络恢复后，因为集群中有两个master节点，集群状态的不一致，出现脑裂的情况：

![image-20220324200414304](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220324200414304.png)



解决脑裂的方案是，要求选票超过 ( eligible节点数量 + 1 ）/ 2 才能当选为主，因此eligible节点数量最好是奇数。对应配置项是discovery.zen.minimum_master_nodes，在es7.0以后，已经成为默认配置，因此一般不会发生脑裂问题

例如：3个节点形成的集群，选票必须超过 （3 + 1） / 2 ，也就是2票。node3得到node2和node3的选票，当选为主。node1只有自己1票，没有当选。集群中依然只有1个主节点，没有出现脑裂。



## 集群分布式存储

elasticsearch会通过hash算法来计算文档应该存储到哪个分片：

![image-20220324200643074](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220324200643074.png)

其中_routing默认是文档的id

>算法与分片数量有关，因此索引库一旦创建，分片数量不能修改！



新增文档的流程如下：

![image-20220324200703927](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220324200703927.png)

- 1）新增一个id=1的文档
- 2）对id做hash运算，假如得到的是2，则应该存储到shard-2
- 3）shard-2的主分片在node3节点，将数据路由到node3
- 4）保存文档
- 5）同步给shard-2的副本replica-2，在node2节点
- 6）返回结果给coordinating-node节点



## 集群分布式查询

elasticsearch的查询分成两个阶段：

- scatter phase：分散阶段，coordinating node会把请求分发到每一个分片

- gather phase：聚集阶段，coordinating node汇总data node的搜索结果，并处理为最终结果集返回给用户

![image-20220324200735780](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20220324200735780.png)



## 集群故障转移

集群的master节点会监控集群中的节点状态，如果发现有节点宕机，会立即将宕机节点的分片数据迁移到其它节点，确保数据安全，这个叫做故障转移。



1）例如一个集群结构如图：

![image-20210723225945963](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210723225945963.png)

现在，node1是主节点，其它两个节点是从节点。



2）突然，node1发生了故障：

![image-20210723230020574](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210723230020574.png)



宕机后的第一件事，需要重新选主，例如选中了node2：

![image-20210723230055974](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210723230055974.png)



node2成为主节点后，会检测集群监控状态，发现：shard-1、shard-0没有副本节点。因此需要将node1上的数据迁移到node2、node3：

![image-20210723230216642](%E5%88%86%E5%B8%83%E5%BC%8F%E6%90%9C%E7%B4%A2%E5%BC%95%E6%93%8Eelasticsearch.assets/image-20210723230216642.png)

