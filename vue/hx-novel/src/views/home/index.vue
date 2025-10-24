<template>
  <div class="home-container">
    <div class="banner">
      <h1>欢迎来到轻小说阅读</h1>
      <p>发现你最爱的轻小说</p>
    </div>

    <div class="content-section">
      <div class="main-content">
        <div class="section-header">
          <h2>最新更新</h2>
          <el-link type="primary" @click="goToNovelList">查看更多 <el-icon>
              <ArrowRight />
            </el-icon></el-link>
        </div>

        <el-row :gutter="20">
          <el-col v-for="novel in latestNovels" :key="novel.id" :span="6">
            <el-card :body-style="{ padding: '0px' }" class="novel-card" @click="goToNovel(novel.id)">
              <div class="novel-cover">
                <img :src="novel.cover || IMAGES.DEFAULT_COVER" />
                <div class="novel-status">
                  <el-tag size="small" :type="novel.status === 'completed' ? 'success' : 'warning'">
                    {{ novel.status === 'completed' ? '已完结' : '连载中' }}
                  </el-tag>
                </div>
              </div>
              <div class="novel-info">
                <h3>{{ novel.title }}</h3>
                <p class="author">{{ novel.author }}</p>
                <p class="description">{{ novel.description }}</p>
              </div>
            </el-card>
          </el-col>
        </el-row>
      </div>

      <div class="side-section">
        <div class="categories-section">
          <h2>小说分类</h2>
          <div class="category-list">
            <el-button v-for="category in categories" :key="category" class="category-item" text
              @click="goToCategory(category)">
              {{ category }}
            </el-button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useNovelStore } from '@/stores/novel'
import { ArrowRight } from '@element-plus/icons-vue'
import type { Novel } from '@/types/novel'
import { IMAGES } from '@/assets/images'

const router = useRouter()
const novelStore = useNovelStore()
const latestNovels = ref<Novel[]>([])
const categories = ['奇幻', '异世界', '冒险', '校园', '恋爱', '日常', '科幻', '游戏']

onMounted(async () => {
  const response = await novelStore.fetchNovelList(1)
  console.log('Mock response:', response)
  if (response?.data?.items) {
    latestNovels.value = response.data.items.slice(0, 4)
  }
})

const goToNovel = (id: string) => {
  router.push({ name: 'novel-detail', params: { id } })
}

const goToNovelList = () => {
  router.push({ name: 'novel-list' })
}

const goToCategory = (category: string) => {
  router.push({
    name: 'novel-list',
    query: { category }
  })
}
</script>

<style scoped lang="scss">
.home-container {
  .banner {
    text-align: center;
    padding: 40px 0;
    background: rgba(255, 255, 255, 0.85);
    border-radius: 8px;
    margin-bottom: 20px;
    box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);

    h1 {
      font-size: 2.5em;
      margin-bottom: 15px;
      color: #333;
      font-weight: 500;
    }

    p {
      font-size: 1.2em;
      color: #666;
    }
  }

  .content-section {
    display: flex;
    gap: 20px;

    .main-content {
      flex: 1;
      background: rgba(255, 255, 255, 0.85);
      border-radius: 8px;
      padding: 20px;
      box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
    }

    .side-section {
      width: 240px;

      .categories-section {
        background: rgba(255, 255, 255, 0.85);
        border-radius: 8px;
        padding: 20px;
        box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);

        h2 {
          margin: 0 0 15px 0;
          font-size: 1.2em;
          color: #333;
          font-weight: 500;
        }

        .category-list {
          display: flex;
          flex-direction: column;
          gap: 10px;

          .category-item {
            text-align: left;
            font-size: 1em;
            color: #666;
            padding: 8px;
            border-radius: 4px;

            &:hover {
              color: var(--el-color-primary);
              background: var(--el-color-primary-light-9);
            }
          }
        }
      }
    }
  }

  .section-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;

    h2 {
      margin: 0;
      font-size: 1.3em;
      color: #333;
      font-weight: 500;
    }
  }

  .novel-card {
    cursor: pointer;
    transition: all 0.3s;
    margin-bottom: 20px;
    border: none;
    box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);

    &:hover {
      transform: translateY(-5px);
      box-shadow: 0 4px 16px rgba(0, 0, 0, 0.15);
    }

    .novel-cover {
      position: relative;
      height: 220px;

      img {
        width: 100%;
        height: 100%;
        object-fit: cover;
        border-radius: 4px 4px 0 0;
      }

      .novel-status {
        position: absolute;
        top: 10px;
        right: 10px;
      }
    }

    .novel-info {
      padding: 12px;
      background: #fff;
      border-radius: 0 0 4px 4px;

      h3 {
        margin: 0 0 8px;
        font-size: 1.1em;
        white-space: nowrap;
        overflow: hidden;
        text-overflow: ellipsis;
        color: #333;
      }

      .author {
        color: #666;
        margin-bottom: 8px;
        font-size: 0.9em;
      }

      .description {
        color: #999;
        font-size: 0.9em;
        height: 38px;
        overflow: hidden;
        text-overflow: ellipsis;
        display: -webkit-box;
        -webkit-line-clamp: 2;
        -webkit-box-orient: vertical;
        line-height: 1.4;
      }
    }
  }
}
</style>