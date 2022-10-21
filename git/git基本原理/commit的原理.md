# 执行commit后多出来的文件

- 多出来一些objects：
  - commit object
  - tree object
  - parent object
- 多出来logs
- 多出来refs/heads/master

- .git/HEAD改变

  > ==其永远指向工作分区==。



![image-20221019235104919](commit%E7%9A%84%E5%8E%9F%E7%90%86.assets/image-20221019235104919.png)



# 关于文件夹

- add的时候：
  - 文件夹本身无对应对象；文件占用一个对象
- commit的时候：
  - ==文件夹将对应一个tree对象==。

![image-20221020000748118](commit%E7%9A%84%E5%8E%9F%E7%90%86.assets/image-20221020000748118.png)





# git diff命令

用于比对工作区和索引区。

- `@@`用于划分一个个block。
- `-4.7`：`-`是指原文件，4是始于第4行，7是向下数7行。

> `git diff --cached`用于比对索引区和仓库。







