<template>
  <p class="text-center demonstration">card vertical layout</p>
  <el-carousel height="400px" direction="vertical" type="card" :autoplay="false">
    <el-carousel-item v-for="item in 4" :key="item">
      <h3 text="2xl" justify="center">{{ item }}</h3>
    </el-carousel-item>
  </el-carousel>
  <el-input v-model="text" style="width: 240px ;padding-top: 6px" maxlength="10" placeholder="Please input"
    show-word-limit type="text" />
</template>

<script setup lang="ts">
import { ref } from 'vue'

const text = ref('')

</script>

<style scoped>
.el-carousel__item h3 {
  color: #475669;
  opacity: 0.75;
  line-height: 200px;
  margin: 0;
  text-align: center;
}

.el-carousel__item:nth-child(2n) {
  background-color: #99a9bf;
}

.el-carousel__item:nth-child(2n + 1) {
  background-color: #d3dce6;
}
</style>