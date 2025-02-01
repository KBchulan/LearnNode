<template>
  <!-- 写一些标签一类的 -->

  <!-- 1.vue模板：模板可以支持运算一类的，也可以支持API调用 -->
  <p>
    {{ sum() ? 'true' : 'false' }}<br>
    {{ arr1.map(value => ({ num: value })) }}
  </p>

  <!-- 2.vue指令 -->
  <span v-text="str1"></span> <!-- 可以解析文本 -->
  <strong v-html="str2"></strong> <!-- 可以解析html标签 -->

  <!-- v-if可以解析布尔值，为true显示true，为false会被更改为注释节点 -->
  <em v-if="bool1">{{ bool1 }}</em>
  <em v-if="bool2">{{ bool2 }}</em>

  <!-- v-show同样也是解析布尔值，但与v-if不同的是，v-show在false时会把css的display属性设置为none，并不是注释 -->
  <div v-show="bool1">{{ bool1 }}</div>
  <div v-show="bool2">{{ bool2 }}</div>

  <!-- 那相对应的，肯定也存在v-else, v-else-if, v-else-if-else -->
  <div v-if="str3 === 'A'">A</div>
  <div v-else-if="str3 === 'B'">B</div>
  <div v-else>C</div>

  <!-- 这里说一下v-on，可以绑定事件 -->
  <button v-on:click="func1">这是一个按钮</button> <!-- 这个是v-on版 -->
  <button @click="func1">这是一个按钮(@版)</button> <!-- 这个是@版，比较常用 -->
  <button @[event1]="func1">这是一个按钮(@[]版)</button> <!-- 这个是@[]版,也就是我们的动态事件 -->

  <!-- 这里写一下v-bind -->
  <div v-bind:id="id">这是一个v-bind演示</div>
  <div :id="id">这是一个v-bind演示(简写版本)</div>
  <div :style="style">这是一个v-bind演示(style)</div>
  <div :class="[bool3 ? 'a' : 'b']">这是一个v-bind演示(class)</div> <!-- 可以同时支持一个动态的，一个静态的 -->

  <!-- 这里写一下v-model,用于实现双向数据绑定,即让数据和视图同步 -->
  <div>
    <input v-model="name"><br> <!-- 文本框 -->

    <textarea v-model="textarea"></textarea><br> <!-- 多行文本 -->

    <input type="checkbox" v-model="checked"><br> <!-- 复选框 -->

    <input type="radio" v-model="radio" value="1"> <!-- 单选按钮 -->
    <input type="radio" v-model="radio" value="2"><br>

    <select v-model="selected"> <!-- 选择框 -->
      <option value="1">选项1</option>
      <option value="2">选项2</option>
    </select>
  </div>

  <!-- 这里写一下v-for，只是遍历，所有有那种二维啊，对象都是可以的 -->
  <div v-for="(item, index) in arr2" :key="index">
    {{ index }} - {{ item }}
  </div>

  <!-- 这里写一下v-once，用于只渲染一次的场景 -->
  <button v-once>
    {{ num1 }}
  </button>

  <!-- 这里写一下v-memo，用于缓存，但是只缓存一次 -->
  <div v-memo="[count]">
    <!-- 只有当 count 改变时，这里的内容才会重新渲染 -->
    <p>{{ count }}</p>
  </div>

</template>

<script setup lang="ts">
// 写一些js/ts代码

// module 1
const a: number = 1
const b: number = 2
const sum = () => {
  return a + b
}

const arr1: number[] = [1, 2, 3]

// module 2
const str1: string = 'this is a string'
const str2: string = '<section style="color: green">this is a section</section>'

const bool1: boolean = true
const bool2: boolean = false

const str3: string = 'B'

const func1 = () => {
  console.log('点击了')
}
const event1 = 'click'

const id: string = '123'
const bool3: boolean = false
const style = {
  color: 'red',
  fontSize: '20px',
  backgroundColor: 'rgba(210, 210, 210, 0.5)'
}

import { ref } from 'vue'
let name = ref('huaixi')
let textarea = ref('')
let checked = ref(false)
let radio = ref('')
let selected = ref('')

const arr2: string[] = ['怀玺', '伊芙', '艾尔莎']

let num1 = ref(1)
let count = ref(0)

</script>

<style scope>
/* 本位置写一些样式一类的 */
.a {
  color: red;
}

.b {
  size: 20px;
  color: blue;
}
</style>
