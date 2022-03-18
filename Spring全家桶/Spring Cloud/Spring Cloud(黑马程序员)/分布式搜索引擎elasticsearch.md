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

## 安装es、kibana

- 安装：见"安装elasticsearch.md"
- 分词器：同上



