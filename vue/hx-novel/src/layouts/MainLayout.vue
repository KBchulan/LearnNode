<template>
  <el-container class="layout-container">
    <el-header class="header">
      <div class="logo">
        <span>轻小说阅读</span>
      </div>
      <el-menu mode="horizontal" router class="nav-menu">
        <el-menu-item index="/">首页</el-menu-item>
        <el-menu-item index="/novels">小说列表</el-menu-item>
      </el-menu>
      <div class="search">
        <el-input v-model="searchKeyword" placeholder="搜索小说" @keyup.enter="handleSearch">
          <template #suffix>
            <el-icon>
              <Search />
            </el-icon>
          </template>
        </el-input>
      </div>
    </el-header>

    <el-main class="main-content">
      <router-view />
    </el-main>

    <el-footer class="footer">
      <p>© 2024 轻小说阅读网站</p>
    </el-footer>
  </el-container>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { useRouter } from 'vue-router'
import { Search } from '@element-plus/icons-vue'
import { IMAGES } from '@/assets/images'

const router = useRouter()
const searchKeyword = ref('')

const handleSearch = () => {
  if (searchKeyword.value.trim()) {
    router.push({
      name: 'novel-list',
      query: { keyword: searchKeyword.value }
    })
  }
}
</script>

<style scoped lang="scss">
.layout-container {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  width: 100vw;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  background: url('@/assets/images/bg.jpg') no-repeat center center fixed;
  background-size: cover;
  margin: 0;
  padding: 0;

  .header {
    position: sticky;
    top: 0;
    z-index: 100;
    background: rgba(255, 255, 255, 0.9);
    display: flex;
    align-items: center;
    padding: 0 20px;
    height: 60px;
    width: 100%;
    box-sizing: border-box;

    .logo {
      font-size: 24px;
      font-weight: 300;
      margin-right: 40px;
    }

    .nav-menu {
      flex: 1;
      border: none;
      background: transparent;
    }

    .search {
      width: 300px;
    }
  }

  .main-content {
    flex: 1;
    padding: 20px;
    background: rgba(255, 255, 255, 0.8);
    min-height: calc(100vh - 120px);
    width: 100%;
    box-sizing: border-box;
    overflow-y: auto;
  }

  .footer {
    background: rgba(255, 255, 255, 0.9);
    text-align: center;
    padding: 20px;
    height: 60px;
    width: 100%;
    box-sizing: border-box;
  }
}
</style>