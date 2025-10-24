<template>
  <div>
    <img id="skygirl" width="400" height="520" src="../assets/1.jpg" alt="">
  </div>
</template>

<script setup lang="ts">
// 我们在前面需要借助defineProps来接收父组件传递过来的参数
const props = defineProps<{
  a: string
  b: string
  num: number
}>()
console.log(props.num, props.a, props.b)

// 我们还可以使用useAttrs这个钩子来接收父组件传递过来的属性
import { useAttrs } from 'vue'      // 还有比如useSlots这些东西
const attrs = useAttrs()
console.log(attrs)

// 自定义hooks
import useBase64 from '../hooks/index'
useBase64({
  el: '#skygirl'
}).then((res) => {
  console.log(res.baseUrl)
})
</script>

<style scoped lang="less"></style>