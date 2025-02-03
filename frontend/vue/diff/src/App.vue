<script setup lang="ts">
const arr: string[] = ['a', 'b', 'c', 'd']


</script>

<template>
  <div>
    <div v-for="item in arr" :key="item">
      {{ item }}
    </div>
  </div>

</template>

<style scoped lang="scss"></style>
