# HTML 表格标签完全指南

*Last updated: 2025-02-21 09:30:57 UTC by KBchulan*

## 1. 基础表格标签

| 标签        | 描述           | 属性示例                                     |
| ----------- | -------------- | -------------------------------------------- |
| `<table>` | 定义表格       | `border`, `cellspacing`, `cellpadding` |
| `<tr>`    | 定义表格行     | `align`, `valign`                        |
| `<td>`    | 定义单元格     | `colspan`, `rowspan`                     |
| `<th>`    | 定义表头单元格 | `scope`, `colspan`                       |

## 2. 表格结构标签

| 标签          | 描述     | 用途               |
| ------------- | -------- | ------------------ |
| `<thead>`   | 表格头部 | 包含表格的表头内容 |
| `<tbody>`   | 表格主体 | 包含表格的主要内容 |
| `<tfoot>`   | 表格页脚 | 包含表格的汇总行   |
| `<caption>` | 表格标题 | 定义表格的标题     |

## 3. 常用属性

### 3.1 表格属性（`<table>`）

- `border`: 设置边框宽度
- `width`: 设置表格宽度
- `cellspacing`: 单元格之间的间距
- `cellpadding`: 单元格内容与边框的间距
- `align`: 表格对齐方式

### 3.2 单元格属性（`<td>`, `<th>`）

- `colspan`: 跨列合并
- `rowspan`: 跨行合并
- `align`: 水平对齐
- `valign`: 垂直对齐
- `width`: 单元格宽度

## 4. 代码示例

### 4.1 基础表格

```html
<table border="1">
    <tr>
        <th>表头1</th>
        <th>表头2</th>
    </tr>
    <tr>
        <td>数据1</td>
        <td>数据2</td>
    </tr>
</table>
```

### 4.2 完整结构的表格

```html
<table border="1">
    <caption>员工信息表</caption>
    <thead>
        <tr>
            <th>姓名</th>
            <th>部门</th>
            <th>工资</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>张三</td>
            <td>研发部</td>
            <td>10000</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>市场部</td>
            <td>8000</td>
        </tr>
    </tbody>
    <tfoot>
        <tr>
            <td colspan="2">总计</td>
            <td>18000</td>
        </tr>
    </tfoot>
</table>
```

### 4.3 合并单元格示例

```html
<table border="1">
    <tr>
        <td rowspan="2">跨行单元格</td>
        <td>普通单元格</td>
    </tr>
    <tr>
        <td>普通单元格</td>
    </tr>
    <tr>
        <td colspan="2">跨列单元格</td>
    </tr>
</table>
```

## 5. CSS 样式示例

```css
/* 基本表格样式 */
table {
    border-collapse: collapse;
    width: 100%;
}

th, td {
    border: 1px solid #ddd;
    padding: 8px;
    text-align: left;
}

th {
    background-color: #f2f2f2;
}

/* 隔行变色 */
tr:nth-child(even) {
    background-color: #f9f9f9;
}

/* 鼠标悬停效果 */
tr:hover {
    background-color: #f5f5f5;
}
```

## 6. 最佳实践

1. **结构化布局**

   - 使用 `<thead>`, `<tbody>`, `<tfoot>` 组织表格结构
   - 使用 `<caption>` 提供表格标题
2. **可访问性**

   - 使用 `<th>` 定义表头
   - 为表格添加适当的 ARIA 属性
   - 使用 `scope` 属性指定表头的作用范围
3. **响应式设计**

   ```css
   @media screen and (max-width: 600px) {
       table {
           display: block;
           overflow-x: auto;
       }
   }
   ```
4. **性能考虑**

   - 避免过度使用合并单元格
   - 大型表格考虑使用虚拟滚动
   - 适当使用 `table-layout: fixed`

## 7. 注意事项

1. 不要使用表格做页面布局
2. 合理使用表格标签的语义化结构
3. 考虑移动设备的适配
4. 注意表格的可访问性
5. 大数据表格考虑分页或懒加载

## 8. 补充说明

1. HTML5 中一些属性已弃用，建议使用 CSS 代替：

   - `cellpadding` → `padding`
   - `cellspacing` → `border-spacing`
   - `align` → `text-align`
   - `bgcolor` → `background-color`
2. 现代开发中常用的表格增强功能：

   - 排序
   - 筛选
   - 分页
   - 固定表头
   - 列宽调整

这些功能通常通过 JavaScript 库实现，如：

- DataTables
- ag-Grid
- Element UI Table
- Ant Design Table
