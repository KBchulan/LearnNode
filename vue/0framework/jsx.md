# JSX 详解

JSX (JavaScript XML) 是 React 引入的一个语法扩展，允许在 JavaScript 代码中编写类似 HTML 的代码。

## 1. 基本语法

```jsx
// 基本 JSX 语法
const element = <h1>Hello, World!</h1>;

// 多行 JSX
const element = (
  <div>
    <h1>Title</h1>
    <p>Paragraph</p>
  </div>
);
```

## 2. JSX 特性

### 2.1 表达式嵌入

```jsx
const name = 'John';
const element = <h1>Hello, {name}!</h1>;

// 可以使用任何 JavaScript 表达式
const element = <h1>2 + 2 = {2 + 2}</h1>;
```

### 2.2 属性传递

```jsx
// 传递字符串
const element = <div className="container">Hello</div>;

// 传递变量
const imgUrl = "https://example.com/image.jpg";
const element = <img src={imgUrl} />;
```

### 2.3 子元素

```jsx
const element = (
  <div>
    <h1>Title</h1>
    {/* 这是注释 */}
    <p>Paragraph</p>
    {isLoggedIn && <UserGreeting />}
  </div>
);
```

## 3. JSX 转换

JSX 会被 Babel 转换为普通的 JavaScript 代码：

```jsx
// JSX 代码
const element = <h1 className="greeting">Hello, world!</h1>;

// 转换后的 JavaScript 代码
const element = React.createElement(
  'h1',
  { className: 'greeting' },
  'Hello, world!'
);
```

## 4. 常见用法

### 4.1 条件渲染

```jsx
function Greeting({ isLoggedIn }) {
  return (
    <div>
      {isLoggedIn ? (
        <h1>Welcome back!</h1>
      ) : (
        <h1>Please log in</h1>
      )}
    </div>
  );
}
```

### 4.2 列表渲染

```jsx
function List({ items }) {
  return (
    <ul>
      {items.map(item => (
        <li key={item.id}>{item.text}</li>
      ))}
    </ul>
  );
}
```

### 4.3 事件处理

```jsx
function Button() {
  const handleClick = () => {
    alert('Button clicked!');
  };

  return (
    <button onClick={handleClick}>
      Click me
    </button>
  );
}
```

## 5. JSX 最佳实践

### 5.1 组件化

```jsx
// 函数组件
function Welcome(props) {
  return <h1>Hello, {props.name}</h1>;
}

// 使用组件
const element = <Welcome name="Sara" />;
```

### 5.2 样式处理

```jsx
// 内联样式
const style = { backgroundColor: 'red', color: 'white' };
const element = <div style={style}>Styled content</div>;

// 类名
const element = <div className="my-class">Styled content</div>;
```

### 5.3 Fragment 使用

```jsx
// 避免额外的 DOM 节点
function Fragment() {
  return (
    <>
      <h1>Title</h1>
      <p>Paragraph</p>
    </>
  );
}
```

## 6. 注意事项

1. **属性命名**：

   - 使用 camelCase 命名
   - class 要写成 className
   - for 要写成 htmlFor
2. **闭合标签**：

   - 所有标签都必须闭合
   - 可以使用自闭合标签 `<img />`
3. **根元素**：

   - JSX 表达式必须只有一个根元素
   - 可以使用 Fragment 避免多余的 DOM 节点
4. **注释写法**：

```jsx
{/* JSX 中的注释这样写 */}
```

## 7. 实际项目示例

```jsx
// 一个完整的 React 组件示例
import React, { useState } from 'react';

function UserProfile({ user }) {
  const [isEditing, setIsEditing] = useState(false);

  return (
    <div className="user-profile">
      <h2>{user.name}'s Profile</h2>
      {isEditing ? (
        <form onSubmit={handleSubmit}>
          <input
            type="text"
            value={user.name}
            onChange={handleNameChange}
          />
          <button type="submit">Save</button>
        </form>
      ) : (
        <div>
          <p>Email: {user.email}</p>
          <button onClick={() => setIsEditing(true)}>
            Edit Profile
          </button>
        </div>
      )}
    </div>
  );
}
```
