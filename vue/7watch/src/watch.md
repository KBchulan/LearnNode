# Vue Watch 完全指南

*Last updated: 2025-02-22 03:36:09 UTC by KBchulan*

## 1. 基本概念

watch 是 Vue 中用于响应数据变化的特性，用于监听特定的响应式数据，并在数据变化时执行相应的回调函数。

## 2. 基础语法

### 2.1 选项式 API

```javascript
export default {
  data() {
    return {
      message: 'Hello'
    }
  },
  watch: {
    message(newValue, oldValue) {
      console.log('值变化了:', oldValue, '=>', newValue)
    }
  }
}
```

### 2.2 组合式 API

```typescript
import { ref, watch } from 'vue'

const message = ref('Hello')

watch(message, (newValue, oldValue) => {
  console.log('值变化了:', oldValue, '=>', newValue)
})
```

## 3. 高级特性

### 3.1 深度监听

```typescript
// 选项式 API
export default {
  data() {
    return {
      user: {
        name: 'John',
        age: 20
      }
    }
  },
  watch: {
    user: {
      handler(newValue, oldValue) {
        console.log('用户信息变化了', newValue)
      },
      deep: true // 开启深度监听
    }
  }
}

// 组合式 API
const user = ref({
  name: 'John',
  age: 20
})

watch(user, (newValue, oldValue) => {
  console.log('用户信息变化了', newValue)
}, { deep: true })
```

### 3.2 立即执行

```typescript
// 选项式 API
export default {
  watch: {
    message: {
      handler(newValue) {
        console.log('消息:', newValue)
      },
      immediate: true // 立即执行一次
    }
  }
}

// 组合式 API
watch(message, (newValue) => {
  console.log('消息:', newValue)
}, { immediate: true })
```

### 3.3 多个数据源监听

```typescript
// 组合式 API
const firstName = ref('John')
const lastName = ref('Doe')

watch([firstName, lastName], ([newFirst, newLast], [oldFirst, oldLast]) => {
  console.log('名字变化了:', 
    `${oldFirst} ${oldLast} => ${newFirst} ${newLast}`)
})
```

## 4. watchEffect

watchEffect 是 Vue 3 中新增的 API，它会自动追踪响应式依赖。

```typescript
import { ref, watchEffect } from 'vue'

const count = ref(0)
const message = ref('Hello')

watchEffect(() => {
  console.log(`count is: ${count.value}`)
  console.log(`message is: ${message.value}`)
})
```

## 5. 停止监听

```typescript
// watch
const stopWatch = watch(message, () => {
  console.log('message changed')
})

// 停止监听
stopWatch()

// watchEffect
const stopEffect = watchEffect(() => {
  console.log(message.value)
})

// 停止监听
stopEffect()
```

## 6. 完整示例

```typescript
import { ref, watch, watchEffect } from 'vue'

// 1. 基础监听
const count = ref(0)
watch(count, (newValue, oldValue) => {
  console.log(`Count changed from ${oldValue} to ${newValue}`)
})

// 2. 深度监听对象
const user = ref({
  name: 'John',
  address: {
    city: 'New York'
  }
})

watch(user, (newValue, oldValue) => {
  console.log('User changed:', newValue)
}, { deep: true })

// 3. 监听多个来源
const firstName = ref('John')
const lastName = ref('Doe')

watch(
  [firstName, lastName],
  ([newFirst, newLast], [oldFirst, oldLast]) => {
    console.log(`Name changed from ${oldFirst} ${oldLast} to ${newFirst} ${newLast}`)
  }
)

// 4. 使用 watchEffect
watchEffect(() => {
  console.log(`Current name is ${firstName.value} ${lastName.value}`)
})

// 5. 条件监听
const shouldWatch = ref(true)
const stopWatch = watch(count, () => {
  if (!shouldWatch.value) {
    stopWatch()
  }
  console.log('Count changed')
})
```

## 7. 最佳实践

1. **选择合适的监听方式**

   - 需要访问旧值时使用 `watch`
   - 需要自动追踪依赖时使用 `watchEffect`
   - 需要深度监听对象时添加 `deep: true`
2. **性能优化**

```typescript
// 优化深度监听
const user = ref({
  name: 'John',
  meta: {
    // 大量数据
  }
})

// 只监听特定属性
watch(() => user.value.name, (newName) => {
  console.log('Name changed to:', newName)
})
```

3. **错误处理**

```typescript
watch(source, (value, oldValue) => {
  try {
    // 处理逻辑
  } catch (error) {
    console.error('Watch error:', error)
  }
})
```

4. **清理副作用**

```typescript
watch(source, (value, oldValue, onCleanup) => {
  const timer = setInterval(() => {
    // 某些操作
  }, 1000)
  
  onCleanup(() => {
    clearInterval(timer)
  })
})
```

## 8. 注意事项

1. **避免在 watch 回调中直接修改被监听的值**
2. **合理使用 deep 选项，避免不必要的深度监听**
3. **及时清理不需要的监听器**
4. **注意监听复杂数据类型时的引用问题**
5. **考虑使用 `watchEffect` 替代简单的 `watch`**

## 9. 常见问题解决

1. **监听对象属性变化**

```typescript
// 正确方式
watch(() => obj.value.property, (newVal) => {
  // 处理变化
})
```

2. **监听数组变化**

```typescript
// 监听数组长度和内容变化
watch(array, (newArray) => {
  // 处理变化
}, { deep: true })
```

3. **处理异步操作**

```typescript
watch(source, async (value) => {
  try {
    await someAsyncOperation()
  } catch (error) {
    console.error(error)
  }
})
```
