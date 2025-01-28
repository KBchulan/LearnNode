import { IMAGES } from '@/assets/images'

export const mockNovels = [
    {
        id: '1',
        title: '关于我转生成为史莱姆这档事',
        author: '伏瀬',
        cover: IMAGES.DEFAULT_COVER,
        description: '一个普通上班族被歹徒刺杀后，转生到异世界成为史莱姆的故事。在获得了独特的技能和能力后，他开始了在异世界的冒险...',
        categories: ['奇幻', '异世界', '冒险'],
        status: 'ongoing',
        lastUpdated: '2024-01-28',
        chapters: [
            { id: '1-1', title: '第一章 重生', content: '那天，我在街上走着...', novelId: '1', chapterNumber: 1 },
            { id: '1-2', title: '第二章 觉醒', content: '当我意识到自己变成了史莱姆...', novelId: '1', chapterNumber: 2 },
        ]
    },
    {
        id: '2',
        title: '魔法科高校的劣等生',
        author: '佐岛勤',
        cover: IMAGES.DEFAULT_COVER,
        description: '魔法已经成为科学技术的一部分的世界，在魔法科高中就读的兄妹，展开了一段非同寻常的校园生活...',
        categories: ['校园', '奇幻', '科幻'],
        status: 'completed',
        lastUpdated: '2024-01-27',
        chapters: [
            { id: '2-1', title: '第一章 入学', content: '魔法科高校迎来了新的学期...', novelId: '2', chapterNumber: 1 }
        ]
    },
    {
        id: '3',
        title: '刀剑神域',
        author: '川原砾',
        cover: IMAGES.DEFAULT_COVER,
        description: '在虚拟现实游戏世界中被困的玩家们，必须通关游戏才能回到现实世界...',
        categories: ['奇幻', '游戏', '冒险'],
        status: 'ongoing',
        lastUpdated: '2024-01-26',
        chapters: [
            { id: '3-1', title: '第一章 游戏开始', content: '这是一个改变了一切的日子...', novelId: '3', chapterNumber: 1 }
        ]
    },
    {
        id: '4',
        title: '我的青春恋爱物语果然有问题',
        author: '渡航',
        cover: IMAGES.DEFAULT_COVER,
        description: '高中生比企谷八幡被强制加入学校的侍奉部，开始了一段特别的青春故事...',
        categories: ['校园', '恋爱', '日常'],
        status: 'completed',
        lastUpdated: '2024-01-25',
        chapters: [
            { id: '4-1', title: '第一章 侍奉部', content: '那是开学后的第一个星期...', novelId: '4', chapterNumber: 1 }
        ]
    }
]

// 模拟API响应延迟
const delay = (ms: number) => new Promise(resolve => setTimeout(resolve, ms))

// 模拟API
export const mockApi = {
    async getNovelList(page = 1, limit = 20) {
        await delay(500) // 模拟网络延迟
        const start = (page - 1) * limit
        const end = start + limit
        return {
            items: mockNovels.slice(start, end),
            total: mockNovels.length
        }
    },

    async getNovelDetail(id: string) {
        await delay(300)
        return mockNovels.find(novel => novel.id === id)
    },

    async getChapterContent(novelId: string, chapterId: string) {
        await delay(200)
        const novel = mockNovels.find(n => n.id === novelId)
        return novel?.chapters.find(c => c.id === chapterId)
    },

    async searchNovels(keyword: string) {
        await delay(300)
        return mockNovels.filter(novel =>
            novel.title.includes(keyword) ||
            novel.author.includes(keyword) ||
            novel.description.includes(keyword)
        )
    }
} 