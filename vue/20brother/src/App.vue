<script setup lang="ts">
// 兄弟组件的传参其实很简单，我们可以把父组件当作一个桥梁，进行传递即可
import { ref } from 'vue';
import A from './components/A.vue'
import B from './components/B.vue'

let FlagInFather = ref<boolean>(false)
const getFlag = (params: boolean) => {
  FlagInFather.value = params
  console.log(FlagInFather.value)
}

</script>

<template>
  <div>
    <A @on-AbuttonClick="getFlag"></A>
    <B :flag="FlagInFather"></B>
  </div>
</template>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 20px;
  padding-left: 20px;
}
</style>
