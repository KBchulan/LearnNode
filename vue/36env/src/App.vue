<template>
  <div class="hx-header">
    <h1>Hello World</h1>
  </div>
</template>

<script setup lang="ts">
console.log(import.meta.env)
console.log(import.meta.url)

</script>

<style lang="scss">
@include block('header') {
  font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
  font-size: 20px;
  color: #fff;
}
</style>
