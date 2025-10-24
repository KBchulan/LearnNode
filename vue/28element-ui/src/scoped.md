***scoped 是vue中的一种css样式隔离机制，用于防止css样式污染。***

*主要作用：*

* 给HTML的DOM节点加一个不重复data属性(形如：data-v-123)来表示他的唯一性
* 在每句css选择器的末尾（编译后的生成的css语句）加一个当前组件的data属性选择器（如[data-v-123]）来私有化样式并命中当前组件的样式
* 如果组件内部包含有其他组件，只会给其他组件的最外层标签加上当前组件的data属性

*属性穿透*
  也就是说，在一个组件中，可以分为两种情况
- 一种是DOM，这种情况，vue会自动给DOM加上data-v-123属性，同时css选择器会命中这个DOM
- 另外一种是组件，由于组件只有最外层才会有data-v-123属性，所以css选择器指挥命中这个其他组件的最外层，而不会影响到其他组件内部的样式，所以我们需要加上类似于下面的代码，给css的标签选择器或类选择器或者id选择器加上deep()，这样就可以穿透到组件内部，影响到组件内部的样式

```css
:deep(.box){
  color: red;
}
```

*插槽选择器*
当我们在父组件去定义了子组件的class选择器，但是我们想要在子组件去定义这个class，我们就需要:slotted(){}

```css
:slotted(.box){
  color: red;
}
```

*全局样式*
我们可以通过:global(){}来穿透到全局，这样就可以影响到全局了
但是实际开发中，这样不好，我们可以在APP.vue里写一个<style></style>，用来存全局的，再写一个<style scoped></style>，用来存局部的，这样就可以避免全局样式污染了

```css
:global(.box){
  color: red;
}
```

*动态样式*
我们可以通过v-bind:class=""来绑定动态样式，这样就可以根据条件来动态的改变样式了

```vue
<template>
  <div class="box">
    这是一个BOX
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
const style = ref<string>('red')
</script>

<style scoped>
.box{
  color: v-bind(style);
}
</style>
```

