<script setup lang="ts">
import { ref, toRef, watch } from 'vue'
let msg1 = ref<string>('张三')
let msg2 = ref<string>('李四')
let obj = ref({
  foo: {
    bar: {
      name: '王五',
      age: 45
    }
  }
})

// watch是用于响应数据变化的特性，用于监听特定的响应式数据，并在数据变化时执行相应的回调函数

// 监听单个元素
watch(msg1, (value, oldValue) => {
  console.log(value, oldValue)   // 此时的value和oldValue是被监听的响应式数据的值
})

// 监听多个元素
watch([msg1, msg2], (value, oldValue) => {
  console.log(value, oldValue)     // 此时的value和oldValue是被监听的响应式数据的数组集合
})

// 监听一个对象，此时会比较奇怪，响应式对象会返回一样的新值和旧值
watch(obj, (value, oldValue) => {
  console.log(value, oldValue)
}, {
  deep: true,        // 深度监听：比如ref对象层级比较深，就需要开启深度监听，注意reactive对象默认开启深度监听，且无法关闭
  immediate: true,   // 立即执行：默认为false，如果为true，会先执行一次callback
  flush: "pre"       // pre组件更新之前执行，sync组件更新的同步执行，post组件更新后执行
})

// 如果想监听一个对象的具体属性，我们会发现一个事情，proxy不能代理string，故有两种解决方案
watch(() => obj.value.foo.bar.name, () => { })
watch(toRef(obj.value.foo.bar.name), () => { })

</script>

<template>

  <div>
    case1: <input v-model="msg1" type="text">
    <hr>
    case2: <input v-model="msg2" type="text">
    <hr>
    case3: <input v-model="obj.foo.bar.name" type="text">
    <hr>
    case4: <input v-model="obj.foo.bar.age" type="text">
  </div>

</template>

<style scoped></style>
