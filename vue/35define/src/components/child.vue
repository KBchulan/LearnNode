<template>
  <h1>子组件</h1>
  <div>
    name: {{ name }}
  </div>
  <div>
    <button @click="sendToFather">派发</button>
  </div>
</template>

<script generic="T extends object" setup lang="ts">
import type { VNode } from 'vue'
const props = defineProps<{
  // name: string[]  // 这是原来就有的写法，但是比如我传过来的数组类型会改变呢，如变成一个number[]类型的数组
  name: T[] // 对defineProps增加了泛型的支持
}>()
console.log(props.name)

// const emit = defineEmits(['on_son_click'])
const emit = defineEmits<{
  on_son_click: [string[]]  // defineEmits也有升级的写法
}>()
const sendToFather = () => {
  emit('on_son_click', ['son_prop_1', 'son_prop_2', 'son_prop_3'])
}

// defineOptions 是vue3.3新增的api，用于定义组件的名称，其他optionsAPI的也是支持，我们主要是自定义当前组件名称
defineOptions(
  {
    name: 'child-change',
  }
)

// defineSlots，目前只有声明没有实现，主要作用就是约束插槽
defineSlots<{
  default: (props: {
    name: string
  }) => VNode[]
}>()
</script>

<style scoped lang="less"></style> 