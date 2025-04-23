<template>
  <h1>父组件</h1>
  <div v-for="item in prop">
    {{ item }}
  </div>
  <hr>
  <child @on_son_click="logProps" :name="[name]"></child>
</template>

<script setup lang="ts">
import { ref } from 'vue'
const name = ref<string>('父组件--name')
const prop = ref<string[]>()

const logProps = (props: string[]) => {
  prop.value = props
  props.forEach(v => {
    console.log('父=====>' + v)
  })
}
</script>

<style></style>