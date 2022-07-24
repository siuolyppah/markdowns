# 相关资料

- [dunwu/nginx-tutorial: 这是一个 Nginx 极简教程，目的在于帮助新手快速入门 Nginx。 (github.com)](https://github.com/dunwu/nginx-tutorial)



# 什么是反向代理

反向代理（Reverse Proxy）方式是指，以代理服务器来接受 internet 上的连接请求，然后**将请求转发给内部网络上的服务器**，并将从服务器上得到的结果返回给 internet 上请求连接的客户端，此时代理服务器对外就表现为一个反向代理服务器。



# Docker相关

- 官网镜像：https://hub.docker.com/_/nginx/

- 下载镜像：`docker pull nginx`

- 启动容器：

  ```sh
  docker run --name my-nginx -p 80:80 -v /data/docker/nginx/logs:/var/log/nginx -v /data/docker/nginx/conf/nginx.conf:/etc/nginx/nginx.conf:ro -d nginx
  ```

- 重新加载配置（目前测试无效，只能重启服务）：`docker exec -it my-nginx nginx -s reload`

- 停止服务：`docker exec -it my-nginx nginx -s stop` 或者：`docker stop my-nginx`

- 重新启动服务：`docker restart my-nginx`



> nginx.conf一开始不存在的解决方案：先不绑定数据卷，复制一份出来。
>
> ```sh
> docker cp my-nginx:/etc/nginx/nginx.conf /data/docker/nginx/conf/nginx.conf
> docker cp my-nginx:/etc/nginx/conf.d /data/docker/nginx/conf/conf.d
> ```
>
>
> ```nginx
> 
> user  nginx;
> worker_processes  auto;
> 
> error_log  /var/log/nginx/error.log notice;
> pid        /var/run/nginx.pid;
> 
> 
> events {
>     worker_connections  1024;
> }
> 
> 
> http {
>     include       /etc/nginx/mime.types;
>     default_type  application/octet-stream;
> 
>     log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
>                       '$status $body_bytes_sent "$http_referer" '
>                       '"$http_user_agent" "$http_x_forwarded_for"';
> 
>     access_log  /var/log/nginx/access.log  main;
> 
>     sendfile        on;
>     #tcp_nopush     on;
> 
>     keepalive_timeout  65;
> 
>     #gzip  on;
> 
>     include /etc/nginx/conf.d/*.conf;
> }
> 
> ```



>在 C:\Windows\System32\drivers\etc 目录下的 host 文件中添加一条 DNS 记录:
>```
>127.0.0.1 www.helloworld.com
>```



浏览器中访问 [www.helloworld.com](http://www.helloworld.com/)



