[十分钟学会正确的github工作流，和开源作者们使用同一套流程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV19e4y1q7JJ/?spm_id_from=333.999.0.0)



1. 克隆主分支到本地

   ```sh
   git clone
   ```

2. 在本地创建feature-branch，并进行修改

   ```sh
   git checkout -b <branchName>
   ```

3. 在本地进行更改

   ```sh
   git add
   ```

   ```sh
   git commit
   ```

4. 推送

   ```sh
   git push origin <branchName>
   ```

   ![image-20221001142930416](git%E7%9A%84pr.assets/image-20221001142930416.png)

5. 假设此时main branch发生了更新

6. 更新local的main branch：

   ```sh
   git checkout main
   ```

   ```sh
   git pull origin master
   ```

7. 解决feature branch的冲突：

   ```sh
   git checkout <branchName>
   ```

   ```sh
   git rebase main
   ```

   > 以main为基础，在main的基础上再进行上一次的更新。
   >
   > 可能需要手动解决此过程中的冲突。

8. 再次推送特性分支：

   ```sh
   git push -f origin <branchName>
   ```

9. 发起合并到main的请求（==pull request==）

   >即要求main分支，对特性分支进行pull。

10. 主分支的维护者，经过审查，进行拉取：

    ```sh
    squash and merge
    ```

11. 删除remote上的特性分支：

    ```sh
    delete branch
    ```

12. 删除本地的特性分支：
    ```sh
    checkout main
    git branch -D my-feature
    ```

13. 更新本地的main分支：

    ```sh
    git pull origin master
    ```

    









>- 在Disk中，保存了源文件的真实的样貌。
>
>- 文件改动后：
>
>  需要通过add命令，告知git发生了更改（放入暂存区）。
>
>- 当使用commit时，暂存区的更改将同步至硬盘。