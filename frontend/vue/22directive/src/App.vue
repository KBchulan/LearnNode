<template>
  <div>
    <h1>
      app.vue
    </h1>
    <button @click="changed">change</button>
    <button @click="flag = !flag">unmount</button>
    <div class="hx-sons">
      <did v-if="flag" v-hx:aaa.huaixi="{ background: color }"></did>
      <simble></simble>
    </div>
  </div>
</template>

<script setup lang="ts">
// 本节我们介绍自定义组件
import { ref } from 'vue'
import type { Directive, DirectiveBinding } from 'vue'

function generateRandomNumber(min: number, max: number): number {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

interface Dir {
  background: string
}
let colors = ref<string[]>([
  'red',
  'green',
  'blue'
])

let flag = ref<boolean>(true)
let color = ref(colors.value[generateRandomNumber(0, 2)])

const vHx: Directive = {
  created() {
    console.log('=====>created')
  },
  beforeMount() {
    console.log('=====>beforeMount')
  },
  mounted(el: HTMLElement, dir: DirectiveBinding<Dir>) {
    console.log('=====>mounted')
    el.style.background = dir.value.background
  },
  // 更新是value的值发生变化时会触发的钩子
  beforeUpdate() {
    console.log('=====>beforeUpdate')
  },
  updated(el: HTMLElement, dir: DirectiveBinding<Dir>) {
    console.log('=====>updated')
    el.style.background = dir.value.background
  },
  beforeUnmount() {
    console.log('=====>beforeUnmount')
  },
  unmounted() {
    console.log('=====>unmounted')
  }
}

const changed = () => {
  const num = generateRandomNumber(0, 2)
  color.value = colors.value[num]
}

</script>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 20px;
  padding-left: 20px;
}

@include block(sons) {
  display: flex;
}
</style>