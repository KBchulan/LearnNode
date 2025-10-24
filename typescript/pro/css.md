# CSS 书写规则完全指南

*Last updated: 2025-02-23 03:00:53 UTC by KBchulan*

## 1. 基本语法规则

### 1.1 选择器和声明块

```css
/* 基本结构 */
选择器 {
    属性: 值;
    属性2: 值2;
}

/* 示例 */
.button {
    color: #333;
    background-color: #fff;
    padding: 10px 20px;
}
```

### 1.2 多个选择器

```css
/* 多个选择器共用一个声明块 */
h1,
.title,
#main-title {
    font-size: 20px;
    color: #333;
}
```

## 2. 属性书写顺序

### 2.1 推荐的属性顺序

```css
.element {
    /* 1. 定位属性 */
    position: absolute;
    top: 0;
    right: 0;
    bottom: 0;
    left: 0;
    z-index: 10;
  
    /* 2. 盒模型 */
    display: flex;
    float: right;
    width: 100px;
    height: 100px;
    margin: 10px;
    padding: 10px;
  
    /* 3. 排版属性 */
    font-family: Arial;
    font-size: 16px;
    line-height: 1.5;
    text-align: center;
  
    /* 4. 视觉属性 */
    background-color: #fff;
    border: 1px solid #ccc;
    border-radius: 4px;
  
    /* 5. 其他 */
    opacity: 1;
    cursor: pointer;
    transition: all 0.3s;
}
```

## 3. 命名规范

### 3.1 类名命名规则

```css
/* 使用连字符分隔 */
.user-profile { }
.navigation-menu { }

/* 功能性命名 */
.btn-primary { }
.text-center { }
.hidden { }

/* 状态类 */
.is-active { }
.has-error { }
```

### 3.2 ID命名规则

```css
#main-content { }
#sidebar { }
#user-profile { }
```

## 4. 嵌套规则

### 4.1 选择器嵌套（使用预处理器）

```scss
.nav {
    &__item {
        padding: 10px;
      
        &:hover {
            background: #f5f5f5;
        }
    }
  
    &__link {
        color: blue;
      
        &--active {
            color: red;
        }
    }
}
```

## 5. 数值与单位

### 5.1 常用写法

```css
.element {
    /* 使用简写 */
    margin: 0;                /* 无单位的0 */
    padding: 10px 20px;      /* 两个值 */
    border: 1px solid #ccc;   /* 多个值 */
  
    /* 颜色值 */
    color: #333;             /* 十六进制 */
    background: rgba(0,0,0,.5); /* RGBA */
  
    /* 小数 */
    opacity: 0.8;            /* 省略0 */
    line-height: 1.5;        /* 无单位 */
}
```

## 6. 注释规范

```css
/* ================================
   主要区块注释
   ================================ */

/* 次要区块注释
   ----------------------- */

/* 单行注释 */

/**
 * 文档型注释
 * @param {string} color - 主题颜色
 */
```

## 7. 媒体查询

```css
/* 推荐的媒体查询写法 */
@media screen and (min-width: 768px) {
    .element {
        width: 750px;
    }
}

@media screen and (min-width: 1024px) {
    .element {
        width: 960px;
    }
}
```

## 8. 最佳实践

### 8.1 代码组织

```css
/* 1. 重置样式 */
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

/* 2. 全局样式 */
body {
    font-family: Arial, sans-serif;
    line-height: 1.6;
}

/* 3. 布局样式 */
.container {
    max-width: 1200px;
    margin: 0 auto;
}

/* 4. 组件样式 */
.button {
    /* 按钮样式 */
}

/* 5. 工具类 */
.text-center {
    text-align: center;
}
```

### 8.2 避免的做法

```css
/* 避免过度嵌套 */
.header .nav .nav-item .nav-link { } /* 不推荐 */
.nav-link { } /* 推荐 */

/* 避免使用!important */
.element {
    color: red !important; /* 尽量避免 */
}

/* 避免使用内联样式 */
<div style="color: red;"> /* 不推荐 */
```

## 9. 性能考虑

1. **选择器性能**

   - 避免过长的选择器链
   - 避免使用通配符选择器
   - 减少使用后代选择器
2. **文件组织**

   - 模块化CSS
   - 使用CSS预处理器
   - 合理使用注释
3. **优化技巧**

   - 使用简写属性
   - 合并相似的规则
   - 删除无用的代码
