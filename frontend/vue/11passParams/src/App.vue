<script setup lang="ts">
import { ref } from 'vue'
import waterFall from './components/water-fall.vue'

let name: string = 'huaixi'

// 从difineExpose获取到方法或属性
const son = ref<InstanceType<typeof waterFall>>()

const getPropsFromSonComponents = (pros: any[]) => {
  pros.forEach(value => console.log(value))
  console.log(son.value?.name)
  son.value?.opne()
}
</script>

<template>
  <div class="hx-father">
    father components
  </div>
  <hr>
  <waterFall ref="son" @on-click="getPropsFromSonComponents" :name="name" :arr="[1, 2, 3, 4, 5]" :sex="'man'">
  </waterFall>
  <!-- 我们可以通过v-bind为这个组件绑定一个属性，名字随便起 -->
</template>

<style lang="scss">
#app {
  @include bfc;
}

@include block(father) {
  color: cadetblue;
}
</style>
