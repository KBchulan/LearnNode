# Pinia 入门指南

## 什么是 Pinia？

Pinia 是 Vue.js 的存储库，可以跨组件/页面共享状态。它具有以下特点：

- 完整的 TypeScript 支持
- 轻量级（体积约 1kb）
- 模块化设计
- 开发工具支持
- 服务端渲染支持
- Vue 2 和 Vue 3 兼容

## 安装

```bash
# 使用 npm
npm install pinia

# 使用 yarn
yarn add pinia

# 使用 pnpm
pnpm add pinia
```

## 在 Vue 应用中配置 Pinia

```javascript
import { createApp } from 'vue'
import { createPinia } from 'pinia'
import App from './App.vue'

const pinia = createPinia()
const app = createApp(App)

app.use(pinia)
app.mount('#app')
```

## Store 的基本概念

存储管理应用程序状态（数据）的地方，它是 store 的核心属性，用于集中管理和共享多个组件之间需要访问的数据。

### 定义 Store

```javascript
// stores/counter.js
import { defineStore } from 'pinia'

export const useCounterStore = defineStore('counter', {
  // state
  state: () => ({
    count: 0,
    name: 'Counter'
  }),
  
  // getters
  getters: {
    doubleCount: (state) => state.count * 2,
  },
  
  // actions
  actions: {
    increment() {
      this.count++
    },
    decrement() {
      this.count--
    }
  }
})
```

### 使用 Store

```vue
<template>
  <div>
    <p>当前计数：{{ counter.count }}</p>
    <p>双倍计数：{{ counter.doubleCount }}</p>
    <button @click="counter.increment()">增加</button>
    <button @click="counter.decrement()">减少</button>
  </div>
</template>

<script setup>
import { useCounterStore } from '@/stores/counter'

const counter = useCounterStore()
</script>
```

## Pinia 的核心概念

### 1. State（状态）

State 是 store 的核心部分，代表数据的来源：

```javascript
const useStore = defineStore('storeId', {
  state: () => ({
    user: null,
    items: [],
    isLoading: false
  })
})
```

### 2. Getters（计算属性）

Getters 相当于 store 的计算属性：

```javascript
const useStore = defineStore('storeId', {
  state: () => ({
    todos: []
  }),
  getters: {
    finishedTodos: (state) => state.todos.filter(todo => todo.done),
    unfinishedTodos: (state) => state.todos.filter(todo => !todo.done)
  }
})
```

### 3. Actions（方法）

Actions 相当于组件中的方法，可以包含任意异步操作：

```javascript
const useStore = defineStore('storeId', {
  state: () => ({
    posts: []
  }),
  actions: {
    async fetchPosts() {
      this.posts = await api.getPosts()
    },
    async createPost(post) {
      await api.createPost(post)
      this.posts.push(post)
    }
  }
})
```

## 组合式 API 写法

Pinia 也支持使用组合式 API 的方式定义 store：

```javascript
import { ref, computed } from 'vue'
import { defineStore } from 'pinia'

export const useCounterStore = defineStore('counter', () => {
  // state
  const count = ref(0)
  
  // getters
  const doubleCount = computed(() => count.value * 2)
  
  // actions
  function increment() {
    count.value++
  }
  
  return { count, doubleCount, increment }
})
```

## 状态持久化

可以使用 `pinia-plugin-persistedstate` 插件实现状态持久化：

```javascript
import { createPinia } from 'pinia'
import piniaPluginPersistedstate from 'pinia-plugin-persistedstate'

const pinia = createPinia()
pinia.use(piniaPluginPersistedstate)
```

在 store 中配置持久化：

```javascript
export const useStore = defineStore('main', {
  state: () => ({
    someState: 'hello pinia'
  }),
  persist: true
})
```

## 最佳实践

1. **模块化管理**：将不同功能的 store 分开管理
2. **类型支持**：善用 TypeScript 提供的类型支持
3. **合理使用**：不要将所有状态都放在 store 中，只存储需要共享的状态
4. **命名规范**：使用统一的命名规范，如 `useXxxStore`

## 调试

Pinia 默认集成了 Vue DevTools 支持，你可以在浏览器的 Vue DevTools 中查看和调试 Pinia store。

## 注意事项

1. store 应该只存储全局状态
2. 避免在 store 中存储可以由组件自身管理的状态
3. 适当使用 getters 来处理派生状态
4. actions 中的异步操作要做好错误处理

## 总结

Pinia 作为 Vue 官方推荐的状态管理方案，提供了：

- 简单直观的 API
- 完整的类型推导支持
- 模块化的设计
- 出色的开发者工具支持
- 良好的可扩展性

其实可以这样概括，vue的pinia就是存储一个reactive的全局对象，方便各个组件共享同一属性和方法
: 由于是reactive，所以可以使用computed,watch,ref等响应式api