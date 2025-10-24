import { Request, Response } from 'express'
import { NovelCrawler } from '../services/crawler'

const crawler = new NovelCrawler()

export const startCrawling = async (req: Request, res: Response) => {
    try {
        const page = parseInt(req.query.page as string) || 1
        const maxPages = parseInt(req.query.maxPages as string) || 1

        console.log(`Starting crawler for pages ${page} to ${page + maxPages - 1}`)

        // 先发送响应
        res.json({ message: 'Crawling started successfully' })

        // 异步执行爬虫任务
        for (let i = page; i < page + maxPages; i++) {
            try {
                console.log(`Crawling page ${i}...`)
                await crawler.crawlNovelList(i)
                console.log(`Finished crawling page ${i}`)
                // 每页之间添加延迟
                await new Promise(resolve => setTimeout(resolve, 2000))
            } catch (error) {
                console.error(`Error crawling page ${i}:`, error)
            }
        }
    } catch (error) {
        console.error('Failed to start crawling:', error)
        // 如果响应还没有发送，则发送错误响应
        if (!res.headersSent) {
            res.status(500).json({ message: 'Failed to start crawling', error })
        }
    }
} 