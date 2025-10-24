<template>
  <div v-show="modelValue" class="dialog">
    <div class="dialog-header">
      <h2>我是v-model子组件 dialog</h2>
      <button class="close-btn" @click="emit('update:modelValue', false)">关闭</button>
    </div>
    <div class="dialog-content">
      <label>内容：
        <input @input="change" type="text" :value="text"/>
      </label>
    </div>
  </div>
</template>

<script setup lang="ts">
defineProps<{
  modelValue: boolean
  text: string
}>()

const emit = defineEmits(['update:modelValue', 'update:text'])
const change = (e: Event)=>{
  const target = e.target as HTMLInputElement
  emit('update:text', target.value)
}
</script>

<style scoped lang="less">
.dialog {
  width: 90%;
  padding: 20px;
  border: 1px solid #ddd;
  border-radius: 4px;

  .dialog-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;

    h2 {
      margin: 0;
      font-size: 18px;
    }

    .close-btn {
      padding: 5px 10px;
      border: 1px solid #ddd;
      background-color: #fff;
      border-radius: 4px;
      cursor: pointer;
    }
  }

  .dialog-content {
    input {
      width: 250px;
      padding: 8px;
      border: 1px solid #ddd;
      border-radius: 4px;
    }
  }
}
</style>