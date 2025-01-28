export interface Novel {
    id: string
    title: string
    author: string
    cover: string
    description: string
    categories: string[]
    chapters: Chapter[]
    status: 'ongoing' | 'completed'
    lastUpdated: string
}

export interface Chapter {
    id: string
    title: string
    content: string
    novelId: string
    chapterNumber: number
} 