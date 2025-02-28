<template>
  <div class="father">
    <h1>我是父组件</h1>
    <button @click="isShow = !isShow">控制子组件</button>
    {{ isShow }}
    {{ text }}
    <hr>
    <model v-model="isShow" v-model:text="text"></model>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'

const isShow = ref<boolean>(true)
let text = ref<string>('淮西')
</script>

<style lang="less">
.father {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;

  margin-bottom: 20px;

  color: #fff;

  hr {
    width: 100%;
    margin: 20px 0;
  }
}
</style>