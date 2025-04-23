import { Request, Response } from 'express'
import Novel from '../models/novel'

export const getNovelList = async (req: Request, res: Response) => {
    try {
        const page = parseInt(req.query.page as string) || 1
        const limit = parseInt(req.query.limit as string) || 20
        const category = req.query.category as string
        const status = req.query.status as 'ongoing' | 'completed'

        const query: any = {}
        if (category) query.categories = category
        if (status) query.status = status

        const [items, total] = await Promise.all([
            Novel.find(query)
                .skip((page - 1) * limit)
                .limit(limit)
                .select('-chapters.content')
                .sort({ lastUpdated: -1 }),
            Novel.countDocuments(query)
        ])

        res.json({
            items,
            total,
            page,
            limit
        })
    } catch (err) {
        res.status(500).json({ message: 'Failed to fetch novels' })
    }
}

export const getNovelDetail = async (req: Request, res: Response) => {
    try {
        const novel = await Novel.findById(req.params.id)
            .select('-chapters.content')

        if (!novel) {
            return res.status(404).json({ message: 'Novel not found' })
        }

        res.json(novel)
    } catch (err) {
        res.status(500).json({ message: 'Failed to fetch novel detail' })
    }
}

export const getChapterContent = async (req: Request, res: Response) => {
    try {
        const novel = await Novel.findById(req.params.id)
        if (!novel) {
            return res.status(404).json({ message: 'Novel not found' })
        }

        const chapter = novel.chapters.find(c => c._id.toString() === req.params.chapterId)
        if (!chapter) {
            return res.status(404).json({ message: 'Chapter not found' })
        }

        res.json(chapter)
    } catch (err) {
        res.status(500).json({ message: 'Failed to fetch chapter content' })
    }
}

export const searchNovels = async (req: Request, res: Response) => {
    try {
        const keyword = req.query.keyword as string
        if (!keyword) {
            return res.status(400).json({ message: 'Keyword is required' })
        }

        const novels = await Novel.find(
            { $text: { $search: keyword } },
            { score: { $meta: 'textScore' } }
        )
            .select('-chapters.content')
            .sort({ score: { $meta: 'textScore' } })
            .limit(20)

        res.json(novels)
    } catch (err) {
        res.status(500).json({ message: 'Failed to search novels' })
    }
}

// 添加新的小说
export const createNovel = async (req: Request, res: Response) => {
    try {
        const novelData = req.body
        const novel = new Novel(novelData)
        await novel.save()
        res.status(201).json(novel)
    } catch (err) {
        res.status(500).json({ message: 'Failed to create novel', error: err })
    }
}

// 添加章节
export const addChapter = async (req: Request, res: Response) => {
    try {
        const { id } = req.params
        const chapterData = req.body

        const novel = await Novel.findById(id)
        if (!novel) {
            return res.status(404).json({ message: 'Novel not found' })
        }

        novel.chapters.push(chapterData)
        novel.lastUpdated = new Date()
        await novel.save()

        res.status(201).json(novel)
    } catch (err) {
        res.status(500).json({ message: 'Failed to add chapter', error: err })
    }
} 