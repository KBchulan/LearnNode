<script setup lang="ts">
// 动态组件就是让多个组件使用同一个挂载点，并且可以动态切换
import { reactive, ref, markRaw, shallowRef } from 'vue';
import A from './components/A.vue'
import B from './components/B.vue'
import C from './components/C.vue'

const comID = shallowRef(A)     // 用于component内置标签
const activeID = ref(0)         // current id

const data = reactive([
  {
    name: 'A组件',
    com: markRaw(A)
  },
  {
    name: 'B组件',
    com: markRaw(B)
  },
  {
    name: 'C组件',
    com: markRaw(C)
  }
])

const changeCom = (item: any, index: any) => {
  activeID.value = index  
  comID.value = item.com  // 但是此时会有比较严重的性能浪费，因为ref对象会被劫持，但是组件的具体信息是没有必要的，因此可以调优(通过markRaw（多了一个__skip__属性）和shallowRef)
  console.log(item.com)
}

</script>

<template>
  <div style="display: flex;">
    <div @click="changeCom(item, index)" :class="[activeID == index ? 'hx-active' : '']" class="hx-com" v-for="(item, index) in data">
      <div>
        {{ item.name }}
      </div>
    </div>
  </div>
  <component :is="comID"></component>   <!-- 本节的动态组件 -->
</template>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 3px;
  padding-left: 6px;
}

@include block(active) {
  background-color: skyblue;
}

@include block(com) {
  border: 1px solid #ccc;
  padding: 5px 10px;
  margin: 5px;
  cursor: pointer;
}
</style>
