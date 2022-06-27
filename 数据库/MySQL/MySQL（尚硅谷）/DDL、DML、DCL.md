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

第10章
