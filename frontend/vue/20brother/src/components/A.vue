<script setup lang="ts">
import { ref } from 'vue'
import Bus from '../Bus'

const emit = defineEmits(['on-AbuttonClick'])
let flag = ref<boolean>(false)
const sendToB = (): void => {
  flag.value = !flag.value
  emit('on-AbuttonClick', flag.value)
}

let str:string = '我是A组件的属性'
const sendByBus = ()=>{
  Bus.emit('on-click-bus', str)
}
</script>

<template>
  <div>
    <button @click="sendToB">发送给B</button>
    <button @click="sendByBus">通过Bus发送</button>
  </div>
</template>

<style scoped lang="scss"></style>