<script setup lang="ts">
</script>

<template>
  <div class="hx-sync">
    <div class="hx-sync__content">
      <div><img></div>
      <div class="hx-sync__pop">
        <div class="hx-sync__pop--head"></div>
        <div class="hx-sync__pop--age">&nbsp;</div>
      </div>
    </div>

    <hr>

    <div class="hx-sync__last"></div>
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
      background-color: #666;
    }
  }

  @include element(pop) {
    flex: 1;

    @include modifier(head) {
      font-weight: bold;
      font-size: 14px;
      color: #333;
      background-color: #666;
    }

    @include modifier(age){
      width: 400px;
      background-color: #666;
    }
  }

  hr {
    border: 1px solid #8c8484;
    margin: 3px 0;
    margin-bottom: 2px;
  }

  @include element(last) {
    min-width: 400px;
    min-height: 200px;
    background-color: #666;
  }
}
</style>