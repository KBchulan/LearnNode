<script setup lang="ts">
import { inject } from 'vue'
import type { Ref } from 'vue'

const color = inject<Ref<string>>('color')
</script>

<template>
  <hr>
  <div class="hx-son">
    <h1>son.vue(孙子组件)</h1>
    <div class="hx-son__box"></div>
  </div>
</template>

<style scoped lang="scss">
@include block(son) {
  color: #fff;

  @include element(box) {
    width: 150px;
    height: 150px;
    border: 1px solid #ccc;
    background-color: v-bind(color);
  }
}
</style>