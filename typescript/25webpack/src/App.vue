<template>
    <div>{{ a }}</div>
    <button @click="a++">+</button>
    <div> {{ moment().format('YYYY-MM-DD HH:mm:ss') }} </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import moment from 'moment'
const a = ref<number>(1)
</script>

<style lang="less">
@red: red;

div {
    color: @red;
}
</style>