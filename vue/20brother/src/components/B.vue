<script setup lang="ts">
import Bus from '../Bus'
const props = defineProps<{
  flag: boolean
}>()

Bus.on('on-click-bus', (params: string) => {
  console.log(params)
})
</script>

<template>
  <div>
    {{ props.flag }}
  </div>
</template>

<style scoped lang="scss"></style>