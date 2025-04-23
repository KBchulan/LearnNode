import { defineStore } from 'pinia'
import { ref } from 'vue'
import type { Novel } from '@/types/novel'
import { novelApi } from '@/api/novel'

export const useNovelStore = defineStore('novel', () => {
    const currentNovel = ref<Novel | null>(null)
    const novelList = ref<Novel[]>([])
    const loading = ref(false)
    const error = ref('')

    const fetchNovelList = async (page = 1, filter = {}) => {
        try {
            loading.value = true
            const response = await novelApi.getNovelList(page)
            novelList.value = response.items
            return {
                data: response
            }
        } catch (err) {
            error.value = '获取小说列表失败'
            console.error(err)
            return {
                data: {
                    items: [],
                    total: 0
                }
            }
        } finally {
            loading.value = false
        }
    }

    const fetchNovelDetail = async (id: string) => {
        try {
            loading.value = true
            const response = await novelApi.getNovelDetail(id)
            currentNovel.value = response.data
        } catch (err) {
            error.value = '获取小说详情失败'
            console.error(err)
        } finally {
            loading.value = false
        }
    }

    const fetchChapterContent = async (novelId: string, chapterId: string) => {
        try {
            loading.value = true
            const response = await novelApi.getChapterContent(novelId, chapterId)
            return response.data
        } catch (err) {
            error.value = '获取章节内容失败'
            console.error(err)
            return null
        } finally {
            loading.value = false
        }
    }

    return {
        currentNovel,
        novelList,
        loading,
        error,
        fetchNovelList,
        fetchNovelDetail,
        fetchChapterContent
    }
}) 