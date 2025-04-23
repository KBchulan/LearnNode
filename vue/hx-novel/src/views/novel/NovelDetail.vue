<template>
    <div class="novel-detail">
        <el-row :gutter="20">
            <el-col :span="6">
                <img :src="novel?.cover || IMAGES.DEFAULT_COVER" class="cover-image" />
            </el-col>
            <el-col :span="18">
                <div class="info-section">
                    <h1>{{ novel?.title }}</h1>
                    <div class="meta-info">
                        <span class="author">作者：{{ novel?.author }}</span>
                        <el-tag size="small" :type="novel?.status === 'completed' ? 'success' : 'warning'"
                            class="status-tag">
                            {{ novel?.status === 'completed' ? '已完结' : '连载中' }}
                        </el-tag>
                    </div>
                    <div class="categories">
                        <el-tag v-for="category in novel?.categories" :key="category" size="small" effect="plain"
                            class="category-tag">
                            {{ category }}
                        </el-tag>
                    </div>
                    <p class="description">{{ novel?.description }}</p>
                    <div class="action-buttons">
                        <el-button type="primary" @click="startReading">开始阅读</el-button>
                        <el-button :type="isInBookshelf ? 'success' : 'default'" @click="toggleBookshelf">
                            {{ isInBookshelf ? '已收藏' : '加入书架' }}
                        </el-button>
                    </div>
                </div>
            </el-col>
        </el-row>

        <div class="chapters-section">
            <h2>章节列表</h2>
            <el-row :gutter="20">
                <el-col v-for="chapter in novel?.chapters" :key="chapter.id" :span="8">
                    <div class="chapter-item" @click="goToChapter(chapter.id)">
                        <span class="chapter-title">{{ chapter.title }}</span>
                        <span v-if="isChapterRead(chapter.id)" class="read-status">已读</span>
                    </div>
                </el-col>
            </el-row>
        </div>
    </div>
</template>

<script setup lang="ts">
import { ref, onMounted, computed } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { useNovelStore } from '@/stores/novel'
import { IMAGES } from '@/assets/images'
import { useLocalStorage } from '@vueuse/core'

const route = useRoute()
const router = useRouter()
const novelStore = useNovelStore()
const novel = computed(() => novelStore.currentNovel)

// 书架功能
const bookshelf = useLocalStorage<string[]>('bookshelf', [])
const isInBookshelf = computed(() => bookshelf.value.includes(route.params.id as string))

// 阅读记录
const readChapters = useLocalStorage<Record<string, boolean>>('readChapters', {})

const toggleBookshelf = () => {
    const novelId = route.params.id as string
    if (isInBookshelf.value) {
        bookshelf.value = bookshelf.value.filter(id => id !== novelId)
    } else {
        bookshelf.value.push(novelId)
    }
}

const startReading = () => {
    if (novel.value?.chapters.length) {
        goToChapter(novel.value.chapters[0].id)
    }
}

const goToChapter = (chapterId: string) => {
    router.push({
        name: 'chapter-reader',
        params: { id: route.params.id, chapterId }
    })
}

const isChapterRead = (chapterId: string) => {
    return readChapters.value[`${route.params.id}-${chapterId}`]
}

onMounted(async () => {
    await novelStore.fetchNovelDetail(route.params.id as string)
})
</script>

<style scoped lang="scss">
.novel-detail {
    .cover-image {
        width: 100%;
        border-radius: 8px;
        box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
    }

    .info-section {
        h1 {
            margin: 0 0 20px;
            font-size: 2em;
        }

        .meta-info {
            margin-bottom: 15px;
            display: flex;
            align-items: center;
            gap: 15px;

            .author {
                color: #666;
            }
        }

        .categories {
            margin-bottom: 20px;

            .category-tag {
                margin-right: 10px;
            }
        }

        .description {
            color: #666;
            line-height: 1.6;
            margin-bottom: 30px;
        }

        .action-buttons {
            display: flex;
            gap: 15px;
        }
    }

    .chapters-section {
        margin-top: 40px;

        h2 {
            margin-bottom: 20px;
        }

        .chapter-item {
            padding: 15px;
            background: #f5f7fa;
            border-radius: 4px;
            margin-bottom: 15px;
            cursor: pointer;
            display: flex;
            justify-content: space-between;
            align-items: center;
            transition: background-color 0.3s;

            &:hover {
                background: #e6e8eb;
            }

            .read-status {
                font-size: 0.9em;
                color: #67c23a;
            }
        }
    }
}
</style>