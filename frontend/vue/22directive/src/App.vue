<template>
  <div>
    <h1>
      app.vue
    </h1>
    <button @click="change">change</button>
    <button @click="flag = !flag">unmount</button>
    <did v-if="flag" v-hx:aaa.huaixi="{ background: color }"></did>
  </div>
</template>

<script setup lang="ts">
import { ref, Directive, DirectiveBinding } from 'vue'

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
let color = colors.value[generateRandomNumber(0, 2)]

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
  updated() {
    console.log('=====>updated')
  },
  beforeUnmount() {
    console.log('=====>beforeUnmount')
  },
  unmounted() {
    console.log('=====>unmounted')
  }
}

const change = () => {
  color = colors.value[generateRandomNumber(0, 2) + 1]
}

</script>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 20px;
  padding-left: 20px;
}
</style>