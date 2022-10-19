[Git基本原理介绍(8)——Branch和HEAD_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1s54y1Q746/?spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)



# Branch是什么

![image-20221020001440577](Branchs.assets/image-20221020001440577.png)



# Head文件是一个特殊的指针

![image-20221020001714703](Branchs.assets/image-20221020001714703.png)

- 总是指向当前active的branch的==**最新**==一次commit



# 分支的相关操作

![image-20221020002337550](Branchs.assets/image-20221020002337550.png)



# git branch创建分支的变化

- 多一个文件：`.git/refs/heads/分支名`

  > 其内容为一个commit object的hash值。



# git check改变Active分支的变化

- `./git/Head`文件的内容发生变化，指向另一个branch文件。

  > 例如指向`.git/refs/heads/master`文件。



# git branch -D强制删除分支的变化

> - 不能对Active分支进行删除。
> - `git branch --delete`会在删除未merge分支时进行提示。

- 少一个分支文件。

  > 但不会删除objects。



# git checkout的变化

>[Git基本原理介绍(10)——checkout特定的commit_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1Ga4y1v7YW/?spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)









