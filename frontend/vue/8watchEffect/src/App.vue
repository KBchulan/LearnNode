<script setup lang="ts">
import { ref, watchEffect } from 'vue'
let msg1 = ref<string>('s1')
let msg2 = ref<string>('s2')

// watchEffect是非惰性的，进入页面就会触发，而且直接写对应响应式对象即可监听，这个写法比较类似于compted的第二种重载
// 要想停止一个监听器，请调用watch或watchEffect返回的函数
const stop = watchEffect((on) => {
  let ipt: HTMLInputElement = document.querySelector('#ipt')!
  console.log(ipt, 'iptDom')

  console.log('msg1:', msg1.value)
  console.log('msg2:', msg2.value)
  on(() => {
    console.log('before')
  })
}, {
  flush: "post",   // 如果不加这个选项，那ipt这个dom是不是没有渲染，打印出来是null，可以加上post标志在组件更新之后再执行cb
  onTrigger(event) {
    event.target
    debugger
  }
})

</script>

<template>
  <div>
    <input id="ipt" v-model="msg1" type="text">
    <hr>
    <input v-model="msg2" type="text">
    <hr>
    <button @click="stop()">停止监听</button>
  </div>
</template>

<style scoped></style>
