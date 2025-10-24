<template>
  <div v-move class="box">
    <div class="header" :class="[flag ? 'headerhover' : '']">可拖拽的框</div>
    <div class="content">内容</div>
  </div>
</template>

<script setup lang="ts">
import { ref, type Directive } from 'vue'

let flag = ref<boolean>(false)

const vMove: Directive = (el: HTMLElement) => {
  flag.value = true
  let moveElement: HTMLDivElement = el.firstElementChild as HTMLDivElement

  const mouseDown = (e: MouseEvent) => {
    let X = e.clientX - el.offsetLeft
    let Y = e.clientY - el.offsetTop
    const move = (ee: MouseEvent) => {
      el.style.left = ee.clientX - X + 'px'
      el.style.top = ee.clientY - Y + 'px'
    }
    document.addEventListener('mousemove', move)
    document.addEventListener('mouseup', () => {
      flag.value = false
      document.removeEventListener('mousemove', move)
    })
  }
  moveElement.addEventListener('mousedown', mouseDown)
}
</script>

<style scoped lang="less">
.box {
  width: 400px;
  height: 400px;
  border: 1px solid rgb(167, 187, 172);
  position: absolute;

  .header {
    height: 40px;
    background-color: beige;

    // 标题居中
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .headerhover {
    cursor: pointer;
  }

  .content {
    // 文本居中
    display: grid;
    justify-content: center;

    // 配色
    color: aliceblue;
  }
}
</style>