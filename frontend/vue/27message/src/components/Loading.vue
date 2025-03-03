<template>
  <div v-if="flag" class="message">
    Loading...
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'

let flag = ref<boolean>(false)
const show = () => flag.value = true
const hide = () => flag.value = false

defineExpose({
  show,
  hide
})

</script>

<style scoped>
.message {
  display: flex;
  justify-content: center;
  align-items: center;

  background-color: black;
  opacity: 0.8;

  height: 100vh;
  color: white;
}
</style>