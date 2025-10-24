import mongoose, { Schema, Document } from 'mongoose'

export interface INovel extends Document {
    title: string
    author: string
    cover: string
    description: string
    categories: string[]
    status: 'ongoing' | 'completed'
    lastUpdated: Date
    chapters: {
        _id?: string
        title: string
        content: string
        chapterNumber: number
        createdAt: Date
    }[]
}

const NovelSchema = new Schema({
    title: { type: String, required: true },
    author: { type: String, required: true },
    cover: { type: String, required: true },
    description: { type: String, required: true },
    categories: [{ type: String }],
    status: {
        type: String,
        enum: ['ongoing', 'completed'],
        default: 'ongoing'
    },
    lastUpdated: { type: Date, default: Date.now },
    chapters: [{
        title: { type: String, required: true },
        content: { type: String, required: true },
        chapterNumber: { type: Number, required: true },
        createdAt: { type: Date, default: Date.now }
    }]
}, {
    timestamps: true
})

// 添加索引以优化查询
NovelSchema.index({ title: 'text', author: 'text', description: 'text' })
NovelSchema.index({ categories: 1 })
NovelSchema.index({ status: 1 })
NovelSchema.index({ lastUpdated: -1 })

export default mongoose.model<INovel>('Novel', NovelSchema) 