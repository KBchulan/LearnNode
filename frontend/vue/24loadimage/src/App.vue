<template>
  <div>
    <div class="images">
      <img v-lazy="item" v-for="item in arr" width="360" height="500" alt="">
    </div>
  </div>
</template>

<script setup lang="ts">
import * as types from './types'
import type { Directive, DirectiveBinding } from 'vue'

// 这里说一下eager的作用，普通的glob是一个懒加载的，也就是会返回一个回调，需要我们调用才能实现图片的加载，
// 但是加上eager之后则会被直接改为一个module的模式，类似于直接在代码里import
let imageList = import.meta.glob('./assets/images/*.jpg', { eager: true })
let arr = Object.values(imageList).map(va => (va as types.ImageValue).default)    // 这里也可以使用Record约束原对象

// Directive两个泛型，1,当前的HTML元素类型，2.绑定值的类型
let vLazy: Directive<HTMLImageElement, string> = async (el: HTMLImageElement, bingings: DirectiveBinding<string>) => {
  const defaultImage = await import('./assets/vue.svg')

  el.src = defaultImage.default

  // 检测图片出现的程度
  const observer = new IntersectionObserver((entries) => {
    // 若出现在屏幕内则进行加载，即intersectionRatio > 0
    if (entries[0].intersectionRatio) {
      // 但是加载的效果比较快，不好看出来，我们加一个Timer
      setTimeout(() => {
        el.src = bingings.value
        observer.unobserve(el)
      }, 1000)

    }
  })

  observer.observe(el)
}

</script>

<style lang="less">
.images {
  display: flex;
  flex-direction: column;
  align-items: center;
}
</style>