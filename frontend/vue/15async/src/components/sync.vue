<script setup lang="ts">
import { axios } from '../server/axios'

interface Data {
  data: {
    name: string,
    age: number,
    img: string,
    dec: string
  }
}

const { data } = await axios.get<Data>('./data.json')

</script>

<template>
  <div class="hx-sync">
    <!-- 头像及名称 -->
    <div class="hx-sync__content">
      <div><img :src="data.img"></div>
      <div class="hx-sync__pop">
        <div>{{ data.name }}</div>
        <div>&nbsp;{{ data.age }}</div>
      </div>
    </div>

    <hr>

    <!-- 评论的内容 -->
    <div>{{ data.dec }}</div>
  </div>
</template>

<style scoped lang="scss">
@include block(sync) {
  padding: 15px;
  margin: 10px 0;
  border-radius: 8px;

  @include element(content) {
    display: flex;
    align-items: flex-start;

    img {
      width: 40px;
      height: 40px;
      border-radius: 50%;
      margin-right: 12px;
      object-fit: cover;
    }
  }

  @include element(pop) {
    flex: 1;

    div:first-child {
      font-weight: bold;
      font-size: 14px;
      color: #333;
    }
  }

  hr {
    border: 1px solid #8c8484;
    margin: 3px 0;
    margin-bottom: 2px;
  }

  // 评论内容
  div:last-child {
    font-size: 14px;
    color: #666;
    line-height: 1.6;
    word-wrap: break-word;
  }
}
</style>