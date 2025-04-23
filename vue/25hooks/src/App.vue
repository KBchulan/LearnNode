<template>
  <!-- <ColorMode :num="num"></ColorMode> -->
  <A a="123" b="456" :num="num"></A>
</template>

<script setup lang="ts">
import { ref } from 'vue';

// hooks(钩子)：
// 1. 生命周期钩子，比如component，transition，Directive, keep-alive的生命周期
// 2. 在组合式API里，一些共有的代码逻辑可以封装为一个hook，被多个组件所公用

// 与v2的mixins比较像，但是mixins是有一些问题的，比如变量被覆盖，不利于阅读等原因
let num = ref<number>(123_456_789)
</script>

<style lang="less"></style>