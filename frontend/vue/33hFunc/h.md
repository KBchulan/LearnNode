
# Vue 3 中的 h 函数详细指南

## 1. 什么是 h 函数?

h 函数是 Vue 3 中的一个核心函数,用于创建虚拟 DOM(VNode)。它是 `createVNode` 函数的一个更友好的封装版本,主要用于手写渲染函数时使用。"h" 来自于 hyperscript 的缩写,这个命名常见于虚拟 DOM 实现中。

## 2. 基本语法

h 函数的基本语法有以下几种形式:

```typescript
// 1. 仅类型
h('div')

// 2. 类型 + 属性
h('div', { id: 'foo' })

// 3. 类型 + 子节点（忽略属性）
h('div', ['子节点数组'])           // 数组子节点
h('div', '文本内容')              // 文本子节点
h('div', h('span'))              // 单个VNode子节点
h(Component, () => {})           // 默认插槽

// 4. 类型 + 属性 + 子节点
h('div', {}, ['子节点数组'])      // 数组子节点 
h('div', {}, '文本内容')          // 文本子节点
h('div', {}, h('span'))          // 单个VNode子节点
h(Component, {}, () => {})       // 默认插槽

// 5. 命名插槽（需要显式传null作为属性）
h(Component, null, {
  foo: () => h('div', '具名插槽内容')
})
```

## 3. 参数详解

h 函数接收1-3个参数:

### 3.1 第一个参数 - type

可以是以下几种类型:

- 字符串: HTML 标签名
- 组件对象
- Fragment
- Text
- Comment
- Teleport
- Suspense

### 3.2 第二个参数 - props (可选)

一个对象,包含:

- DOM 属性
- HTML 属性
- 组件 props
- 事件监听器
- key
- ref
- 等其他特殊属性

### 3.3 第三个参数 - children (可选)

可以是以下几种类型:

- 字符串: 文本内容
- 数组: 多个子节点
- 对象: 具名插槽
- 函数: 默认插槽
- VNode: 单个子节点

## 4. 实际使用示例

### 4.1 基础 DOM 元素

```typescript
// 创建一个 div
h('div')

// 带属性的 div
h('div', { class: 'foo', onClick: () => {} })

// 带子元素的 div
h('div', [
  h('span', 'Hello'),
  h('span', 'World')
])
```

### 4.2 组件

```typescript
// 基础组件渲染
h(MyComponent)

// 带 props 的组件
h(MyComponent, {
  prop1: 'value1',
  prop2: 'value2'
})

// 带插槽的组件
h(MyComponent, null, {
  default: () => h('div', 'Default Slot'),
  named: () => h('div', 'Named Slot')
})
```

### 4.3 条件渲染

```typescript
h('div', [
  condition 
    ? h('div', 'True') 
    : h('span', 'False')
])
```

### 4.4 列表渲染

```typescript
h('ul', 
  items.map(item => 
    h('li', { key: item.id }, item.text)
  )
)
```

## 5. 最佳实践

1. **类型提示**: 使用 TypeScript 时建议为 props 添加类型注解以获得更好的类型提示。
2. **Key 管理**: 在渲染列表时始终提供 key 属性。
3. **Props 验证**: 在组件中定义 props 类型和默认值。
4. **性能优化**: 对于大型列表考虑使用虚拟滚动。
5. **代码组织**: 对于复杂的渲染函数，将其拆分为更小的函数以提高可维护性。

## 6. 注意事项

1. h 函数返回的是虚拟节点(VNode),而不是实际的 DOM 元素。
2. 当使用 TypeScript 时,建议使用 defineComponent 来包装组件以获得更好的类型推导。
3. 相比模板语法,h 函数的写法更加灵活但也更复杂,建议在确实需要的场景下使用。
4. 使用 h 函数时需要显式导入:

```typescript
import { h } from 'vue'
```

5. 在处理事件时注意使用驼峰命名(onClick)而不是 kebab-case(on-click)。
