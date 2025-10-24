<script setup lang="ts">
import { onMounted, reactive } from 'vue';

const props = defineProps<{
  list: any[]
}>()

const waterList = reactive<any[]>([])
const heightList: number[] = []

const init = () => {
  const width = 130
  const x = document.body.clientWidth
  const coloms = Math.floor(x / width)

  for (let i = 0; i < props.list.length; i++) {
    if (i < coloms) {
      props.list[i].left = i * width
      props.list[i].top = 20
      waterList.push(props.list[i])
      heightList.push(props.list[i].height)
    }
  }
}

onMounted(() => {
  init()
})

</script>

<template>
  <div class="hx-wraps">
    <div class="hx-wraps__items" v-for="item in waterList"
      :style="{ height: item.height + 'px', background: item.background, left: item.left + 'px', top: item.top + 'px' }">
    </div>
  </div>
</template>

<style scoped lang="scss">
@include block(wraps) {
  position: relative;

  @include element(items) {
    position: absolute;
    width: 120px;
  }
}
</style>
