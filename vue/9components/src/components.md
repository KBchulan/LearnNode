# Vue 组件完全指南

*Last updated: 2025-02-22 06:30:29 UTC by KBchulan*

## 1. 组件基础

### 1.1 组件定义

```typescript
<script setup lang="ts">
import { ref } from 'vue'

// 数据
const count = ref(0)

// 方法
const increment = () => {
  count.value++
}
</script>

<template>
  <div class="counter">
    <p>Count: {{ count }}</p>
    <button @click="increment">+1</button>
  </div>
</template>

<style scoped>
.counter {
  padding: 10px;
}
</style>
```

### 1.2 组件注册

```typescript
// 全局注册
import { createApp } from 'vue'
import App from './App.vue'
import GlobalComponent from './components/GlobalComponent.vue'

const app = createApp(App)
app.component('global-component', GlobalComponent)

// 局部注册
import LocalComponent from './components/LocalComponent.vue'

export default {
  components: {
    LocalComponent
  }
}
```

## 2. 组件通信

### 2.1 Props 父传子

```typescript
// 子组件
<script setup lang="ts">
interface Props {
  title: string
  count?: number
}

// 定义 props
defineProps<Props>()
</script>

<template>
  <div>
    <h2>{{ title }}</h2>
    <p>Count: {{ count }}</p>
  </div>
</template>

// 父组件使用
<template>
  <child-component
    title="My Title"
    :count="42"
  />
</template>
```

### 2.2 Emits 子传父

```typescript
// 子组件
<script setup lang="ts">
const emit = defineEmits<{
  (e: 'update', value: number): void
  (e: 'delete'): void
}>()

const handleClick = () => {
  emit('update', 100)
}
</script>

// 父组件
<template>
  <child-component
    @update="(value) => handleUpdate(value)"
    @delete="handleDelete"
  />
</template>
```

### 2.3 v-model 双向绑定

```typescript
// 子组件
<script setup lang="ts">
const props = defineProps<{
  modelValue: string
}>()

const emit = defineEmits<{
  (e: 'update:modelValue', value: string): void
}>()
</script>

<template>
  <input
    :value="modelValue"
    @input="e => emit('update:modelValue', e.target.value)"
  >
</template>

// 父组件使用
<template>
  <custom-input v-model="searchText" />
</template>
```

## 3. 组件生命周期

```typescript
<script setup lang="ts">
import { onMounted, onUpdated, onUnmounted, onBeforeMount } from 'vue'

// 组件挂载前
onBeforeMount(() => {
  console.log('组件挂载前')
})

// 组件挂载后
onMounted(() => {
  console.log('组件已挂载')
})

// 组件更新后
onUpdated(() => {
  console.log('组件已更新')
})

// 组件卸载时
onUnmounted(() => {
  console.log('组件已卸载')
})
</script>
```

## 4. 组件复用

### 4.1 组合式函数 (Composables)

```typescript
// 定义组合式函数
import { ref } from 'vue'

export function useCounter(initialValue = 0) {
  const count = ref(initialValue)
  
  const increment = () => count.value++
  const decrement = () => count.value--
  
  return {
    count,
    increment,
    decrement
  }
}

// 在组件中使用
import { useCounter } from './composables/useCounter'

const { count, increment, decrement } = useCounter(10)
```

### 4.2 插槽 (Slots)

```typescript
// 带插槽的组件
<template>
  <div class="card">
    <header>
      <slot name="header">默认标题</slot>
    </header>
    <main>
      <slot>默认内容</slot>
    </main>
    <footer>
      <slot name="footer"></slot>
    </footer>
  </div>
</template>

// 使用插槽
<card>
  <template #header>
    自定义标题
  </template>
  <template #default>
    主要内容
  </template>
  <template #footer>
    页脚内容
  </template>
</card>
```

## 5. 组件优化

### 5.1 动态组件

```typescript
<script setup lang="ts">
import { ref } from 'vue'
import ComponentA from './ComponentA.vue'
import ComponentB from './ComponentB.vue'

const currentComponent = ref('ComponentA')
</script>

<template>
  <component :is="currentComponent" />
  <button @click="currentComponent = 'ComponentB'">
    切换组件
  </button>
</template>
```

### 5.2 异步组件

```typescript
import { defineAsyncComponent } from 'vue'

// 基础用法
const AsyncComp = defineAsyncComponent(() =>
  import('./components/AsyncComponent.vue')
)

// 带选项的用法
const AsyncCompWithOptions = defineAsyncComponent({
  loader: () => import('./components/AsyncComponent.vue'),
  loadingComponent: LoadingComponent,
  errorComponent: ErrorComponent,
  delay: 200,
  timeout: 3000
})
```

## 6. 组件状态管理

### 6.1 Provide/Inject

```typescript
// 父组件提供数据
<script setup lang="ts">
import { provide, ref } from 'vue'

const theme = ref('dark')
provide('theme', theme)
</script>

// 子组件注入数据
<script setup lang="ts">
import { inject } from 'vue'

const theme = inject('theme', 'light') // 默认值 light
</script>
```

### 6.2 Pinia 状态管理

```typescript
import { defineStore } from 'pinia'

export const useUserStore = defineStore('user', {
  state: () => ({
    name: '',
    isLoggedIn: false
  }),
  actions: {
    login(username: string) {
      this.name = username
      this.isLoggedIn = true
    },
    logout() {
      this.name = ''
      this.isLoggedIn = false
    }
  }
})
```

## 7. 最佳实践

1. **组件命名规范**

   - 使用 PascalCase 命名组件
   - 使用多词组合避免与 HTML 元素冲突
2. **组件结构**

   ```
   src/
   ├── components/
   │   ├── common/       # 通用组件
   │   ├── layout/       # 布局组件
   │   └── business/     # 业务组件
   ├── composables/      # 组合式函数
   └── views/            # 页面组件
   ```
3. **性能优化**

   - 合理使用 `v-show` 和 `v-if`
   - 使用 `keep-alive` 缓存组件
   - 使用计算属性代替复杂的模板表达式
4. **代码复用**

   - 抽取通用逻辑到组合式函数
   - 使用混入或插件封装全局功能
   - 合理使用继承和组合

## 8. 常见问题解决

1. **组件通信问题**

   - 简单父子通信用 props/emit
   - 跨层级通信用 provide/inject
   - 全局状态管理用 Pinia
2. **性能问题**

   - 大型列表使用虚拟滚动
   - 频繁更新使用防抖/节流
   - 合理使用异步组件
3. **状态管理**

   - 局部状态用 ref/reactive
   - 共享状态用 Pinia
   - 临时状态用本地存储
