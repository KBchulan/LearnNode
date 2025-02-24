<script setup lang="ts">
import { ref } from 'vue';

// 先整体认知一下slot：slot相当于子组件给父组件的一个占位符，父组件会将template里面的东西粘贴到子组件对应位置 
let name = ref<string>('header')
</script>

<template>
  <div>
    <Slot>
      <!-- 插槽也是可以简写的:#  --其中匿名插槽是#default -->
      <template v-slot:header> <!-- 具名插槽 -->
        <!-- v-slot:header ="{updata}",具名插槽也是像这样接收子组件传来的数据的，直接在v-slot后的任意位置写'={}'即可 -->
        <div>
          header
        </div>
      </template>

      <template v-slot="{ data, index }"> <!-- 匿名插槽，我们可以通过'='的方式将 -->
        <div>
          {{ data.name }}--{{ data.age }}--{{ index }}
        </div>
      </template>

      <template #footer> <!-- 具名插槽 -->
        <div>
          footer
        </div>
      </template>

      <template #[name]>  <!-- 我是动态插槽 -->
        <div>
          我是动态插槽
        </div>
      </template>
    </Slot>
  </div>
</template>

<style lang="scss">
#app {
  @include bfc;

  padding-top: 3px;
  padding-left: 6px;
}
</style>
