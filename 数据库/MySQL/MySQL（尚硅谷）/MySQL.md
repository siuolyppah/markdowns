# 相关资料

- [视频教程](https://www.bilibili.com/video/BV1iq4y1u7vj?spm_id_from=333.337.search-card.all.click&vd_source=be746efb77e979ca275e4f65f2d8cda3)



# SQL分类

SQL语言在功能上主要分为如下3大类：

- DDL（Data Definition Languages、数据定义语言）：

  这些语句定义了不同的数据库、表、视图、索引等数据库对象，还可以用来创建、删除、修改数据库和数据表的结构

  主要的语句关键字包括 `CREATE` 、 `DROP` 、 `ALTER` 等。

- DML（Data Manipulation Language、数据操作语言），用于添加、删除、更新和查询数据库记录，并检查数据完整性。

  主要的语句关键字包括 `INSERT` 、 `DELETE` 、 `UPDATE` 、 `SELECT` 等。

  >因为查询语句使用的非常的频繁，所以很多人把查询语句单拎出来一类：DQL（数据查询语言）。

- DCL（Data Control Language、数据控制语言），用于定义数据库、表、字段、用户的访问权限和安全级别。

  主要的语句关键字包括 `GRANT` 、 `REVOKE` 、 `COMMIT` 、 `ROLLBACK` 、 `SAVEPOINT` 等。  

  >还有单独将 COMMIT 、 ROLLBACK 取出来称为TCL （Transaction Control Language，事务控制语言）  



# SQL语言的规则与规范

-  SQL可以分行写，并可以添加缩进
- 每条命令以`;`,`\g`,`\G`结束
- 关键字，不允许缩写和分行
- 关于标点符号：
  - 字符串和日期类型的数据，可以使用单引号表示
  - 列的别名，建议使用双引号

- 关于大小写：
  - 数据库名、表名、表别名、字段名、字段别名等都小写
  - SQL 关键字、函数名、绑定变量等都大写

- 关于注释：

  ```sql
  单行注释：#注释文字(MySQL特有的方式)
  单行注释：-- 注释文字(--后面必须包含一个空格。)
  多行注释：/* 注释文字 */
  ```

- 关于命名规则：

  - 只能包含 A–Z, a–z, 0–9, _共63个字符

  - 数据库、表名不得超过30个字符，变量名限制为29个

  - 同一个MySQL软件中，数据库不能同名；同一个库中，表不能重名；同一个表中，字段不能重名

  - 必须保证你的字段没有和保留字、数据库系统或常用方法冲突。

    > 如果已冲突但坚持使用，请在SQL语句中使用`（着重号）引起来  
