# Markdown 完全指南 (Complete Markdown Guide)

*最后更新: 2025-02-19 by @KBchulan*

## 目录 (Table of Contents)

- [1. 文本格式化 (Text Formatting)](#1-文本格式化-text-formatting)
- [2. 标题 (Headers)](#2-标题-headers)
- [3. 列表 (Lists)](#3-列表-lists)
- [4. 链接和图片 (Links &amp; Images)](#4-链接和图片-links--images)
- [5. 代码 (Code)](#5-代码-code)
- [6. 表格 (Tables)](#6-表格-tables)
- [7. 引用 (Blockquotes)](#7-引用-blockquotes)
- [8. 水平线 (Horizontal Rules)](#8-水平线-horizontal-rules)
- [9. 高级功能 (Advanced Features)](#9-高级功能-advanced-features)
- [10. HTML 集成 (HTML Integration)](#10-html-集成-html-integration)

## 1. 文本格式化 (Text Formatting)

### 基本格式 (Basic Formatting)

```markdown
**粗体** 或 __粗体__
*斜体* 或 _斜体_
***粗斜体*** 或 ___粗斜体___
~~删除线~~
==高亮== (部分编辑器支持)
```

效果：
**粗体** 或 __粗体__
*斜体* 或 _斜体_
***粗斜体*** 或 ___粗斜体___
~~删除线~~

### 上标和下标 (部分编辑器支持)

```markdown
H~2~O
X^2^
```

### 特殊字符转义

```markdown
\* 星号
\\ 反斜线
\` 反引号
\_ 下划线
\{ \} 大括号
\[ \] 中括号
\( \) 小括号
\# 井号
\+ 加号
\- 减号
\. 点
\! 感叹号
```

## 2. 标题 (Headers)

### 使用 # 号

```markdown
# 一级标题
## 二级标题
### 三级标题
#### 四级标题
##### 五级标题
###### 六级标题
```

### 替代语法

```markdown
一级标题
=========

二级标题
---------
```

## 3. 列表 (Lists)

### 无序列表

```markdown
* 项目1
* 项目2
  * 子项目2.1
  * 子项目2.2
    * 子项目2.2.1

- 项目1
- 项目2
  - 子项目2.1
  - 子项目2.2

+ 项目1
+ 项目2
  + 子项目2.1
  + 子项目2.2
```

### 有序列表

```markdown
1. 第一项
2. 第二项
   1. 子项2.1
   2. 子项2.2
3. 第三项

1. 第一项
1. 第二项（数字会自动递增）
1. 第三项
```

### 任务列表

```markdown
- [x] 已完成任务
- [ ] 未完成任务
  - [x] 子任务1
  - [ ] 子任务2
```

## 4. 链接和图片 (Links & Images)

### 链接

```markdown
[行内链接](https://www.example.com)
[带标题的链接](https://www.example.com "链接标题")
[引用式链接][1]
[引用式链接][link-ref]

[1]: https://www.example.com
[link-ref]: https://www.example.com "链接标题"
```

### 自动链接

```markdown
<https://www.example.com>
<example@email.com>
```

### 图片

```markdown
![替代文本](image.jpg)
![替代文本](image.jpg "图片标题")
[![带链接的图片](image.jpg)](https://www.example.com)

[图片引用][img1]
[img1]: image.jpg "图片标题"
```

## 5. 代码 (Code)

### 行内代码

```markdown
使用 `code` 标记行内代码
```

### 代码块

````markdown
```
// 普通代码块
const hello = 'world';
```

```javascript
// 指定语言的代码块
console.log('Hello World!');
```

```python
# Python代码
def hello():
    print("Hello World!")
```
````
