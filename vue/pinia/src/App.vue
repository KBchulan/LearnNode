<template>
  <div class="hx-test">
    <span>{{ test.name }}</span>
    --
    <span>{{ test.current }}</span>
    <button @click="change">changed</button>
  </div>

  <hr>

  <div class="hx-actions">
    <span>{{ test.user.name }}</span><br>
    <span>{{ test.user.age }}</span><br>
    <span>getter:{{ test.nameChanged }}</span>
    <br>
    <button @click="callbackFunc">Action调用</button>
  </div>

  <hr>

  <div class="hx-long-live-data">
    {{ base.baseData }}<br>
    持久化的数据: {{ test.longLiveData }}<br>
    <button @click="test.setLongLiveData">设置持久化数据</button>
  </div>
</template>

<script setup lang="ts">
import { useTestStore, useBaseStore } from './store/index'
import { storeToRefs } from 'pinia'

const test = useTestStore() // it will changed to a reactive object
const base = useBaseStore()

const change = () => {
  // 我们可以通过5种方式修改state
  // 1. actions ===> store/index.ts
  // test.changedName() func1

  // 2. 直接修改
  // test.name = 'func2'

  // 3. 通过$patch修改，这种方式是一个对象，因此只有属性修改的操作
  /**
   * test.$patch({
   *   name: 'func3'
   * })
  */

  // 4. 通过$patch修改，这种方式是一个函数，因此可以修改多个属性, 同时还可以进行一些逻辑判断
  // test.$patch((state) => {
  //   if (state.current < 6) {
  //     state.current++
  //   }
  //   state.name = 'func4'
  // })

  // 5. 直接通过$state修改，可以直接指定某个属性进行修改
  test.$state.name = 'func5'
  test.$state.current = 10
}

const callbackFunc = () => {
  // test.setUser()  这里是一个同步的调用
  test.asyncsetUser()
}

// 在vue/5to/src/App.vue中，我们讲了toRef，toRefs，toRaw，toValue，toRefValue，toRawValue，toValueValue，toRawValueValue
// 对reactive，我们会使用toRefs进行解构
const { name, current } = storeToRefs(test)
console.log(name, current)

// 最后介绍一下pinia的API
const intro = () => {
  test.$reset()     // 可以帮助我们重置state的状态

  test.$subscribe((args, state) => {
    console.log(args)   // 这个是修改的属性，包含修改的属性名和修改的值以及old值
    console.log(state)  // 这个是修改后的state
  })   // state有任何属性发生变化就会走这个函数，和vue的watch基本一致

  // 监听action的调用
  test.$onAction(({ name, store, args, after, onError }) => {
    console.log(name, store, args, after, onError)
  })
}

intro()

</script>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 20px;
  padding-left: 20px;
}

@include block(test) {
  width: 100px;
  height: 100px;
  color: red;
}

@include block(actions) {
  font-size: 24px;
  font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
  color: white;
}

@include block(long-live-data) {
  font-size: 24px;
  font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
  color: white;
}
</style>