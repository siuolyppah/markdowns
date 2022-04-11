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



`{{ }}`中应为JS表达式。特别的，可以引用Vue实例的属性及Vue实例原型的属性

>Vue的data对象中定义的属性，将变成Vue实例的属性



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

![image-20220411113957603](Vue%E5%9F%BA%E7%A1%80.assets/image-20220411113957603.png)

- M：模型（Model），一般JS对象，***对应Vue实例的data属性***

- V：视图（view），模板，***对应着HTML页面***

- VM：视图模型（ViewModel），***对应着Vue实例对象***

  > 因此往往用变量名vm，命名Vue实例



# 数据代理

## Object.defineproperty()方法的基本配置

通过使用`Object.defineproperty()`方法实现

```html
<script>
    let person = {
        name:'zhangsan',
        sex:'male',
    };


    Object.defineProperty(person,'age',{
        value:18,
        enumerable:true,
        writable:true,
        configurable:true,
    });
</script>
```



> 若配置项对象中无enumerable为true，则以这种方式添加的属性，***是不可被枚举的***
>
> ```html
> <script>
>     //以下两种方式不能获取到age属性
>   
>     console.log(Object.keys(person));
> 
>     for(let key in person){
>         console.log(person[key]);
>     }
> </script>
> ```

>若配置项对象中无writable为true，则以这种方式添加的属性，***是不可被修改的***

> 若配置项对象中无configurable为true，则以这种方式添加的属性，***是不可被删除的***
>
> ```html
> <script>
> 	delete person.age;
> </script>
> ```



##Object.defineproperty()方法的setter和getter

```html
<script>
    let age = 18;
    let person = {
        name:'zhangsan',
        sex:'male',
    };

    Object.defineProperty(person,'age',{
        //当有人读取person对象的age属性时，该方法被调用，返回值将成为age的值
        get:function(){
            return age;
        },

        //当有人修改Person对象的age属性时，该方法被调用，value即为待修改值
        set(value){
            console.log(value);
            age = value;	//注意，person对象的age属性来源于age变量
        },
    });
</script>
```



## 数据代理

数据代理：使用一个对象，代理另一个对象的读写操作

```html
<script>
    let obj1 = { x: 100 };
    let obj2 = { y: 100 };

    Object.defineProperty(obj2, 'x', {
        get() {
            return obj1.x;
        },

        set(value) {
            obj1.x = value;
        }
    });
</script>
```



## Vue中的数据代理

```html
<script>
    let data = {
        name : 'xiaowang'
    };

    let vm = new Vue({
        el:"#root",
        data
    });
</script>
```

> Vue实例的_data属性，即为传入的配置对象的data属性。即存在`vm._data === data`;



小总结：

1. 创建Vue实例时的配置对象，其data属性将被赋值给Vue实例的_data属性

2. 采用数据代理的方式，将Vue实例的_data属性，直接由Vue实例代理

   >***这一步的目的是简化编码***

   <img src="Vue%E5%9F%BA%E7%A1%80.assets/image-20220411124621639.png" alt="image-20220411124621639" style="zoom:50%;" />

   





# 事件处理

