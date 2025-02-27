<script setup lang="ts">
import { ref } from 'vue'
import _ from 'lodash'

let list = ref(Array.apply(null, {length: 81} as number[]).map((_, index)=>{
  return {
    id: index,
    number: (index % 9) + 1
  }
}))

const handleClick = () => {
  // list.value = list.value.sort(() => Math.random() - 0.5)
  list.value = _.shuffle(list.value)
}

</script>

<template>
  <div>
    <button @click="handleClick">随机</button>

    <transition-group move-class="moveClass" class="hx-list" tag="div">
      <div class="hx-list__item" v-for="item in list" :key="item.id">
        {{ item.number }}
      </div>
    </transition-group>
  </div>
</template>

<style lang="scss">
#app{
  @include bfc;
  // 居中显示
  display: flex;
  justify-content: center;
  align-items: center;
}

.moveClass{
  transition: all 1s ease;
}

@include block(list){
  display: flex;
  flex-wrap: wrap;
  width: calc(25px * 9 + 9px);

  @include element(item){
    width: 25px;
    height: 25px;
    border: 1px solid #ccc;
    display: flex;
    justify-content: center;
    align-items: center;
  }
}
</style>
