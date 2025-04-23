<template>
    <div class="chapter-reader" :class="{ 'dark-mode': isDarkMode }">
        <div class="reader-header">
            <div class="title">{{ chapter?.title }}</div>
            <div class="controls">
                <el-button-group>
                    <el-button :icon="Minus" @click="decreaseFontSize" />
                    <el-button :icon="Plus" @click="increaseFontSize" />
                </el-button-group>
                <el-button :icon="Moon" @click="toggleDarkMode" />
            </div>
        </div>

        <div class="content" :style="{ fontSize: `${fontSize}px` }">
            {{ chapter?.content }}
        </div>

        <div class="navigation">
            <el-button :disabled="!hasPrevChapter" @click="goToPrevChapter">上一章</el-button>
            <el-button type="primary" @click="showChapterList = true">目录</el-button>
            <el-button :disabled="!hasNextChapter" @click="goToNextChapter">下一章</el-button>
        </div>

        <el-drawer v-model="showChapterList" title="章节目录" direction="rtl">
            <div class="chapter-list">
                <div v-for="item in novel?.chapters" :key="item.id" class="chapter-item"
                    :class="{ active: item.id === chapter?.id }" @click="goToChapter(item.id)">
                    {{ item.title }}
                </div>
            </div>
        </el-drawer>
    </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, watch } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { useNovelStore } from '@/stores/novel'
import { useLocalStorage } from '@vueuse/core'
import { Minus, Plus, Moon } from '@element-plus/icons-vue'
import type { Chapter } from '@/types/novel'

const route = useRoute()
const router = useRouter()
const novelStore = useNovelStore()

const chapter = ref<Chapter | null>(null)
const showChapterList = ref(false)
const fontSize = useLocalStorage('reader-font-size', 18)
const isDarkMode = useLocalStorage('reader-dark-mode', false)

const novel = computed(() => novelStore.currentNovel)
const currentChapterIndex = computed(() => {
    if (!novel.value?.chapters || !chapter.value) return -1
    return novel.value.chapters.findIndex(c => c.id === chapter.value?.id)
})

const hasPrevChapter = computed(() => currentChapterIndex.value > 0)
const hasNextChapter = computed(() => {
    if (!novel.value?.chapters) return false
    return currentChapterIndex.value < novel.value.chapters.length - 1
})

const readChapters = useLocalStorage<Record<string, boolean>>('readChapters', {})

const loadChapter = async (chapterId: string) => {
    try {
        const response = await novelStore.fetchChapterContent(route.params.id as string, chapterId)
        chapter.value = response
        // 标记为已读
        readChapters.value[`${route.params.id}-${chapterId}`] = true
        // 保存阅读进度
        localStorage.setItem(`reading-progress-${route.params.id}`, chapterId)
    } catch (error) {
        console.error('加载章节失败:', error)
    }
}

const goToChapter = (chapterId: string) => {
    router.push({
        name: 'chapter-reader',
        params: { ...route.params, chapterId }
    })
    showChapterList.value = false
}

const goToPrevChapter = () => {
    if (hasPrevChapter.value && novel.value) {
        const prevChapter = novel.value.chapters[currentChapterIndex.value - 1]
        goToChapter(prevChapter.id)
    }
}

const goToNextChapter = () => {
    if (hasNextChapter.value && novel.value) {
        const nextChapter = novel.value.chapters[currentChapterIndex.value + 1]
        goToChapter(nextChapter.id)
    }
}

const increaseFontSize = () => {
    if (fontSize.value < 32) fontSize.value += 2
}

const decreaseFontSize = () => {
    if (fontSize.value > 12) fontSize.value -= 2
}

const toggleDarkMode = () => {
    isDarkMode.value = !isDarkMode.value
}

watch(() => route.params.chapterId, (newChapterId) => {
    if (newChapterId) {
        loadChapter(newChapterId as string)
    }
})

onMounted(async () => {
    if (!novel.value) {
        await novelStore.fetchNovelDetail(route.params.id as string)
    }
    if (route.params.chapterId) {
        await loadChapter(route.params.chapterId as string)
    }
})
</script>

<style scoped lang="scss">
.chapter-reader {
    max-width: 800px;
    margin: 0 auto;
    padding: 20px;
    min-height: calc(100vh - 120px);
    background: #fff;
    transition: all 0.3s ease;

    &.dark-mode {
        background: #1a1a1a;
        color: #e0e0e0;

        .reader-header {
            background: #2a2a2a;
        }

        .content {
            color: #e0e0e0;
        }

        .chapter-list {
            .chapter-item {
                background: #2a2a2a;
                color: #e0e0e0;

                &:hover {
                    background: #3a3a3a;
                }

                &.active {
                    background: #4a4a4a;
                }
            }
        }
    }

    .reader-header {
        position: sticky;
        top: 0;
        background: #fff;
        padding: 15px 0;
        margin-bottom: 20px;
        border-bottom: 1px solid #eee;
        display: flex;
        justify-content: space-between;
        align-items: center;
        z-index: 10;

        .title {
            font-size: 1.2em;
            font-weight: bold;
        }

        .controls {
            display: flex;
            gap: 10px;
        }
    }

    .content {
        line-height: 1.8;
        margin-bottom: 40px;
        text-align: justify;
        color: #333;
        transition: font-size 0.3s ease;
    }

    .navigation {
        display: flex;
        justify-content: center;
        gap: 20px;
        margin-top: 40px;
        padding: 20px 0;
        border-top: 1px solid #eee;
    }
}

.chapter-list {
    .chapter-item {
        padding: 12px 15px;
        margin-bottom: 8px;
        border-radius: 4px;
        cursor: pointer;
        background: #f5f7fa;
        transition: background-color 0.3s;

        &:hover {
            background: #e6e8eb;
        }

        &.active {
            background: #ecf5ff;
            color: #409eff;
        }
    }
}
</style>