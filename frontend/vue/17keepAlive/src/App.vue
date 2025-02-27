<script setup lang="ts">
import { ref } from 'vue';
// 额外的，keep-alive会使组件多上onDeactivated和onActivated两个生命周期

let flag = ref<boolean>(false)
const changeComponent = (): void => {
  flag.value = !flag.value
}
</script>

<template>
  <div class="hx-box">
    <!-- keep-alive的作用是帮助我们缓存组件，比如我们在登录界面输入了账号，但是忘记
    了密码，此时我们跳转到其他页面，那么回来时这个账号我们肯定是期望它还存在的，但是实
    际上并不会存在，这是我们就可以借助keep-alive实现这个小功能，但是注意，什么参数都
    没有的情况下只会缓存第一层组件，就像shallowRef那些东西一样 -->
    <div>
      <!-- 这个上面有一些属性，如:include="['inner']"表示缓存inner组件(默认include包括所有的)，同样的，:exclude表示不缓存某个组件 -->
      <keep-alive>
        <inner v-if="flag"></inner>
        <other v-else></other>
      </keep-alive>
    </div>

    <div>
      <button class="hx-box__change" @click="changeComponent">change</button>
    </div>
  </div>
</template>

<style lang="scss">
#app {
  @include bfc;
  padding-top: 3px;
  padding-left: 6px;
}

@include block(box) {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  height: 100%;

  @include element(change) {
    background-color: #007bff;
    border: none;
    color: white;
    padding: 10px 20px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    cursor: pointer;
    border-radius: 12px;
    transition-duration: 0.4s;
  }
}
</style>
