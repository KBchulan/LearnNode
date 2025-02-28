<script setup lang="ts">
import { reactive, toRef, watch } from 'vue'
import Inner from './components/inner.vue'
import gsap from 'gsap'

const num = reactive({
  current: 0,
  tweendNumber: 0
})

watch(toRef(num, 'current'), (newVal) => {
  gsap.to(num, {
    duration:1,
    tweendNumber: newVal
  })
})

</script>

<template>
  <div>
    <input type="number" step="20" v-model="num.current">
    <div>
      {{ num.tweendNumber }}
    </div>
  </div>

  <div>
    <Inner></Inner>
  </div>
</template>

<style lang="scss"></style>
