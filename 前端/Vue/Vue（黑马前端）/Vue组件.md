# 单页面应用程序SPA

## 什么是单页面应用程序

- 单页面应用程序（英文名：Single Page Application）简称 SPA，顾名思义，指的是一个 **Web 网站中只有唯一的一个 HTML 页面**，所有的功能与交互都在这唯一的一个页面内完成  



## 单页面应用程序的特点

- 单页面应用程序将所有的功能局限于一个 web 页面中，**仅在该 web 页面初始化时加载相应的资源**（ HTML、JavaScript 和 CSS）  
- 一旦页面加载完成了，SPA **不会**因为用户的操作而**进行页面的重新加载或跳转**。而是利用 JavaScript 动态地变换HTML 的内容，从而实现页面与用户的交互  



## 单页面应用程序的优点

- 良好的交互体验
  - 单页应用的内容的改变不需要重新加载整个页面
  - 获取数据也是通过 Ajax 异步获取
  - 没有页面之间的跳转，不会出现“白屏现象  
- 良好的前后端工作分离模式
  - 后端专注于提供 API 接口，更易实现 API 接口的复用
  - 前端专注于页面的渲染，更利于前端工程化的发展
- 减轻服务器的压力
  - 服务器只提供数据，不负责页面的合成与逻辑的处理，吞吐能力会提高几倍



## 单页面应用程序的缺点

- 首屏加载慢
  - 路由懒加载
  - 代码压缩
  - CDN 加速
  - 网络传输压缩
- 不利于 SEO
  - SSR 服务器端渲染



## 如何快速创建vue的SPA项目

vue 官方提供了两种快速创建工程化的 SPA 项目的方式：

- 基于 vite 创建 SPA 项目
- 基于 vue-cli 创建 SPA 项目



# Vue-cli

## 安装Vue-cli

Vue-cli是npm上的一个全局包，使用npm instal命令：

```sh
npm install -g @vue/cli
```



## 创建Vue项目

```sh
vue create 项目名
```



> 可以选择手动配置(Manual)，例如：
>
> ![image-20220620162537579](Vue%E7%BB%84%E4%BB%B6.assets/image-20220620162537579.png)
>
> ![image-20220620162847927](Vue%E7%BB%84%E4%BB%B6.assets/image-20220620162847927.png)
>
> > Lint用于约束代码风格



## 项目结构

![image-20220620163944679](Vue%E7%BB%84%E4%BB%B6.assets/image-20220620163944679.png)

- node_modules：第三方包
- public
- src：项目源码目录
  - assets：用于存放项目中使用的静态资源文件，如CSS, img
  - components：用于存放组件
  - main.js：项目的入口文件
  - App.vue：项目的**根组件**



## vue 项目的运行流程

在工程化的项目中，vue 要做的事情很单纯：通过 main.js 把 App.vue 渲染到 index.html 的指定区域中。其中：

- App.vue 用来编写待渲染的模板结构
- index.html 中需要预留一个 el 区域
- main.js 把 App.vue 渲染到了 index.html 所预留的区域中





# 组件的基本概念

- 组件化开发指的是：根据封装的思想，把页面上可重用的 UI 结构封装为组件，从而方便项目的开发和维护。  
- vue 中规定：组件的后缀名是 .vue。之前接触到的 App.vue 文件本质上就是一个 vue 的组件。  



## Vue组件的三个组成部分

每个 .vue 组件都由 3 部分构成，分别是：  

- template -> 组件的模板结构

  ```vue
  <template>
  	<!-- 当前组件的Dom结构-->
  </template>
  ```

  >- template是vue提供的容器标签，不会被渲染为DOM元素
  >
  >-   template中只能包含唯一的**根节点**

- script -> 组件的 JavaScript 行为

  ```vue
  <script>
  	<!-- 当前组件的行为 -->
      export default{}	// 当前组件配置项(data,methods,...)
  </script>
  ```

- style -> 组件的样式

  为style标签，添加lang="less"属性，即可使用less语法

>**每个组件中必须包含 template 模板结构**，而 script 行为和 style 样式是可选的组成部分  



## 组件的父子关系

- 组件在被定义好之后，彼此之间是项目独立的，不存在父子关系：

  ![image-20220620170531049](Vue%E7%BB%84%E4%BB%B6.assets/image-20220620170531049.png)

- 但在使用组件的时候，根据彼此的嵌套关系，形成了父子关系、兄弟关系：

  ![image-20220620170540613](Vue%E7%BB%84%E4%BB%B6.assets/image-20220620170540613.png)



## 使用组件的三步骤

1. 使用`import`语法，导入需要的组件
2. 使用`components`节点注册组件
3. 以`标签形式`使用组件

![image-20220620170649733](Vue%E7%BB%84%E4%BB%B6.assets/image-20220620170649733.png)



## 全局组件

- 通过components，注册是**私有子组件**

- 在 vue 项目的 **main.js** 入口文件中，通过 Vue.component() 方法，可以注册**全局组件**：

  ```js
  import Count from '@/components/Count.vue'
  
  Vue.component('MyCount',Count)
  ```



# 组件的props选项

[黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=95&vd_source=be746efb77e979ca275e4f65f2d8cda3)
