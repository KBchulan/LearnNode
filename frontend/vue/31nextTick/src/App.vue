<template>
  <div class="wraps" ref="box">
    <div class="item" v-for="item in chatList">
      {{ item.name }} : {{ item.msg }}
    </div>
  </div>

  <div class="ipt">
    <div>
      <textarea type="text" v-model="ipt"></textarea>
    </div>
    <div>
      <button @click="send">发送</button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive, nextTick } from 'vue'

let chatList = reactive([
  { name: '张三', msg: 'Hello, World!' }
])

let box = ref<HTMLDivElement>()
let ipt = ref<string>('')

// 当我们发现dom获取到的是上一次的数据时，就要加上nextTick
// 本质原因是Vue更新数据是同步，但是更新dom是异步
const send = async () => {
  chatList.push({
    name: 'xxx',
    msg: ipt.value
  })
  await nextTick()
  box.value!.scrollTop = 9999999
}

</script>

<style scoped lang="less">
.wraps {
  margin: 2px auto;
  width: 500px;
  height: 400px;
  overflow: auto;
  overflow-x: hidden;
  background: #fff;
  border: 1px solid #ccc;

  .item {
    width: 100%;
    height: 50px;
    background: #ccc;
    display: flex;
    align-items: center;
    padding: 0 10px;
    border-bottom: 1px solid #fff;
  }

}

.ipt {
  margin: 10px auto;
  width: 500px;
  height: 100px;
  background: #fff;
  border: 1px solid #ccc;

  textarea {
    width: 100%;
    height: 100%;
    border: none;
    outline: none;
  }

  button {
    width: 100px;
    margin: 10px 0;
    float: right;
  }
}
</style>