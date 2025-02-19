<script setup lang="ts">
let num: number = 1
const cahnge = () => {
  num++
  console.log(num)
}
</script>

<template>
  <div>
    <button v-once @click="cahnge">
      {{ num }}
    </button>
  </div>
</template>

<style scoped></style>
