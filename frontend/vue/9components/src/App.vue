<script setup lang="ts">
import { ref } from 'vue';
import A from './components/A.vue'    // 像这样引入组件后可以把 A当作标签使用

let flag = ref<boolean>(true)
</script>

<template>
  <h1>
    生命周期
  </h1>
  <A v-if="flag"></A>
  <button @click="flag = !flag">create-delete</button>
</template>

<style scoped></style>
