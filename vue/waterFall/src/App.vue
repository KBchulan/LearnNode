<script setup lang="ts">
import waterFall from './components/waterFall.vue'

// 设定瀑布初始化参数
const list = [
  {
    height: 300,
    background: 'red'
  },
  {
    height: 400,
    background: 'pink'
  },
  {
    height: 500,
    background: 'blue'
  },
  {
    height: 350,
    background: 'green'
  },
  {
    height: 100,
    background: 'palegreen'
  },
  {
    height: 150,
    background: 'paleturquoise'
  },
  {
    height: 80,
    background: 'purple'
  },
  {
    height: 50,
    background: 'yellowgreen'
  },
  {
    height: 80,
    background: 'red'
  },
  {
    height: 160,
    background: 'blue'
  },
  {
    height: 90,
    background: 'yellowgreen'
  },
  {
    height: 220,
    background: 'palegreen'
  },
  {
    height: 170,
    background: 'pink'
  },
  {
    height: 360,
    background: 'purple'
  },
  {
    height: 410,
    background: 'green'
  },
  {
    height: 290,
    background: 'yellowgreen'
  }
]

</script>

<template>
  <waterFall :list="list"></waterFall>
</template>

<style lang="scss">
#app {
  @include bfc;
}
</style>
