# 内容

- 创建和管理表
- 数据处理之增删改
- MySQL数据类型
- 约束





# 显示表结构

使用DESCRIBE 或 DESC 命令，表示表结构。

```sql
DESCRIBE employees;
或
DESC employees;
```

```sql
mysql> DESC employees;
+----------------+-------------+------+-----+---------+-------+
| Field          | Type        | Null | Key | Default | Extra |
+----------------+-------------+------+-----+---------+-------+
| employee_id    | int         | NO   | PRI | 0       |       |
| first_name     | varchar(20) | YES  |     | NULL    |       |
| last_name      | varchar(25) | NO   |     | NULL    |       |
| email          | varchar(25) | NO   | UNI | NULL    |       |
| phone_number   | varchar(20) | YES  |     | NULL    |       |
| hire_date      | date        | NO   |     | NULL    |       |
| job_id         | varchar(10) | NO   | MUL | NULL    |       |
| salary         | double(8,2) | YES  |     | NULL    |       |
| commission_pct | double(2,2) | YES  |     | NULL    |       |
| manager_id     | int         | YES  | MUL | NULL    |       |
| department_id  | int         | YES  | MUL | NULL    |       |
+----------------+-------------+------+-----+---------+-------+
11 rows in set (0.00 sec)
```

- Field：表示字段名称
- Type：表示字段类型表  
-  Null：表示该列是否可以存储NULL值
- Key：表示该列是否已编制索引。
  - PRI表示该列是表主键的一部分；
  - UNI表示该列是UNIQUE索引的一部分；
  - MUL表示在列中某个给定值允许出现多次

- Default：表示该列是否有默认值，如果有，那么值是多少
- Extra：表示可以获取的与给定列有关的附加信息，例如AUTO_INCREMENT等



# 创建和管理表

## 标识符命名规则

- 数据库名、表名不得超过30个字符，变量名限制为29个
- 必须只能包含 A–Z, a–z, 0–9, _共63个字符
- 同一个MySQL软件中，数据库不能同名；同一个库中，表不能重名；同一个表中，字段不能重名
- 必须保证你的字段没有和保留字、数据库系统或常用方法冲突。如果坚持使用，请在SQL语句中使用`（着重号）引起来
- 保持字段名和类型的一致性：在命名字段并为其指定数据类型的时候一定要保证一致性。假如数据类型在一个表里是整数，那在另一个表里可就别变成字符型了  



## MySQL数据类型

| 类型              | 类型举例                                                     |
| ----------------- | ------------------------------------------------------------ |
| 整数类型          | TINYINT、SMALLINT、MEDIUMINT、INT(或INTEGER)、BIGINT         |
| 浮点类型          | FLOAT、DOUBLE                                                |
| 定点数类型        | DECIMAL                                                      |
| 位类型            | BIT                                                          |
| 日期时间类型      | YEAR、TIME、DATE、DATETIME、TIMESTAMP                        |
| 文本字符串类型    | CHAR、VARCHAR、TINYTEXT、TEXT、MEDIUMTEXT、LONGTEXT          |
| 枚举类型          | ENUM                                                         |
| 集合类型          | SET                                                          |
| 二进制字符串类 型 | BINARY、VARBINARY、TINYBLOB、BLOB、MEDIUMBLOB、LONGBLOB      |
| JSON类型          | JSON对象、JSON数组                                           |
| 空间数据类型      | 单值：GEOMETRY、POINT、LINESTRING、POLYGON； 集合：MULTIPOINT、MULTILINESTRING、MULTIPOLYGON、 GEOMETRYCOLLECTION |



其中， 常用的几类类型介绍如下：

| 数据类型      | 描述                                                         |
| ------------- | ------------------------------------------------------------ |
| INT           | 从-2^31^到2^31^-1的整型数据。存储大小为 4个字节              |
| CHAR(size)    | 定长字符数据。若未指定，默认为1个字符，最大长度255           |
| VARCHAR(size) | 可变长字符数据，根据字符串实际长度保存，必须指定长度         |
| FLOAT(M,D)    | 单精度，占用4个字节，M=整数位+小数位，D=小数位。 D<=M<=255,0<=D<=30， 默认M+D<=6 |
| DOUBLE(M,D)   | 双精度，占用8个字节，D<=M<=255,0<=D<=30，默认M+D<=15         |
| DECIMAL(M,D)  | 高精度小数，占用M+2个字节，D<=M<=65，0<=D<=30，最大取值范围与DOUBLE 相同。 |
| DATE          | 日期型数据，格式'YYYY-MM-DD'                                 |
| BLOB          | 二进制形式的长文本数据，最大可达4G                           |
| TEXT          | 长文本数据，最大可达4G                                       |



## 数据库

### 创建数据库

- 创建数据库：

  ```sql
  CREATE DATABASE 数据库名;
  ```

- 创建数据库并指定字符集：

  ```sql
  CREATE DATABASE 数据库名 CHARACTER SET 字符集;
  ```

- 判断数据库是否已经存在，不存在则创建数据库（ `推荐` ）：

  ```sql
  CREATE DATABASE IF NOT EXISTS 数据库名;
  ```



### 使用数据库

- 查看所有的数据库：

  ```sql
  SHOW DATABASES;
  ```

- 查看当前正在使用的数据库：

  ```sql
  SELECT DATABASE(); #使用的一个 mysql 中的全局函数
  ```

- 查看指定库下所有的表：

  ```sql
  SHOW TABLES FROM 数据库名;
  ```

- 查看数据库的创建信息：

  ```sql
  SHOW CREATE DATABASE 数据库名;
  ```

- 使用/切换数据库：

  ```sql
  USE 数据库名;
  ```



### 修改数据库

- 修改数据库字符集：

  ```sql
  ALTER DATABASE 数据库名 CHARACTER SET 字符集; #比如：gbk、utf8等
  ```

  

### 删除数据库

- 删除指定的数据库：

  ```sql
  DROP DATABASE 数据库名;
  ```

- 删除指定的数据库（推荐）：

  ```sql
  DROP DATABASE IF EXISTS 数据库名;
  ```

  

## 数据表

### 创建表

> 创表的前提：
>
> - 具备Create Table权限
> - 存储空间



语法：

```sql
CREATE TABLE [IF NOT EXISTS] 表名(
字段1, 数据类型 [约束条件] [默认值],
字段2, 数据类型 [约束条件] [默认值],
字段3, 数据类型 [约束条件] [默认值],
……
[表约束条件]
);
```



例如：

```sql
-- 创建表
CREATE TABLE emp (
    -- int类型
    emp_id INT,
    -- 最多保存20个中英文字符
    emp_name VARCHAR(20),
    -- 总位数不超过15位
    salary DOUBLE,
    -- 日期类型
    birthday DATE
);
```



创建表并插入数据：

![image-20220628141138997](DDL%E3%80%81DML%E3%80%81DCL.assets/image-20220628141138997.png)

> 指定的列，要和子查询中的列一一对应

```sql
CREATE TABLE dept80
AS
SELECT employee_id, last_name, salary*12 ANNSAL, hire_date
FROM employees
WHERE department_id = 80;
```



### 查看表结构

- `DESCRIBE/DESC`

- `SHOW CREATE TABLE `

  ```sql
  SHOW CREATE TABLE 表名;
  ```

  >使用SHOW CREATE TABLE语句不仅可以查看表创建时的详细语句，**还可以查看存储引擎和字符编码**。



### 修改表

使用 `ALTER TABLE` 语句可以实现：

- 向已有的表中添加列

  ```sql
  ALTER TABLE 表名 ADD 【COLUMN】 字段名 字段类型 【FIRST|AFTER 字段名】;
  ```

  ```sql
  ALTER TABLE dept80
  ADD job_id varchar(15);
  ```

- 修改现有表中的列（数据类型，长度、默认值和位置  ）：

  ```sql
  ALTER TABLE 表名 MODIFY 【COLUMN】 字段名1 字段类型 【DEFAULT 默认值】【FIRST|AFTER 字段名】;
  ```

  ```sql
  ALTER TABLE dept80
  MODIFY salary double(9,2) default 1000;
  ```

- 重命名现有表中的列：

  ```sql
  ALTER TABLE 表名 CHANGE 【column】 列名 新列名 新数据类型;
  ```

  ```sql
  ALTER TABLE dept80
  CHANGE department_name dept_name varchar(15);
  ```

- 删除现有表中的列：

  ```sql
  ALTER TABLE 表名 DROP 【COLUMN】字段名
  ```

  ```sql
  ALTER TABLE dept80
  DROP COLUMN job_id;
  ```

- 重命名表：

  ```sql
  RENAME TABLE emp
  TO myemp
  ```

  或：

  ```sql
  ALTER table dept
  RENAME [TO] detail_dept; -- [TO]可以省略
  ```

  

### 删除表

- 在MySQL中，当一张数据表 `没有与其他任何数据表形成关联关系` 时，可以将当前数据表直接删除。

- 数据和结构都被删除
- 所有正在运行的相关事务被提交
- 所有相关索引被删除



```sql
DROP TABLE [IF EXISTS] 数据表1 [, 数据表2, …, 数据表n];
```

例如：

```sql
DROP TABLE dept80;
```

> ##### 💡 DROP TABLE 语句不能回滚



### 清空表

`TRUNCATE TABLE`语句：

- 删除表中所有的数据
- 释放表的存储空间

例如：

```sql
TRUNCATE TABLE detail_dept;
```



`DELETE`语句：

```sql
SET autocommit = FALSE;

DELETE FROM emp2;
#TRUNCATE TABLE emp2;

SELECT * FROM emp2;

ROLLBACK;
SELECT * FROM emp2;
```

> 注意：
>
> - TRUNCATE TABLE：
>
>   不能回滚
>
> - DELETE：
>
>   不带 WHERE 子句的 DELETE 语句，与TRUNCATE功能相同
>
>   但DELETE可以回滚。



# 数据处理之增删改

## 插入数据

- 为表的所有字段**按默认顺序**插入数据：

  ```sql
  INSERT INTO 表名
  VALUES (value1,value2,....);
  ```

- 为表的**指定字段**插入数据：

  ```sql
  INSERT INTO 表名(column1 [, column2, …, columnn])
  VALUES (value1 [,value2, …, valuen]);
  ```

- 同时插入**多条记录**：

  ```SQL
  INSERT INTO table_name
  VALUES
  (value1 [,value2, …, valuen]),
  (value1 [,value2, …, valuen]),
  ...
  (value1 [,value2, …, valuen]);
  ```

  或者：

  ```sql
  INSERT INTO table_name(column1 [, column2, …, columnn])
  VALUES
  (value1 [,value2, …, valuen]),
  (value1 [,value2, …, valuen]),
  ……
  (value1 [,value2, …, valuen]);
  ```

  

- 将查询结果，插入到表中：

  ```sql
  INSERT INTO 目标表名
  (tar_column1 [, tar_column2, …, tar_columnn])
  SELECT
  (src_column1 [, src_column2, …, src_columnn])
  FROM 源表名
  [WHERE condition]
  ```

  例如：

  ```sql
  INSERT INTO sales_reps(id, name, salary, commission_pct)
  SELECT employee_id, last_name, salary, commission_pct
  FROM employees
  WHERE job_id LIKE '%REP%';
  ```

  

## 更新数据

```sql
UPDATE table_name
SET column1=value1, column2=value2, … , column=valuen
[WHERE condition]
```



## 删除数据

```sql
DELETE FROM table_name [WHERE <condition>];
```



> 如果省略 WHERE 子句，则表中的全部数据将被删除  
>
> ```sql
> DELETE FROM copy_emp;
> ```



# MySQL数据类型

12章



