# HTML5的新特性

- 针对于以往的不足，添加了一些新的**标签、表单、表单属性**等。



# 新增的语义化标签

## 替代div标签

- `<header>`
- `nav`：导航栏
- `article`
- `section`：**定义文档某个区域**
- `aside`：侧边栏
- `footer`



![image-20220616231509990](HTML5%E6%A0%87%E7%AD%BE.assets/image-20220616231509990.png)



注意：

- 这些语义化标签，是针对于搜索引擎的

- 在IE9中，这些元素默认不是块级元素，需要`display: block;`



## 多媒体标签

- `<video>`：视频

  ![image-20220616234408848](HTML5%E6%A0%87%E7%AD%BE.assets/image-20220616234408848.png)

  ```html
  <video src="" controls="controls"></video>
  ```

  > HTML5可以在不使用插件的情况下，**原生播放视频**。但存在如下的格式要求：
  >
  > ![image-20220616233801551](HTML5%E6%A0%87%E7%AD%BE.assets/image-20220616233801551.png)

- `<audio>`：音频

  [黑马程序员pink老师前端入门教程，零基础必看的h5(html5)+css3+移动端前端视频教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV14J4114768?p=277&spm_id_from=pageDriver&vd_source=be746efb77e979ca275e4f65f2d8cda3)