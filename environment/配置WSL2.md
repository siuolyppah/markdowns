# 相关资料

- [Windows10开发环境搭建(2) | Terminal和VS Code_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1Zz4y167Vo/?spm_id_from=333.788.recommend_more_video.-1&vd_source=be746efb77e979ca275e4f65f2d8cda3)







# 流程

## 令vscode更强大

1. 安装wsl2：

   [Manual installation steps for older versions of WSL | Microsoft Docs](https://docs.microsoft.com/en-us/windows/wsl/install-manual)

2. 微软商店安装：Ubuntu20.04

1. 微软商店安装：Windows Terminal

   ![image-20220720214426423](%E9%85%8D%E7%BD%AEWSL2.assets/image-20220720214426423.png)

2. VS Code，安装插件：Remote-WSL

   ![image-20220720214929793](%E9%85%8D%E7%BD%AEWSL2.assets/image-20220720214929793.png)



## docker

安装Docker：

1. 在Ubuntu20.04中，执行如下shell脚本：

   docker-install.sh

   ```shell
   # install docker
   curl -fsSL https://get.docker.com -o get-docker.sh
   sudo sh get-docker.sh
   
   if [! $(getent group docker) ];
   then
       sudo groupadd docker;
   else
       echo "docker user group already exists"
   fi
   
   sudo gpasswd -a $USER docker
   sudo service docker restart
   
   rm -rf get-docker.sh
   ```

2. 执行：

   ```shell
   sh ./docker-install.sh
   ```

3. 测试：

   ```shell
   docker version
   ```



启动docker服务：

```C++
sudo service docker start
```



开机自启docker服务：

```shell
sudo systemctl enable docker
```



## 配置zsh

> Ubuntu，默认是bash。



在Ubuntu系统中：

1. 安装zsh：

   ```sh
   sudo apt install zsh
   ```

   > 此时terminal中输入`zsh`，即可进入zsh。

2. 安装oh-my-zsh：

   >https://github.com/ohmyzsh/ohmyzsh
   >
   >[(29条消息) 使用 curl/git 命令时出现 Failed to connect to XXX port 443: 拒绝连接_张学徒的博客-CSDN博客](https://blog.csdn.net/qq_37280924/article/details/107822696)

   二选一：

   ```sh
   sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
   ```

   ```sh
   sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
   ```

3. 修改主题：

   >```sh
   >ls -la
   >```

   ```sh
   code .zshrc
   ```

4. 安装插件：

   - 已集成的插件：

     ```sh
     # 根目录下
     cd .oh-my-zsh/
     cd plugins
     ls # 将列出所有已集成的插件
     ```

     已集成的插件，只要在.zshrc文件中配置即可。

   - 需要下载的插件：

     >zsh-autosuggestions https://github.com/zsh-users/zsh-autosuggestions

     将它克隆到plugins文件夹，然后进行配置。

     ```sh
     git clone https://github.com/zsh-users/zsh-autosuggestions
     ```

     ```
     plugins=(
         git
         zsh-autosuggestions
     )
     ```

     

   

# Vagrant

https://www.bilibili.com/video/BV14i4y1w7N5



