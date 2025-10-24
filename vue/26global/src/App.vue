<template>
  <div>
    {{ $age }}
    <hr>
    {{ $filiter.format('123') }}
    <hr>
    {{ $mmm }}
  </div>
</template>

<script setup lang="ts">
import { getCurrentInstance } from 'vue'

const app = getCurrentInstance()
console.log(app?.proxy?.$age)    // 这里获取到的本质是proxy代理的get方法获取的

</script>

<style scoped>

</style>