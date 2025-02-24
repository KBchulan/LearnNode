<script setup lang="ts">
</script>

<template>
  <div class="hx-card">
    <header class="hx-card__header">
      <div>标题</div>
      <div>副标题</div>
    </header>

    <section class="hx-card__section">
      内容
    </section>
  </div>
</template>

<style scoped lang="scss">
@include block(card) {
  border: 1px solid #ccc;
  width: 500px;

  @include element(header) {
    display: flex;
    justify-content: space-between;
    padding: 5px;
    border-bottom: 1px solid #ccc;
  }

  @include element(section) {
    padding: 5px;
    min-height: 300px;
  }
}
</style>
