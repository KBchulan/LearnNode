<script setup lang="ts">
const arr: string[] = ['a', 'b', 'c', 'd']
arr.splice(2, 0, 'DDD')

</script>

<template>
  <div>
    <!-- 这里说一下，vue 的 diff 算法 -->
    <!-- no key，vue 会认为所有节点都变了，所以会重新渲染所有节点，
     比如我原来只有 a b c d，现在我插入一个 DDD，vue 会认为我原来有 a b c d，
     现在我变成了 a b DDD c d，所以会重新渲染所有节点，导致性能下降，
     
    无key：替换 新增 删除
    区别是是无key patch会重新渲染所有节点，有key patch会根据key来渲染节点 -->
    <div v-for="item in arr">
      {{ item }}
    </div>

    <!-- with key
     1.前序对比算法
     2.尾序对比算法
     3.对比完后发现新的Vnode树有多的就会进行新增
     4.若是发现新的Vnode树有少的就会进行卸载
     5.乱序的情况下：会有一个最长递增子序列算法，然后进行一个移动 -->
    <div :key="item" v-for="item in arr">
      {{ item }}
    </div>

  </div>

</template>

<style scoped lang="scss"></style>
