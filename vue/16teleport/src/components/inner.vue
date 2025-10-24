<script setup lang="ts">
</script>

<template>
  <div class="hx-dialog">
    <header class="hx-dialog__header">
      <div>这是一个弹框</div>
    </header>

    <main class="hx-dialog__main">
      内容
    </main>

    <footer class="hx-dialog__footer">
      <button>取消</button>
      <button>确认</button>
    </footer>
  </div>
</template>

<style scoped lang="scss">
@include block(dialog) {
  width: 400px;
  height: 400px;
  background-color: #141414;
  display: flex;
  flex-direction: column;
  position: absolute;
  left: 50%;
  top: 50%;
  margin-left: -200px;
  margin-top: -200px;

  @include element(header) {
    color:aqua;
    border-bottom: 1px solid #ccc;
  }

  @include element(main) {
    color:aqua;
    border-bottom: 1px solid #ccc;
  }

  @include element(footer) {
    color:aqua;
  }
}
</style>