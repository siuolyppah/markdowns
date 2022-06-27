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

props是组件的`自定义属性`，在`封装通用组件`时，合理地使用 props 可以极大的`提高组件的复用性`。



## 语法

```vue
<script>
export default {
  props: ['自定义属性A', '自定义属性B', '自定义属性C'],  // 组件的自定义属性
  data() {
    return {};  // 组件的私有数据
  }
}
</script>
```



> 注意：
>
> - 传入的**字符串**：
>
>   ```vue
>   <Count init="1"></Count>
>   ```
>
> - 结合v-bind指令：引号内为js表达式，因此传入的是**数值类型**：
>
>   ```vue
>   <Count :init="1"></Count>
>   ```



## props是只读的

vue 规定：组件中封装的自定义属性是只读的，程序员**不能直接修改 props 的值，否则会直接报错**。



解决方式：

- 将props的值，传入data属性中。

  > 先解析props选项，后解析data选项

  ```vue
  <script>
  export default {
    name: "Count",
    props: [
      'init'
    ],
    data() {
      return {
        count: this.init,
      }
    },
    methods: {
      addCount() {
        this.count++;
      }
    }
  }
  </script>
  
  使用者：
  <Count init="1"></Count>
  ```

  

## props的default选项

可以通过default选项，来定义属性的默认值：
```vue
<script>
    export default {
        name: "Count",
        props: {
            'init': {
                default: 0,
            }
        }
    }
</script>
```



> 若未设置默认值，且调用者未传入值，将为undefined



## props的type选项

- 可以通过 type 来定义属性的值类型。

  如果传递过来的值不符合规则，将在终端报错

- 示例代码如下：  

  ```vue
  <script>
  export default {
      props:{
          init:{
              default:0,
              type:Number
          }
      }
  }
  </script>
  ```

  > 可选类型：
  >
  > - Number, Boolean, String, Array, Object,...



## props的required选项

- 在声明自定义属性时，可以通过 required 选项，将属性设置为必填项，强制用户必须传递属性的值。

- 示例代码如下：

  ```vue
  <script>
  export default {
      props:{
          init:{
  			type: Number,
              required: true
          }
      }
  }
  </script>
  ```

  

## 组件之间的样式冲突问题

- 默认情况下，**写在.vue组件中的样式会全局生效**。

- 导致组件之间样式冲突的根本原因：

  - 单页面应用程序。所有组件的DOM结构，都是基于唯一的index.html页面呈现的
  - 每个组件的样式，都会影响整个index.html中DOM元素

- 解决方案：

  为**每个组件分配唯一的自定义属性**，在编写组件样式时，**通过属性选择器来控制样式的作用域**，示例代码如下：

  ```vue
  <template>
    <div class="container" data-v-001>
      <h3 data-v-001>轮播图组件</h3>
    </div>
  
  </template>
  
  
  <style>
  .container[data-v-001]{
    border: 1px solid red;
  }
  </style>
  ```

-  为了提高开发效率和开发体验，**vue 为 style 节点提供了 scoped 属性**，从而防止组件之间的样式冲突问题：  

  ```vue
  <template>
    <div class="container">
      <h3>轮播图组件</h3>
    </div>
  
  </template>
  
  
  <style scoped> 
  /*
    style节点的scoped属性，将为每仲组件分配唯一的“自定义属性”
  */
  .container {
    border: 1px solid red;
  }
  </style>
  ```

  

## /deep/ 样式穿透

- 如果给当前组件的 style 节点添加了 scoped 属性，则**当前组件的样式**对其**子组件**是不生效的。

- 如果想让某些样式**对子组件生效**，可以使用` /deep/ `深度选择器：

  ```vue
  <style scoped>
      /* 生成的选择器格式为：.title[data-v-052242de] */
      .title{
          color:blue;
      }
      
      /* 生成的选择器格式为： [data-v-052242de] .title*/
      /deep/ .title{
          color:blue;
      }
  </style>
  ```

> 使用场景：
>
> - 修改第三方组件库（如Vant、Elemnt-ui）的样式





# 组件的生命周期

## 组件实例

>Vue项目运行的本质：
>
>1. 通过vue-template-compiler将所有的.vue文件，编译为JS代码
>2. 由此JS代码操作DOM结构



- 组件实例：即Vue构造函数创建的对象。
  - Vue构造函数：对应着.vue文件
  - Vue实例对象：\<Xxx>标签的一次引用，将创建一个对应的Vue实例



## 生命周期 & 生命周期函数

[黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=104&spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)

- 生命周期（Life Cycle）是指一个组件从创建 -> 运行 -> 销毁的整个阶段，强调的是一个时间段。
- 生命周期函数：是由 vue 框架提供的内置函数，会伴随着组件的生命周期，自动按次序执行。

>注意：生命周期强调的是**时间段**，生命周期函数强调的是**时间点**。  



## 生命周期函数的分类

![image-20220625185617425](Vue%E7%BB%84%E4%BB%B6.assets/image-20220625185617425.png)





## 生命周期图示

[Vue 实例 — Vue.js (vuejs.org)](https://cn.vuejs.org/v2/guide/instance.html)

![lifecycle](Vue%E7%BB%84%E4%BB%B6.assets/lifecycle.png)



# 组件的数据共享

[黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=110&vd_source=be746efb77e979ca275e4f65f2d8cda3)



## 组件之间的关系

在项目开发中，组件之间的最常见的关系分为如下两种：

- 父子关系
- 兄弟关系

![image-20220625193636951](Vue%E7%BB%84%E4%BB%B6.assets/image-20220625193636951.png)



## 父子组件之间的数据共享

父子组件之间的数据共享又分为：

- 父 -> 子共享数据
- 子 -> 父共享数据  



### 父组件 -数据> 子组件

需要**在子组件中**，使用**自定义属性**。

- 父组件：

  ```vue
  <template>
    <div>
      <Son :msg="message"></Son>
    </div>
  </template>
  
  <script>
  import Son from "@/components/Son";
  
  export default {
    name: 'Father',
    data() {
      return {
        message:"hello vue.js"  
      }
    },
    components: {
      Son
    }
  }
  </script>
  ```

- 子组件：

  ```vue
  <template>
    <div>
      <h5>Son 组件</h5>
      <p>父组件传递的msg：{{ msg }}</p>
    </div>
  </template>
  
  <script>
  export default {
    name: "Son",
    props: ['msg']
  }
  </script>
  ```

  

### 子组件 -数据> 父组件

需要**在父组件**中，使用**自定义事件**

![image-20220625195648092](Vue%E7%BB%84%E4%BB%B6.assets/image-20220625195648092.png)



- 父组件：

  ```vue
  <template>
    <!--父组件-->
    <div id="app">
      <h3>父组件当前Count值为：{{ countFromSon }}</h3>
      <Son @numChange="getNewCount"></Son>
    </div>
  </template>
  
  <script>
  import Son from "@/components/Son";
  
  export default {
    name: 'App',
    data() {
      return {
        countFromSon: 0
      }
    },
    methods: {
      getNewCount(val) {
        this.countFromSon = val
      }
    },
    components: {
      Son
    }
  }
  </script>
  ```

- 子组件：

  ```vue
  <template>
    <div>
      <h5>Son 组件，当前Count值为：{{ count }}</h5>
      <button @click="add">+1</button>
    </div>
  </template>
  
  <script>
  export default {
    name: "Son",
    data() {
      return {
        count: 0
      }
    },
    methods: {
      add() {
        this.count += 1
        // 通过 $emit() 触发自定义事件
        this.$emit('numChange', this.count)
      }
    }
  }
  </script>
  ```



## 兄弟组件之间的数据共享

在 vue2.x 中，兄弟组件之间数据共享的方案是 EventBus。

![image-20220625200108437](Vue%E7%BB%84%E4%BB%B6.assets/image-20220625200108437.png)

EventBus的使用步骤如下：

1. 创建 eventBus.js 模块，并向外**共享一个 Vue 的实例对象**
2. 在数据**发送方**，调用` bus.$emit`('事件名称', 要发送的数据) 方法**触发自定义事件**
3. 在数据**接收方**，调用 `bus.$on`('事件名称', 事件处理函数) 方法**注册一个自定义事件**



# ref引用

## 什么是ref引用

- ref 用来辅助开发者在不依赖于 jQuery 的情况下，获取 DOM 元素或组件的引用
- 每个 vue 的组件实例上，都包含一个 `$refs` 对象，里面存储着对应的 DOM 元素或组件的引用。
- 默认情况下，组件的 `$refs` 指向一个空对象。  



## 使用ref引用Dom元素

```vue
<template>
  <div>
    <h3 ref="myh3">被引用的DOM元素</h3>
    <button @click="changeColor">change color</button>
  </div>
</template>

<script>
export default {
  name: 'App',
  methods: {
    changeColor() {
      this.$refs.myh3.style.color = 'red'
    }
  }
}
</script>
```



## 使用ref引用Vue组件实例

> 在获取到Vue组件实例后，就可以调用该实例上的methods方法



- Test.vue：

  ```vue
  <template>
    <div>
      <h3>Count值为：{{ count }}</h3>
    </div>
  </template>
  
  <script>
  export default {
    name: "test",
    data() {
      return {
        count: 0
      }
    },
    methods: {
      addCount() {
        this.count++
      }
    }
  }
  </script>
  
  ```

- App.vue：

  ```vue
  <template>
    <div id="app">
      <Test ref="testRef"></Test>
      <button @click="triggerAddCount">click</button>
    </div>
  </template>
  
  <script>
  import Test from "@/components/Test";
  
  export default {
    name: 'App',
    components: {
      Test
    },
    methods: {
      triggerAddCount() {
        this.$refs.testRef.addCount()
      }
    }
  }
  </script>
  ```

  

## this.$nextTick(cb) 方法

组件的 $nextTick(cb) 方法，会把 cb 回调推迟到下一个 DOM 更新周期之后执行

> cb即CallBack，回调函数

> 可以通俗的理解为：
>
> - 等组件的DOM 更新完成之后，再执行 cb 回调函数
> - 从而能保证 cb 回调函数可以操作到最新的 DOM 元素。  



案例：

```vue
<template>
  <div id="app">
    <input type="text" v-if="inputVisible" ref="ipt">
    <button @click="showInput">展示Input框</button>
  </div>
</template>

<script>
export default {
  name: 'App',
  data() {
    return {
      inputVisible: false
    }
  },
  methods: {
    showInput() {
      this.inputVisible = true

      // 把对Input框的操作，推迟到下次DOM更新之后。否则此时页面中根本不存在Input元素
      this.$nextTick(() => {
        this.$refs.ipt.focus()
      })
    }
  }
}
</script>
```



> updated生命周期钩子也可实现此业务逻辑



# 动态组件

[黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=146&vd_source=be746efb77e979ca275e4f65f2d8cda3)

动态组件，指的是**动态切换组件的显示与隐藏**。  



## 语法

使用Vue提供的`<component>`，并为其添加`is`属性。

```vue
<template>
  <div>
      <component is="Count"></component>
  </div>
</template>

<script>
import Count from "@/components/Count";

export default {
  name: 'App',
  components:{
    Count
  }
}
</script>
```



## 使用keep-alive保持状态

- 在默认情况下，当切换动态组件时，旧组件将被销毁，新组件将被创建。

- 此时可以使用 vue 内置的 `<keep-alive> `组件保持动态组
  件的状态。

  

示例代码如下：  

```vue
<template>
  <div id="app">
    <button @click="change">click to change</button>
    <keep-alive>
      <component :is="compName"></component>
    </keep-alive>
  </div>
</template>

<script>
import Count from "@/components/Count";

export default {
  name: 'App',
  components: {
    Count
  },
  data() {
    return {
      compName: 'Count'
    };
  },
  methods: {
    change() {
      if (this.compName === 'Count') {
        this.compName = ''
      } else {
        this.compName = 'Count'
      }
    }
  }
}
</script>
```



## keep-alive 对应的生命周期函数

- 当组件被**缓存**时，会自动触发组件的 deactivated 生命周期函数。
- 当组件被**激活**时，会自动触发组件的 activated 生命周期函数。  

> 注意：
>
> 当组件第一次被创建时，将先后触发created和actived生命周期钩子



## keep-alive 的 include 属性 

- include 属性用来指定：只有**名称匹配**的组件会被缓存
- 多个组件名之间使用英文的逗号分隔

```vue
<keep-alive include="MyLeft,MyRight"> <!-- 注意是组件的注册名称 -->
	<component :is="compName"></component>
</keep-alive>
```



> - 对应的，`exclude`属性用于指定不进行缓存的组件
> - include和exclude属性，只能同时指定一个



# 插槽 Slot

- 插槽（Slot）是 vue 为组件的封装者提供的能力。

- 允许开发者在封装组件时，把不确定的、希望由用户指定的部分定义为插槽。  

  >可以把插槽认为是组件封装期间，为用户预留的内容的占位符。  

![image-20220626201647788](Vue%E7%BB%84%E4%BB%B6.assets/image-20220626201647788.png)



## 插槽的基本用法

- 封装了插槽的组件（提供插槽者）：

  ```vue
  <template>
    <div>
      <p>以下是插槽内容：</p>
      <slot></slot>
    </div>
  </template>
  
  <script>
  export default {
    name: "test",
  }
  </script>
  ```

- 使用者：

  ```vue
  <template>
    <div id="app">
      <Test>
        <h1>此处内容将被放入插槽</h1>
      </Test>
    </div>
  </template>
  
  <script>
  import Test from "@/components/Test";
  
  export default {
    name: 'App',
    components:{
      Test
    }
  }
  </script>
  ```

  

## 没有预留插槽的内容会被丢弃

如果在封装组件时<span style="color:#c20200;">没有预留任何 \<slot> 插槽</span>，则用户提供的任何<span style="color:#c20200;">自定义内容都会被丢弃</span>。示例代码如下：  

- 封装者：

  ```vue
  <template>
  	<p>此组件没有插槽</p>
  </template>
  ```

- 使用者：

  ```vue
  <template>
  	<Test>
      	<p>用户自定义内容</p> <!-- 将被丢弃 -->
      </Test>
  </template>
  ```

  

## 后备内容

- 封装组件时，可以为预留的 `<slot>` 插槽提供后备内容（默认内容）。
- 如果组件的使用者没有为插槽提供任何内容，则后备内容会生效。



示例代码如下：  

```vue
<template>
	<p>以下插槽内容：</p>
	<slot>此为后备内容</slot>
</template>
```



## 具名插槽

- 如果在封装组件时**需要预留多个插槽节点**，则**需要为每个 `<slot>` 插槽指定具体的 name 名称**。
- 这种带有具体名称的插槽叫做“具名插槽”。



示例代码如下  ：

- 提供者：

  ```vue
  <template>
    <div class="container">
      <header>
        <slot name="header"></slot>
      </header>
  
      <main>
        <slot></slot>
      </main>
  
      <footer>
        <slot name="footer"></slot>
      </footer>
    </div>
  </template>
  
  <script>
  export default {
    name: "test",
  }
  </script>
  ```

- 使用者：

  在`<template>`元素上，使用`v-slot`指令，其后跟着插槽的name。

  ```vue
  <template>
    <div id="app">
      <Test>
        <template v-slot:header>
          <h1>标题内容</h1>
        </template>
  
        <template v-slot:default>
          中心内容
        </template>
  
        <template v-slot:footer>
          底部内容
        </template>
      </Test>
    </div>
  </template>
  
  <script>
  import Test from "@/components/Test";
  
  export default {
    name: 'App',
    components: {
      Test
    }
  }
  </script>
  ```

  >没有指定name名称的插槽，会有隐含的name值：“default”

  > 具名插槽的简写形式：
  >
  > - 将参数之前的所有内容（即v-slot:）简写为#。
  >
  >   即 v-slot:header，可以简记为#header
  >
  >   ```vue
  >   <template #header>...</template>
  >   ```



## 作用域插槽

- 在封装组件的过程中，可以为预留的 `<slot>` 插槽绑定 props 数据。

  这种带有 props 数据的 \<slot> 叫做“作用域插槽”  



- 提供插槽的组件：

  ```vue
  <template>
    <div>
      <h3>以下为插槽</h3>
      <slot msg="hello,vue.js"></slot>
    </div>
  </template>
  
  <script>
  export default {
    name: "test",
  }
  </script>
  ```

- 使用插槽的组件：

  父组件将获得一个对象，在该对象中保存了子组件在插槽中定义的属性

  ```vue
  <template>
    <div id="app">
      <Test>
        <template v-slot:default="obj">
          父组件获得到：{{ obj.msg }}
        </template>
      </Test>
    </div>
  </template>
  
  <script>
  import Test from "@/components/Test";
  
  export default {
    name: 'App',
    components: {
      Test
    }
  }
  </script>
  ```

  

## 作用域插槽的解构赋值

- 提供者：

  ```vue
  <template>
    <div>
      <h3>以下为插槽</h3>
      <slot msg="hello,vue.js" author="me"></slot>
    </div>
  </template>
  
  <script>
  export default {
    name: "test",
  }
  </script>
  ```

- 使用者：

  ```vue
  <template>
    <div id="app">
      <Test>
        <template v-slot:default="{msg,author}"> <!-- 此处使用解构赋值-->
          {{ msg }},{{ author }}
        </template>
      </Test>
    </div>
  </template>
  
  <script>
  import Test from "@/components/Test";
  
  export default {
    name: 'App',
    components: {
      Test
    }
  }
  </script>
  ```



# 自定义指令

[黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=160&vd_source=be746efb77e979ca275e4f65f2d8cda3)



vue 中的自定义指令分为两类，分别是：

- 私有自定义指令
- 全局自定义指令



## 私有自定义指令

在每个 vue 组件中，可以在 directives 节点下声明私有自定义指令。示例代码如下：  

```vue
<template>
  <div id="app">
    <h1 v-red>APP根组件</h1>
  </div>
</template>

<script>
export default {
  name: 'App',
  directives: {
    red: {
      bind(el) {  // el形参，为绑定此指令的原生DOM对象
        el.style.color = 'red'
      }
    }
  }
}
</script>
```

 

> 在声明自定义指令时，可以通过形参的第二个参数（对象类型），来接收指令的参数值。



- bind函数：

  bind 函数只调用 1 次：当指令第一次绑定到元素时调用，当 DOM 更新时 bind 函数不会被触发  

- update函数：

  update 函数会在每次 DOM 更新时被调用



## 全局自定义指令

全局共享的自定义指令需要通过“`Vue.directive()`”进行声明，示例代码如下：  

main.js：

```js
Vue.directive('red',{
  bind(el) {  // el形参，为绑定此指令的原生DOM对象
    el.style.color = 'red'
  }
})
```



# 路由Router

## SPA与前端路由

- SPA：单页面应用，所有组件的展示与切换，都在唯一一个html中完成
- 在SPA项目中，想要实现根据请求地址，动态切换功能页面，需要使用前端路由



即**前端路由**：**Hash地址**（锚链接，形如#/home）与**组件**的对应关系。

> 锚链接，不会引起页面刷新，但会产生浏览历史



## 前端路由的工作方式

1. 用户点击页面上的路由链接
2. URL地址栏的Hash值发生变化
3. **前端路由监测到Hash值变化**
4. 前端路由将当前Hash地址对应的组件，进行渲染

![image-20220627211342153](Vue%E7%BB%84%E4%BB%B6.assets/image-20220627211342153.png)



`````vue
<template>
  <div id="app">
    <a href="#/home">Home</a>
    <a href="#/test">Test</a>

    <component :is="hash"></component>
  </div>
</template>

<script>
import Home from "@/components/Home";
import Test from "@/components/Test";

export default {
  name: 'App',
  components: {
    Home,
    Test
  },
  data() {
    return {
      hash: 'Home'
    }
  },
  created() {
    window.onhashchange = () => {
      switch (location.hash) {
        case '#/home':
          this.hash = 'Home'
          break
        case '#/test':
          this.hash = 'Test'
          break
      }
    }
  }
}
</script
`````



## vue-router

vue-router 是 vue.js 官方给出的路由解决方案。它只能结合 vue 项目进行使用，能够轻松的管理 SPA 项目中组件的切换。  

vue-router 的官方文档地址：https://router.vuejs.org/zh/  



### vue-router的基本使用

1. 安装vue-router包

   ```sh
   npm i vue-router@3.5.2 -S
   ```

2. 创建路由模块：

   在src目录下，新建`router/index.js`路由模块：

   ```js
   import Vue from "vue";
   import VueRouter from "vue-router";
   
   // 为Vue安装插件
   Vue.use(VueRouter)
   
   const router = new VueRouter()
   
   export default router	
   ```

3. 导入并挂载路由模块：

   在src/`main.js`入口文件中：

   ```js
   import router from "@/router";	// 若只给出路径，则默认加载路径下的index.js
   
   new Vue({
       render: h => h(App),
       router: router
   }).$mount('#app')
   ```

4. 在src/App.vue组件中，使用vue-router提供的`<router-link>`和`<router-view>`声明路由链接和占位符：

   ```vue
   <template>
     <div class="app-container">
       <h1>App 根组件</h1>
   
       <!-- 定义路由链接 -->
       <router-link to="/home">首页</router-link>
       <router-link to="/movie">电影</router-link>
       <router-link to="/about">关于</router-link>
       <hr />
   
       <!-- 声明路由的占位符 -->
       <router-view></router-view>
       
     </div>
   </template>
   
   <script>
   import Home from '@/components/Home.vue'
   import Movie from '@/components/Movie.vue'
   import About from '@/components/About.vue'
   
   export default {
     name: 'App',
     components: {
       Home,
       Movie,
       About
     }
   }
   </script>
   ```

2. 声明路由的匹配规则：

   在 src/router/index.js 路由模块中，通过 `routes 数组`声明路由的匹配规则：

   ```js
   import Vue from "vue";
   import VueRouter from "vue-router";
   import Home from "@/components/Home";
   import Movie from "@/components/Movie";
   import About from "@/components/About";
   
   Vue.use(VueRouter)
   
   const router = new VueRouter({
       routes: [
           {path: '/home', component: Home},
           {path: '/movie', component: Movie},
           {path: '/about', component: About},
       ]
   })
   
   export default router
   ```

   

> 完整案例：见附件“router-demo-quick.zip”



### vue-router的常见用法

- day7
- [黑马程序员Vue全套视频教程，从vue2.0到vue3.0一套全覆盖，前端必会的框架教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1zq4y1p7ga?p=181&vd_source=be746efb77e979ca275e4f65f2d8cda3)
