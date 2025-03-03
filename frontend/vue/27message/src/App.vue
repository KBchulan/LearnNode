<template>
  <div></div>
</template>

<script setup lang="ts">
import { getCurrentInstance } from 'vue'

const instance = getCurrentInstance()
instance?.proxy?.$loading.show()
</script>

<style lang="less"></style>