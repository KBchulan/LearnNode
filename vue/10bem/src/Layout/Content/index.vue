<script setup lang="ts">
</script>

<template>
  <div class="hx-content">
    <div class="hx-content__items" v-for="item in 100">{{ item }}</div>
  </div>
</template>

<style scoped lang="scss">
@include block(content) {
  flex: 1;
  overflow: auto;

  @include element(items) {
    padding: 10px;
    margin: 10px;
    border: 1px solid #ccc;
    border-radius: 4px;
  }
}
</style>
