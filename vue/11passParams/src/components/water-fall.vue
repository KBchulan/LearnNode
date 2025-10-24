<script setup lang="ts">
// 通过defineProps这种方式接收父组件传的值

// 如果使用js
// const __js_props = defineProps({
//   name: {
//     type: String,
//     default: '默认'
//   }
// })

// 如果使用ts
// const __ts_props = defineProps<{
//   name: string,
//   arr: number[]
// }>()

// 比如父组件没有传sex: 'man'，我们需要引入withDefaults
const props = withDefaults(defineProps<{
  name: string,
  arr: number[]
  sex: string
}>(), {
  sex: () => 'man'
})

// 注意，代码层面是不能直接使用传递的值的，我们需要借助返回值来调用，模板里面是可以直接使用的
// console.log(name)   无法工作  

// 子组件给父组件传值
const emit = defineEmits(['on-click'])
// const emit = defineEmits<{
//   (e: 'on-click', prop: any[]): void
// }>()
const send = () => {
  emit('on-click', ['huaixi', 18, 'man'])
}

// 给父组件暴露一些方法或者属性
defineExpose({
  name: 'aaa',
  opne: () => console.log(1)
})

</script>

<template>
  <div class="hx-son">
    son components
    <div>
      父传子的值:{{ name }}--{{ arr }}--{{ props.sex }}
      <!-- js方式的返回值:{{ __js_props.name }} -->
      <!-- ts方式的返回值:{{ __ts_props.name }}--{{__ts_props.arr.filter(value => value % 2)}} -->
    </div>
    <button @click="send" class="hx-son__sendButton">给父组件传值</button>
    <hr>
  </div>
</template>

<style scoped lang="scss">
@include block(son) {
  color: cadetblue;

  @include element(sendButton) {
    border-radius: 4px;
    color: rgb(12, 162, 162);
    background-color: azure;
  }
}
</style>
