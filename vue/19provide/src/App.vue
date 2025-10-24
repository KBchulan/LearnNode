<script setup lang="ts">
// 我们前面有使用过defineProps来实现父子组件传值
// 但是defineProps只能实现单向传值，父组件传值给子组件
// 如果子组件想要传值给父组件，就需要使用emit
// 但是emit需要手动去定义，如果组件很深，就会很麻烦
// 所以我们可以使用provide和inject来实现跨级组件传值

// 本节的核心就是介绍；依赖注入provide和inject
import { ref, provide } from 'vue'
let colorValue = ref<string>('red')

provide('color', colorValue)
</script>

<template>
  <div class="hx-grand">
    <h1>APP.vue (爷爷组件)</h1>
    <label>
      <input v-model="colorValue" value="red" name="color" type="radio">
      红色
    </label>

    <label>
      <input v-model="colorValue" value="blue" name="color" type="radio">
      蓝色
    </label>

    <label>
      <input v-model="colorValue" value="green" name="color" type="radio">
      绿色
    </label>

    <div class="hx-grand__box"></div>
  </div>
  <father></father>
</template>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 20px;
  padding-left: 20px;
}

@include block(grand) {
  color: #fff;

  label {
    display: inline-block;
    margin-right: 20px;
  }

  @include element(box){
    width: 150px;
    height: 150px;
    border: 1px solid #ccc;
    background-color: v-bind(colorValue);
  }
}
</style>
