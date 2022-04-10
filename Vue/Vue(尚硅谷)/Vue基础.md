# 参考资料

- 视频链接：https://www.bilibili.com/video/BV1Zy4y1K7SH


- Vue官网：https://cn.vuejs.org/



# 插件

VS插件：

- Live Server：会将整个项目作为端口为5500的服务器的资源

  ![image-20220410214110917](Vue%E5%9F%BA%E7%A1%80.assets/image-20220410214110917.png)

  ![image-20220410214122329](Vue%E5%9F%BA%E7%A1%80.assets/image-20220410214122329.png)

- 





# Javascript基础

- ES6语法规范
- ES6模块化
- 包管理器：如npm
- 原型、原型链模式
- 数组的常用方法
- axios
- promise



# 搭建Vue开发环境

## 安装Vue

https://cn.vuejs.org/v2/guide/installation.html

- 直接使用\<script>引入
  -   [开发版本](https://cn.vuejs.org/js/vue.js)包含完整的警告和调试模式
  -   [生产版本](https://cn.vuejs.org/js/vue.min.js)删除了警告
- NPM结合Vue-cli工具

> 当引入Vue后，***多了一个Vue的构造函数***







若想关闭此提示：设置[productionTip](https://cn.vuejs.org/v2/api/#productionTip)为false即可：

![image-20220410212801510](Vue基础.assets/image-20220410212801510.png)

```html
<script>
    Vue.config.productionTip = false;	//阻止 vue 在启动时生成生产提示
</script>
```



## 安装Vue Devtools

即Vue开发者工具

https://github.com/vuejs/devtools#vue-devtools





# helloworld项目

1. 准备Vue容器

2. 根据传入的配置对象，创建Vue实例

   > 通常配置对象的名称固定，数据类型固定

   > 配置对象的el属性，用于绑定dom节点

3. 使用Vue插值语法，从Vue实例中获取数据

   > Vue容器中的Vue代码，称为***Vue模板***



```html
<!DOCTYPE html>
<html>

<head>
    <meta charset=UTF-8>
    <title>Hello World!</title>
    <script src="../js/vue.js"></script>
</head>

<body>
    <!-- 为Vue准备一个容器 -->
    <div id="root">

        <!-- Vue的插值语法 -->
        <h1>hello,{{name}}</h1>
    </div>

    <script type="text/javascript">
        Vue.config.productionTip = false;

        //根据配置对象，创建Vue对象实例
        new Vue({

            // el:document.getElementById("#root");
            el: "#root", //通常为CSS选择器的字符串

            data: {  //自定义数据对象
                name: 'zhangsan'
            },
        });

    </script>
</body>

</html>
```



## 细节问题

- Vue实例和Dom节点是一对一的关系（即第一个Dom节点）

  真实开发中只有一个Vue实例，同时配合多个Vue组件使用

- Vue插值`{{ }}`中，可以写JS表达式

  ***此处可以自动读取到data对象中定义的属性***，例如`{{name.toUpperCase()}}`





# 模板语法

## 插值语法`{{ }}`

插值语法，***用于标签体内容***



`{{ }}`中应为JS表达式。特别的，可以引用Vue实例的data对象中定义的属性



## 指令语法

指令语法，***用于解析标签***，包括：

- 标签属性
- 标签体内容
- 绑定事件
- ....







### 例如v-bind指令

> 只要是标签的属性，就支持v-bind指令
>
> ```html
> <div v-bind:hh="hh"></div>
> ```



> 特别的，`v-bind:`指令可以简写为`:`



在使用v-bind指令后，`v-bind:href="url"`中的url，将***被当作JS表达式被执行***

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Document</title>
    
    <script src="../js/vue.js"></script>
</head>
<body>
    <div id="root">
        <a v-bind:href="url">点击跳转至百度</a>
    </div>

    <script>
        new Vue({
            el:"#root",
            data:{
                url:"https://www.baidu.com/",
            },
        });

    </script>
</body>
</html>
```



# 数据绑定

## 单向绑定（v-bind指令）

即***仅支持Vue实例中的数据流向HTML页面***



```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Document</title>
    
    <script src="../js/vue.js"></script>
</head>
<body>
    <div id="root">
        单向数据绑定：<input type="text" v-bind:value="name">
    </div>


    <script>
        new Vue({
            el:"#root",
            data:{
                name:"尚硅谷",
            },
        });
    </script>
</body>
</html>
```



## 双向绑定（v-model指令）

注意，`v-model`指令仅可使用在表单类（输入类）元素[^1]上：

[^1]: 即具有value属性的元素

- input
- checkbox
- radio
- select
- ...



> `v-model:value`，可以简写为`v-model`，因为v-model默认收集的就是value值





```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Document</title>
    
    <script src="../js/vue.js"></script>
</head>
<body>
    <div id="root">
        双向数据绑定：<input type="text" v-model:value="name">
    </div>


    <script>
        new Vue({
            el:"#root",
            data:{
                name:"尚硅谷",
            },
        });
    </script>
</body>
</html>
```



# el属性和data属性的两种写法

el属性：

- 方式一：

  ```html
  <script>
      new Vue({
          el: "#root",
          data: {
              name:'尚硅谷',
          },
      });
  </script>
  ```

- 方式二：

  > Vue对象的，以$开头的属性名，是给用户自定义的

  ```html
  <script>
      const v = new Vue({
          data: {
              name:'尚硅谷',
          },
      });
  
      v.$mount("#root");
  </script>
  ```

  



data属性：

- 对象式：

  ```html
  <script>
      new Vue({
          el: "#root",
          data: {
              name:'尚硅谷',
          },
      });
  </script>
  ```

- 函数式：

  > - 此函数必须返回一个对象
  > - 此函数不能是箭头函数

  ```html
  <script>
      new Vue({
          el: "#root",
          data:function(){
  
              //此处的this为Vue实例对象
              return {
                  name:"尚硅谷",
              };
          },
      });
  </script>
  ```

  



# MVVM模型

https://www.bilibili.com/video/BV1Zy4y1K7SH?p=10&spm_id_from=pageDriver