<template>
  <div>
    <header class="header">
      <div>left</div>
      <div>center</div>
      <div>right</div>
    </header>
    <div>
      <button @click="change(36)">大------</button>
      <button @click="change(24)">中------</button>
      <button @click="change(16)">小------</button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { useCssVar } from '@vueuse/core'
const change = (num: number) => {
  const size = useCssVar('--font-size')
  size.value = `${num}px`
}
</script>

<style lang="less">
:root {
  --font-size: 16px;
}

.header {
  display: flex;

  div {
    height: 50px;
    line-height: 50px;
    color: #fff;
    text-align: center;
    font-size: var(--font-size);
  }

  /**
    我们想做自适应的话，这个px就不好使了
    px: 绝对单位，不随屏幕大小变化

    那用rem怎么样，因为rem是相对单位，是根据根元素的font-size来变化的，我们想要算出来还需要引入淘宝的lib-flexible

    所以我们可以使用vw, vh来实现自适应

    vw: 视口宽度，1vw = 1%的视口宽度
    vh: 视口高度，1vh = 1%的视口高度

    但是注意，这个不是百分比，因为百分比是基于父元素的，而vw, vh是基于视口的
  */
  div:nth-child(1) {
    width: 100px;
    background-color: red;
  }

  div:nth-child(2) {
    flex: 1;
    background-color: blue;
  }

  div:nth-child(3) {
    width: 100px;
    background-color: green;
  }
}
</style>