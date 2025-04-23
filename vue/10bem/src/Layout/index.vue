<script setup lang="ts">
import Content from './Content/index.vue'
import Header from './Header/index.vue'
import Menu from './Menu/index.vue'

</script>

<template>
  <div class="hx-box">
    <div class="hx-box__left">
      <Menu></Menu>
    </div>
    <div class="hx-box__right">
      <Header></Header>
      <Content></Content>
    </div>
  </div>
</template>

<style scoped lang="scss">
@include block(box) {
  @include bfc;
  display: flex;

  @include element(left) {
  }

  @include element(right) {
    display: flex;
    flex-direction: column;
    flex: 1;
  }
}
</style>
