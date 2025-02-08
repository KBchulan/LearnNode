<script setup lang="ts">
import { reactive, toRef, toRefs, toRaw } from 'vue'
let huaixi = {
  name: 'huaixi',
  age: 18,
  language: 'c++'
}

const change = (): void => {
  huaixi.language = 'go'
  console.log(huaixi)
}

</script>

<template>
  <div>
    {{ huaixi }}
  </div>
  <hr>
  <button @click="change">修改</button>
</template>

<style scoped></style>
