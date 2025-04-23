import { mockApi } from '@/mock/novels'
import type { Novel, Chapter } from '@/types/novel'

export interface NovelListResponse {
    items: Novel[]
    total: number
}

export const novelApi = {
    // 获取小说列表
    getNovelList: async (page = 1, filter = {}) => {
        const response = await mockApi.getNovelList(page)
        return response
    },

    // 获取小说详情
    getNovelDetail: async (id: string) => {
        const response = await mockApi.getNovelDetail(id)
        return { data: response }
    },

    // 获取章节内容
    getChapterContent: async (novelId: string, chapterId: string) => {
        const response = await mockApi.getChapterContent(novelId, chapterId)
        return { data: response }
    },

    // 搜索小说
    searchNovels: async (keyword: string) => {
        const response = await mockApi.searchNovels(keyword)
        return { data: response }
    }
} 