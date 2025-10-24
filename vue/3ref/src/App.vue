<template>
  <div>
    {{ Huaixi }}<br>
    {{ RefHuaixi }}<br>
    {{ RefTypeHuaixi }}<br>
    {{ RefInterfaceHuaixi }}<br>
    {{ ShallowRefHuaixi }}<br>
    {{ triggerRefHuaixi }}<br>
    {{ myRefHuaixi }}
  </div>
  <!-- Ref也可以读Dom属性 -->
  <div ref="divRef">我是DOM</div>

  <hr>

  <button @click="changeName">changeName</button><br>
  <button @click="RefchangeName">RefchangeName</button><br>
  <button @click="ShallowRefchangeName">ShallowRefchangeName</button><br>
  <button @click="triggerRefchangeName">triggerRefchangeName</button><br>
  <button @click="myRefchangeName">myRefchangeName</button><br>
  <button @click="divRefchangeName">divRefchangeName</button><br>
</template>

<script setup lang="ts">
// 普通的的变量如下，并不是响应式的情况下，我们改变name的值，并不会触发视图的更新
const Huaixi = {
  name: 'huaixi'
}
const changeName = () => {
  Huaixi.name = 'huaixi2'
  console.log('Huaixi2')
}

// 这里说一下vue的逻辑，包含数据，视图，以及桥接，数据变化会触发视图的更新，视图的变化会触发数据的更新
// 所以，我们需要使用ref来创建一个响应式的变量
import { ref, isRef } from 'vue'
let count = -1

const RefHuaixi = ref({
  name: 'refhuaixi'
})
const RefchangeName = () => {
  count++
  if (count % 2 === 0) {
    RefHuaixi.value.name = 'refhuaixi2'
  } else {
    RefHuaixi.value.name = 'refhuaixi'
  }

  console.log(RefHuaixi)
  console.log(isRef(RefHuaixi))   // 判断是不是ref
}

// ref也可以接收一个泛型
type Hu = { name: string }
const RefTypeHuaixi = ref<Hu>({ name: 'RefTypehuaixi' })

// 当然类型比较复杂的情况下用interface也是ok的
import type { Ref } from 'vue'
const RefInterfaceHuaixi: Ref<Hu> = ref({ name: 'RefInterfacehauixi' })

// 接着说一下shallowRef，shallowRef是浅层的响应式，只会在当前层级进行响应式，不会在子级进行响应式
import { shallowRef } from 'vue'
const ShallowRefHuaixi = shallowRef({ name: 'ShallowRefhuaixi' })
const ShallowRefchangeName = () => {
  ShallowRefHuaixi.value.name = 'ShallowRefhuaixi2'      // 这样是不会改变视图的，只会触发数据的更新
  ShallowRefHuaixi.value = { name: 'ShallowRefhuaixi3' } // 这样是会改变视图的，也就是说同级视图会更新
  console.log(ShallowRefHuaixi)
}
// ref是深层次的响应式，会监听对象内部的变化，但是shallowRef是浅层次的响应式，只会监听本层次（.value）的变化
// 同时这两个不能混用，如果混用，shallowRef会失效（即改变ref的值，shallowRef也会改变视图）

// 这里说一下triggerRef，triggerRef是触发响应式，当改变ref的值时，会触发视图的更新
import { triggerRef } from 'vue'
const triggerRefHuaixi = shallowRef({ name: 'triggerRefhuaixi' })
const triggerRefchangeName = () => {
  triggerRefHuaixi.value.name = 'triggerRefhuaixi2'   // 我们在介绍shallowRef的时候说过，shallowRef此时的视图是不会更新的
  triggerRef(triggerRefHuaixi)                        // 这里我们使用triggerRef来触发视图的更新
  console.log(triggerRefHuaixi)
}

// 最后说一下customRef，customRef是自定义响应式，可以自定义响应式的方式
import { customRef } from 'vue'
const MyRef = <T>(value: T) => {
  let timer: any    // 实现一个防抖操作
  return customRef((track, trigger) => {
    return {
      get() {
        track()       // 收集依赖
        return value
      },
      set(newValue: T) {
        clearTimeout(timer)
        timer = setTimeout(() => {
          value = newValue
          trigger()           // 触发依赖
          console.log('set')
          timer = null
        }, 1000)
      }
    }
  })
}

const myRefHuaixi = MyRef('myRefhuaixi')
const myRefchangeName = () => {
  myRefHuaixi.value = 'myRefhuaixi2'
  console.log(myRefHuaixi)
}

const divRef = ref<HTMLDivElement>()
const divRefchangeName = () => {
  console.log(divRef.value?.innerText)
}

</script>

<style scoped lang="scss"></style>
