<script setup lang="ts">
interface Tree {
  name: string,
  checked: boolean,
  children?: Tree[]
}

defineProps<{
  data?: Tree[]
}>()

defineOptions({   // 也可以通过这种方式去重命名文件
  name: 'huaixi'
})

const clickTag = (item: Tree): void => {
  console.log(item)
}

</script>

<template>
  <div @click.stop="clickTag(item)" class="hx-tree" v-for="item in data">    <!-- 这里的点击事件也是递归的，而且是从子到父，也对应我们递归里的`归`，其实还是因为一个事件的冒泡，加上去stop就好了 -->
    <input v-model="item.checked" type="checkbox"> <span>{{ item.name }}</span>
    <Tree v-if="item?.children?.length" :data="item?.children"></Tree> <!-- 我们可以通过这种方式递归，注意标签名应当与当前文件名称一致 -->
    <!-- <huaixi v-if="item?.children?.length" :data="item?.children"></huaixi> 也可以通过这种方式去重命名文件 -->
  </div>
</template>

<script lang="ts">
export default {
  name: 'huaixi'    // 也可以通过这种方式去重命名文件
}
</script>

<style scoped lang="scss">
@include block(tree) {
  padding-left: 20px;
  background-color: aqua;
  margin-left: 10px;
}
</style>
