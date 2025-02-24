<script setup lang="ts">
import { reactive } from 'vue';

interface Name {
  name: string
  age: number
}

const names = reactive<Name[]>([
  {
    name: '张三',
    age: 113
  },
  {
    name: '张四',
    age: 114
  },
  {
    name: '张五',
    age: 115
  },
  {
    name: '张六',
    age: 116
  },
])

</script>

<template>
  <div class="hx-comain">
    <header class="hx-comain__header">
      <slot name="header"></slot> <!-- 具名插槽 -->
      <!-- :updata="'it just a test for name header'" -->
    </header>

    <main class="hx-comain__main"> <!-- 匿名插槽 -->
      <div v-for="(item, index) in names">
        <slot :index="index" :data="item"></slot>
      </div>
    </main>

    <footer class="hx-comain__footer">
      <slot name="footer"></slot> <!-- 具名插槽 -->
    </footer>
  </div>
</template>

<style scoped lang="scss">
@include block(comain) {
  width: 300px;
  height: 240px;

  border: 1px solid #ccc;
  color: black;
  font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;

  @include element(header) {
    height: 80px;
    display: grid;
    place-items: center;
    background-color: skyblue;
  }

  @include element(main) {
    height: 80px;
    display: grid;
    font-size: 10px;
    place-items: center;
    border-top: 1px solid #ccc;
    background-color: lightblue;
  }

  @include element(footer) {
    height: 80px;
    display: grid;
    place-items: center;
    border-top: 1px solid #ccc;
    background-color: deepskyblue;
  }
}
</style>
