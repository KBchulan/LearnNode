<script setup lang="ts">
import { ref, onBeforeMount, onMounted, onBeforeUpdate, onUpdated, onBeforeUnmount, onUnmounted } from 'vue'

const str = ref<string>('colate')
const div = ref<HTMLDivElement>()
const change = () => {
  str.value = '我被改了'
}

// setup模式
console.log('setup')

// dom挂载时调用的钩子，挂载也称渲染
// onBeforeMount之前是读不到dom的，挂载之后才能读到dom元素
onBeforeMount(() => {
  console.log('挂载之前：', div.value)
})
onMounted(() => {
  console.log('挂载完成以后：', div.value)
})

// 更新dom触发更新的钩子
onBeforeUpdate(() => {
  console.log('更新之前：', div.value?.innerText)
})
onUpdated(() => {
  console.log('更新完成以后：', div.value?.innerText)
})

// 卸载dom触发卸载的钩子
onBeforeUnmount(() => {
  console.log('销毁之前：', div.value)
})
onUnmounted(() => {
  console.log('卸载完成以后：', div.value)
})

</script>

<template>
  <h2>
    我是一个组件
  </h2>
  <div ref="div">{{ str }}</div>
  <button @click="change">修改</button>
  <hr>
</template>

<style scoped></style>
