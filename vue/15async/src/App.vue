<script setup lang="ts">
import { defineAsyncComponent } from 'vue'
const Sync = defineAsyncComponent(() => import('./components/sync.vue'))

// in this section, we will introduce the asyncComponent
//    first, we should define a syncComponent(we can use await in the target component) and use defineAsyncComponent to 
// login the sync component in the global
//    second, we can use Suspence in template, and push two slot in this place, the one must #default to slot the sync
// component and the other shoule use #fallback to display the skeleton component
//    lastly, the sync component will be push a other .js in build, it will make a great prove

</script>

<template>
  <Suspense> <!-- 异步组件必须这样展示 -->
    <template #default>
      <Sync></Sync>
    </template>

    <template #fallback>
      <Skeleton></Skeleton>
    </template>
  </Suspense>
</template>

<style lang="scss">
#app {
  @include bfc;
  padding-left: 6px;
  padding-top: 3px;
}
</style>
