<template>
  <div class="hx-btns">
    <button v-has-show="'shop:create'" @click="create" class="hx-btns__create">创建</button>

    <button v-has-show="'shop:edit'" @click="edit" class="hx-btns__edit">编辑</button>

    <button v-has-show="'shop:delete'" @click="deleted" class="hx-btns__deleted">删除</button>
  </div>
</template>

<script setup lang="ts">
import type { Directive, DirectiveBinding } from 'vue'
const create = () => {
  console.log('创建')
}

const edit = () => {
  console.log('编辑')
}
const deleted = () => {
  console.log('删除')
}
// 这里是后台的数据
localStorage.setItem('userID', 'huaixi')

const permissions = [
  'huaixi:shop:create',
  'huaixi:shop:edit',
  'huaixi:shop:delete',
]

// 通过这种方式我们可以实现一个功能，即后台用户返回权限数据，校验其数据并根据权限开放不同的
// 按钮，换到实际里就是更换一个界面或者展示不同的功能
// 注意，这个写法是简写的，此函数是mounted和updated的两个钩子
const vHasShow: Directive = (el: HTMLElement, binding: DirectiveBinding) => {
  const userID = localStorage.getItem('userID')!
  if (!permissions.includes(userID + ':' + binding.value)) {
    el.style.display = 'none'
  }
}

</script>

<style scoped lang="scss">
@include block(btns) {
  width: 400px;
  height: 400px;
  border: 1px solid #ccc;

  display: flex;
  flex-direction: column;
  padding: 50px;

  @include element(create) {
    margin-bottom: 50px;
  }

  @include element(edit) {
    margin-bottom: 50px;
  }

  @include element(delete) {}
}
</style>