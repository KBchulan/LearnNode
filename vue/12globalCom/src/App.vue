<script setup lang="ts">
import { reactive } from 'vue'
import Tree from './components/Tree.vue'

// 本文件夹为全局组件，局部组件 ，递归组件
// import Card from './components/Card.vue'   局部组件

// 下面说一下递归组件
interface Tree {
  name: string,
  checked: boolean,
  children?: Tree[]
}

const data = reactive<Tree[]>([
  {
    name: '1',
    checked: false,
    children: [
      {
        name: '1-1',
        checked: false,
      },
      {
        name: '1-1',
        checked: true,
      }
    ]
  },
  {
    name: '2',
    checked: false
  }, {
    name: '3',
    checked: false,
    children: [
      {
        name: '3-1',
        checked: false,
        children: [
          {
            name: '3-1-1',
            checked: false,
          },
          {
            name: '3-1-2',
            checked: false,
          }
        ]
      }
    ]
  }
])

</script>

<template>
  <!-- <Card></Card> 局部组件 -->
  <!-- <Card></Card> 此时引入的是ts里定义的全局组件 -->
  <Tree :data="data"></Tree>
</template>

<style lang="scss">
#app {
  @include bfc;
  padding: 10px;
}
</style>
