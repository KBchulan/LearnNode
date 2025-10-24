<script setup lang="ts">
import { toRef, reactive } from 'vue'
// toRef的作用是可以让我们将一个[响应式对象]的值单独提出来，并对他作修改(注意，如果是非响应式对象，是没有用的)
type Main = { name: string, age: number, like: string }
const Xi = reactive<Main>({
  name: 'huaixi',
  age: 19,
  like: 'calate'
})

const property = toRef(Xi, 'like')

const change = () => {
  property.value = 'fib'    // 此处我们直接修改property，对应的原响应式对象也会随之发生改变
  console.log(Xi)
}

// toRefs更简单，只是对一个对象的每一个元素都进行了一次toRef,源码实现如下
// const toRefs = <T extends object>(object: T) => {
//   let map: any = {}
//   for (let key in object) {
//     map[key] = toRef(object, key)
//   }
//   return map
// }
import { toRefs } from 'vue'
// 场景就是解构reactive
const { name, age, like } = toRefs(Xi)

import { toRaw } from 'vue'
// 作用就是把一个响应式对象变为原始的非响应式对象
const toRawButton = () => {
  console.log(Xi, toRaw(Xi))
}


</script>

<template>
  <div>
    <div>
      {{ Xi }}
    </div>
    <hr>
    <div>
      {{ property }}
    </div>
    <hr>
    <div>
      {{ name }}--{{ age }}--{{ like }}
    </div>
    <hr>
    <div>
      <button @click="change">change</button>
      <button @click="toRawButton">Raw</button>
    </div>
  </div>
</template>

<style scoped></style>
