<template>
  <!-- 写一些标签一类的 -->
  
  <!-- 1.vue模板：模板可以支持运算一类的，也可以支持API调用 -->
  <p>
    {{ sum() ? 'true' : 'false' }}<br>
    {{ arr1.map(value => ({ num: value })) }}
  </p>

  <!-- 2.vue指令 -->
  <span v-text="str1"></span>         <!-- 可以解析文本 -->
  <strong v-html="str2"></strong>     <!-- 可以解析html标签 -->
  
  <!-- 可以解析布尔值，为true显示true，为false会被更改为注释节点 -->
  <em v-if="bool1">{{ bool1 }}</em>
  <em v-if="bool2">{{ bool2 }}</em>

  <!-- 同样也是解析布尔值，但与v-if不同的是，v-show在false时会把css的display属性设置为none，并不是注释 -->
  <div v-show="bool1">{{ bool1 }}</div>  
  <div v-show="bool2">{{ bool2 }}</div>
  
</template>

<script setup lang="ts">
// 写一些js/ts代码

// module 1
const a: number = 1
const b: number = 2
const sum = () => {
  return a + b
}

const arr1: number[] = [1, 2, 3]

// module 2
const str1: string = 'this is a string'
const str2: string = '<section style="color: green">this is a section</section>'
const bool1: boolean = true
const bool2: boolean = false

</script>

<style>
/* 本位置写一些样式一类的 */
</style>
