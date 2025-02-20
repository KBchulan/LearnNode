<script setup lang="ts">
import { shallowReactive } from 'vue'

let obj = shallowReactive({
  foo: {
    bbb: {
      num: 1
    }
  }
})

const edit = () => {
  obj.foo = { bbb: { num: 3 } }
  console.log(obj)
}
</script>

<template>
  <div>
    {{ obj }}
    <button @click="edit">修改</button>
  </div>
</template>

<style scoped></style>
