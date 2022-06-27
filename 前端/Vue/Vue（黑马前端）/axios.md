# 导入axios并使用

- 安装axios：

  ```sh
  npm i axios -S
  ```

- 发起GET请求：

  ```vue
  <template>
    <div id="app">
      <button @click="getInfo">发起Get请求</button>
    </div>
  </template>
  
  <script>
  import axios from 'axios'
  
  export default {
    name: 'App',
    methods: {
      async getInfo() {
        const obj = await axios.get('...')
        console.log(obj)
      }
    }
  }
  </script>
  ```

- 发起POST请求：

  ```vue
  <template>
    <div id="app">
      <button @click="getInfo">发起Post请求</button>
    </div>
  </template>
  
  <script>
  import axios from 'axios'
  
  export default {
    name: 'App',
    methods: {
      async getInfo() {
        const obj = await axios.post('...',{...})
        console.log(obj)
      }
    }
  }
  </script>
  ```

  

# 将axios挂载到Vue原型并配置请求根路径



main.js：

```js
import axios from "axios";
axios.defaults.baseURL = 'http://www.abc.com:3306'
Vue.prototype.$http = axios
```



使用者：

```vue
<template>
  <div id="app">
    <button @click="getInfo">发起Post请求</button>
  </div>
</template>

<script>

export default {
  name: 'App',
  methods: {
    async getInfo() {
      const obj = await this.$http.get('/api/get')
      console.log(obj)
    }
  }
}
</script>
```



> 缺点：
>
> 无法实现API接口的复用