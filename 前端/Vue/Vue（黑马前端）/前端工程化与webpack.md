# webpack 的基本使用

## 什么是webpack

- 概念：webpack 是前端项目工程化的具体解决方案。

- 主要功能：它提供了友好的前端模块化开发支持，以及代码压缩混淆、处理浏览器端 JavaScript 的兼容性、性能优化等强大的功能。  



## 创建列表隔行变色项目

1. 新建项目空白目录，并运行 `npm init –y` 命令，初始化包管理配置文件`package.json  `

2. 新建 `src` 源代码目录  

3. 新建 src -> `index.html` 首页和 src -> `index.js` 脚本文件 

4. 初始化首页基本的结构   

   ```html
   <!DOCTYPE html>
   <html lang="en">
       <head>
           <meta charset="UTF-8">
           <title>Title</title>
   
           <!-- <script src="./index.js"></script> -->
           <script src="../dist/main.js"></script>
       </head>
       <body>
           <ul>
               <li>这是第 1 个li</li>
               <li>这是第 2 个li</li>
               <li>这是第 3 个li</li>
               <li>这是第 4 个li</li>
               <li>这是第 5 个li</li>
               <li>这是第 6 个li</li>
               <li>这是第 7 个li</li>
               <li>这是第 8 个li</li>
               <li>这是第 9 个li</li>
           </ul>
       </body>
   </html>
   ```

5. 运行 `npm install jquery –S` 命令，安装 jQuery  

   > -S参数（--save）：
   >
   > - 在安装完成后，将其记录到package.json中的depenencies中
   >
   > > 这是一个缺省的默认参数，可以不写

6. 通过 ES6 模块化的方式导入 jQuery，实现列表隔行变色效果  

   ```js
   // 使用ES6默认化语法，导入jquery
   import $ from 'jquery'
   
   // 定义jquery的入口函数
   $(function () {
       $('li:odd').css('background-color', 'red');
       $('li:even').css('background-color', 'pink');
   })
   ```

   

## 在项目中安装 webpack

```sh
npm install webpack@5.5.1 webpack-cli@4.2.0 -D
```

> -D参数（--save-dev）：
>
> - 在安装完成后，将其记录到package.json中的devDepenencies中



## 在项目中配置 webpack

1. 在项目根目录，创建名为`webpack.config.js`的配置文件：

   ```js
   module.exports = {
       mode: 'development' // mode用于指定构建模式。可选值有：development,production
   }
   ```

2. 在package.json的 script 节点下，新增`dev脚本`：

   ```js
   "scripts": {
       "dev": "webpack" // 通过 npm run执行。如 npm run dev
   }
   ```

3. 在终端中执行`如下命令`，启动webpack进行项目的打包构建

   ```sh
   npm run dev
   ```



### mode的可选值

mode 节点的可选值有两个，分别是：

- development
  - 开发环境
  - 不会对打包生成的文件进行代码压缩和性能优化  
  - 打包速度快，适合在开发阶段使用  
- production
  - 生产环境  
  - 会对打包生成的文件进行代码压缩和性能优化  
  - 打包速度很慢，**仅适合在项目发布阶段使用**  



### webpack.config.js 文件作用

- webpack.config.js 是 webpack 的配置文件。
- webpack 在真正开始打包构建之前，会先读取这个配置文件，从而**基于给定的配置**，对项目进行打包。  

>由于 webpack 是基于 node.js 开发出来的打包工具，因此在它的配置文件中**支持使用 node.js 相关的语法和模块**进行 webpack 的个性化配置。  



### webpack 中的默认约定

在 webpack4.x和5.x 中有如下的默认约定：

- 默认的打包入口文件为 src -> index.js  
- 默认的输出文件路径为 dist -> main.js  

> 显然，可以在webpack.config.js中进行修改：
>
> ```js
> const  path =  require('path') // 导入 node.js 中专门操作路径的模块
> 
> module.exports ={
>     entry: path.join(__dirname,'./src/index.js'),    // 打包入口文件的路径
>     output: {
>         path: path.join(__dirname,'./dist'),    // 输出文件的的存放路径
>         filename: "bundle.js"   // 输出文件的名称
>     }
> }
> ```



## 测试

1. 在index.html中，将引入index.js改为main.js
2. 打开页面测试



# webpack 中的插件

## webpack 插件的作用

通过安装和配置第三方的插件，可以拓展 webpack 的能力，从而让 webpack 用起来更方便。  



最常用的webpack 插件有如下两个：  

- `webpack-dev-server`
  - 类似于 node.js 阶段用到的 nodemon 工具
  - 每当修改了源代码，webpack 会自动进行项目的打包和构建  
- `html-webpack-plugin`
  - webpack 中的 HTML 插件（类似于一个模板引擎插件）  
  - 可以通过此插件自定制 index.html 页面的内容  



## webpack-dev-server

webpack-dev-server 可以让 webpack 监听项目源代码的变化，从而进行自动打包构建。



1. 安装webpack-dev-server：

   ```sh
   npm install webpack-dev-server@3.11.0 -D
   ```

   >```json
   >  "devDependencies": {
   >    "webpack": "^5.42.1",
   >    "webpack-cli": "^4.10.0",
   >    "webpack-dev-server": "^3.11.2"
   >  }
   >```

2. 配置webpack-dev-server：

   1. 修改`package.json`->`scripts`中的dev命令：

      ```json
      "scripts":{
          "dev": "webpack serve"
      }
      ```

   2. 再次运行 npm run dev 命令，重新进行项目的打包

   3. 测试：在浏览器中访问 http://localhost:8080 地址，查看自动打包效果  

      >webpack-dev-server 会启动一个实时打包的 http 服务器  



打包生成的文件在哪：

- 不配置 webpack-dev-server 的情况下，webpack 打包生成的文件，会存放到实际的物理磁盘上

  - 严格遵守开发者在 webpack.config.js 中指定配置
  - 根据 output 节点指定路径进行存放  

- 配置了 webpack-dev-server 之后，打包生成的文件存放到了**内存**中

  - 不再根据 output 节点指定的路径，存放到实际的物理磁盘上
  - 提高了实时打包输出的性能，因为内存比物理磁盘速度快很多  

  ```
  i ｢wds｣: Project is running at http://localhost:8080/
  i ｢wds｣: webpack output is served from /
  i ｢wds｣: Content not from webpack is served from D:\vue-learn-heima\change-rows-color
  i ｢wdm｣: asset main.js 649 KiB [emitted] (name: main)
  ```

  >webpack-dev-server 生成到内存中的文件，**默认放到了项目的根目录中**（即main.js被放到了根目录），而且是**虚拟的、不可见的**。
  >
  >- 可以直接用 / 表示项目根目录，后面跟上要访问的文件名称，即可访问内存中的文件



在webpack.config.js配置文件中，可以通过**devServer节点**对此插件进行更多的配置：

```js
module.exports={
    mode:'development',
    devServer: {
        open: true,         // 打包完成后，是否自动打开浏览器
        host: '127.0.0.1',
        port: 80
    },
}
```

> 若修改了webpack.config.js或package.json，需重启生效



## html-webpack-plugin

html-webpack-plugin 是 webpack 中的 HTML 插件，可以通过此插件自定制 index.html 页面的内容。  



1. 安装html-webpack-plugin  

   ```sh
   npm install html-webpack-plugin@5.3.2 -D
   ```

2. 配置 html-webpack-plugin

   ```js
   const HtmlPlugin = require('html-webpack-plugin')   // 导入HTML插件，获得构造函数
   const htmlPlugin = new HtmlPlugin({         // 创建HTML插件的实例对象
       template: './src/index.html',   // 指定原文件的存放路径
       filename: './index.html'        // 指定生成文件的存放路径
   })
   
   module.exports={
       mode:'development',
       plugins: [htmlPlugin]   // 通过plugin节点，使htmlPlugin生效
   }
   ```



> - 复制生成的文件，将被放到内存中
> - 复制生成的文件，会自动引入webpack生成的js文件



# webpack 中的 loader

[黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=14&spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)

## loader概述

- 在实际开发过程中，webpack 默认只能打包处理以 .js 后缀名结尾的模块。

- **其他非 .js 后缀名结尾的模块，webpack 默认处理不了，需要调用 loader 加载器**才可以正常打包，否则会报错！  



loader 加载器的作用：**协助 webpack 打包处理特定的文件模块**。比如：

- css-loader 可以打包处理 .css 相关的文件
- less-loader 可以打包处理 .less 相关的文件
- babel-loader 可以打包处理 webpack 无法处理的高级 JS 语法



## loader的调用过程

![image-20220618225651565](%E5%89%8D%E7%AB%AF%E5%B7%A5%E7%A8%8B%E5%8C%96%E4%B8%8Ewebpack.assets/image-20220618225651565.png)





## 打包处理 css 文件

1. 安装处理css文件的loader：

   ```sh
   npm i style-loader@3.0.0 css-loader@5.2.6 -D
   ```

2. 在 webpack.config.js 的 module -> rules 数组中，添加 loader 规则如下：  

   ```js
   module: {   //所有非.js的第三方模块的匹配规则
       rules: [
           {
               test: /\.css$/, // 后缀.css文件的匹配规则
               use: ['style-loader','css-loader']
           }
       ]
   }
   ```

   > - use数组的顺序有意义
   > - 多个loader的调用顺序：从后向前



## 打包处理 less 文件

1. 安装处理less文件的loader：

   ```sh
   npm i less-loader@10.0.1 less@4.1.1 -D
   ```

2. 在 webpack.config.js 的 module -> rules 数组中，添加 loader 规则如下： 

   ```js
   module: {
       rules: [
           {
               test: /\.less/,
               use: ['style-loader','css-loader','less-loader']
           }
       ]
   }
   ```

   

## url-loader

1. 安装处理url路径相关的loader：

   ```sh
   npm i url-loader@4.1.1 file-loader@6.2.0 -D
   ```

2. 在 webpack.config.js 的 module -> rules 数组中，添加 loader 规则如下： 

   ```js
   module: {
       rules: [
           {
               test: /\.jpg|png|gif$/,
               use: 'url-loader?limit=22229'
           }
       ]
   }
   ```

   > 其中 ? 之后的是loader的参数项：
   >
   > - limit 用来指定图片的大小，单位是字节（byte）
   >
   >   只有 ≤ limit 大小的图片，才会被转为 base64 格式的图片；否则会是请求路径



## loader的另一种配置方式

对于带参数项的 loader， 还可以通过对象的方式进行配置。例如url-loader:

```js
module: {
    rules: [
        {
            test: /\.jpg|png|gif$/,
            use: {
                loader: 'url-loader',
                options:{
                    limit: 22229,
                }
            }
        }
    ]
}
```



## 打包处理 js 文件中的高级语法  

- webpack 只能打包处理一部分高级的 JavaScript 语法。

- 对于那些 webpack 无法处理的高级 js 语法，需要借助于 babel-loader 进行打包处理。例如 webpack 无法处理下面的 JavaScript 代码：  

  ```js
  function info(target){
      target.info = 'Person info'
  }
  
  @info()
  class Person{}
  
  console.log(Person.info)
  ```

  

1. 安装babel-loader：

   ```sh
   npm i babel-loader@8.2.2 @babel/core@7.14.6 @babel/plugin-proposal-decorators@7.14.5 -D
   ```

2. 在 webpack.config.js 的 module -> rules 数组中，添加 loader 规则如下： 

   ```js
   module: {
       rules: [
           {
               test: /\.js/,
               exclude: /node_modules/,	// 配置排除项
               use: 'babel-loader'
           }
       ]
   }
   ```

3. 配置babel-loader：

   在项目根目录下，创建名为babel.config.js的配置文件：

   ```js
   module.exports ={
       plugins:[[
           '@babel/plugin-proposal-decorators',
           {legacy:true}
       ]]
   }
   ```



>https://babeljs.io/docs/en/babel-plugin-proposal-decorators



# 打包发布

[黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=23&vd_source=be746efb77e979ca275e4f65f2d8cda3)



# Source Map