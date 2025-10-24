<script setup lang="ts">
import { inject } from 'vue'
import type { Ref } from 'vue'

const color = inject<Ref<string>>('color')
</script>

<template>
  <hr>
  <div class="hx-father">
    <h1>father.vue(父组件)</h1>
    <div class="hx-father__box"></div>
  </div>
  <son></son>
</template>

<style scoped lang="scss">
@include block(father) {
  color: #fff;

  @include element(box) {
    width: 150px;
    height: 150px;
    border: 1px solid #ccc;
    background-color: v-bind(color);
  }
}
</style>