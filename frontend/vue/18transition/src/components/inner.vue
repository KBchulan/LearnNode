<script setup lang="ts">
import { ref } from 'vue'
import 'animate.css'

const flag = ref<boolean>(true)
let value = ref<number>(0)
</script>

<template>
  <div class="hx-content">
    <button @click="flag = !flag">change</button>
    <!-- 这里做一个方块的盒子，首先transition组件我们可以通过这种方式指定class的类名前缀，
     这样就需要写六种class，分别是enter-from，enter-active，enter-to，leave-from，leave-active，leave-to
     当然我们也可以自己指定这六种状态的名称 -->
    <Transition enter-from-class="box-enter-from" enter-active-class="box-enter-active" enter-to-class="box-enter-to" name="first">
      <div v-if="flag" class="hx-content__box"></div>
    </Transition>

    <!-- 这里我们再写一个块，用animate.css来实现 -->
    <Transition enter-active-class="animate__animated animate__bounce" leave-active-class="animate__animated animate__flash">
      <div v-if="flag" class="hx-content__boxelse"></div>
    </Transition>

    <!-- 生命周期的钩子 -->
    <Transition
    @before-enter=""
    @enter=""
    @after-enter=""
    @enter-cancelled=""

    @before-leave=""
    @leave=""
    @after-leave=""
    @leave-cancelled=""
    >
    </Transition>

    <!-- appear是进入时执行 -->
    <Transition
    appear
    @before-appear=""
    @appear=""
    @after-appear="">
    </Transition>

    <!-- 这里则是一个滑块 -->
    <div class="hx-content__slider-demo-block">
      <el-slider v-model="value" show-input />
    </div>
  </div>
</template>

<style scoped lang="scss">
@include block(content) {
  display: flex;
  flex-direction: column;

  .box-enter-from,
  .first-leave-to {
    opacity: 0;
    transform: scale(0.5);
  }

  .box-enter-active,
  .first-leave-active {
    transition: all 2s ease;
  }

  .box-enter-to,
  .first-leave-from {
    opacity: 1;
    transform: scale(1);
  }

  @include element(box) {
    width: 200px;
    height: 200px;
    background-color: skyblue;
  }

  @include element(boxelse){
    width: 200px;
    height: 200px;
    background-color: deepskyblue;
  }
  @include element(slider-demo-block) {
    max-width: 600px;
    display: flex;
    align-items: center;

    .el-slider {
      margin-top: 0;
      margin-left: 12px;
    }
  }

}
</style>