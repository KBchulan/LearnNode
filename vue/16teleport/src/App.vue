<script setup lang="ts">
// 其实teleport很简单，一句话说来就是把Teleport标签内部的内容传送到to里面指定的位置
</script>

<template>
  <div class="hx-app">
    APP级别
    <Teleport to="body" :disabled="false">
      <inner></inner>
    </Teleport>
  </div>
</template>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 3px;
  padding-left: 6px;
}

@include block(app) {
  height: 50vh;
  font-size: 50px ;
  position: relative;   // 此时这个inner组件会基于自己的父级而不是body做absolute，此时我们就可以通过teleport传送组件
  background-color: aliceblue;
  font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
}
</style>
