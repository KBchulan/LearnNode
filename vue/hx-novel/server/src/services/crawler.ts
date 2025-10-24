import puppeteer from 'puppeteer-extra'
import StealthPlugin from 'puppeteer-extra-plugin-stealth'
import Novel from '../models/novel'
import { INovel } from '../models/novel'

puppeteer.use(StealthPlugin())

export class NovelCrawler {
    private baseUrl = 'http://www.biquge.com.cn'

    async crawlNovelList(page: number = 1): Promise<void> {
        console.log(`Starting to crawl page ${page}...`)
        console.log('Current novels in database:', await Novel.countDocuments())

        const browser = await puppeteer.launch({
            headless: false,
            args: [
                '--no-sandbox',
                '--disable-setuid-sandbox',
                '--disable-web-security',
                '--ignore-certificate-errors',
                '--ignore-certificate-errors-spki-list',
                '--disable-features=IsolateOrigins,site-per-process',
                '--disable-blink-features=AutomationControlled',
                '--disable-extensions'
            ],
            ignoreDefaultArgs: ['--enable-automation'],
            ignoreHTTPSErrors: true
        })

        try {
            const browserPage = await browser.newPage()

            // 设置更真实的浏览器环境
            await browserPage.setExtraHTTPHeaders({
                'Accept-Language': 'zh-CN,zh;q=0.9',
                'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8'
            })

            // 访问小说列表页面
            const listUrl = `${this.baseUrl}/list/1_${page}.html`
            console.log(`Navigating to ${listUrl}`)
            await browserPage.goto(listUrl, {
                waitUntil: 'networkidle0',
                timeout: 60000
            })

            // 等待小说列表加载
            await browserPage.waitForSelector('#newscontent .l', { timeout: 60000 })

            // 获取小说数据
            const novels = await browserPage.evaluate(() => {
                const items = document.querySelectorAll('#newscontent .l li')
                return Array.from(items).map(item => ({
                    title: item.querySelector('.s2 a')?.textContent?.trim() || '',
                    author: item.querySelector('.s4')?.textContent?.trim() || '',
                    cover: '/default-cover.jpg',
                    description: '',  // 在详情页获取
                    categories: ['小说'],
                    status: 'ongoing',
                    chapters: [],
                    novelUrl: item.querySelector('.s2 a')?.getAttribute('href') || ''
                }))
            })

            console.log(`Successfully extracted ${novels.length} novels`)

            // 保存小说数据
            for (const novelData of novels) {
                try {
                    if (!novelData.title || !novelData.author) {
                        console.log('Skipping invalid novel data:', novelData)
                        continue
                    }

                    const existingNovel = await Novel.findOne({ title: novelData.title })
                    if (!existingNovel) {
                        console.log('Creating new novel:', novelData.title)
                        const novel = new Novel({
                            ...novelData,
                            lastUpdated: new Date()
                        })

                        const savedNovel = await novel.save() as unknown as INovel
                        console.log('Successfully saved novel:', savedNovel.title)

                        if (novelData.novelUrl) {
                            console.log('Fetching chapters for:', savedNovel.title)
                            await this.crawlNovelChapters(savedNovel._id.toString(), novelData.novelUrl, browser)
                        }
                    } else {
                        console.log('Novel already exists:', novelData.title)
                    }
                } catch (error) {
                    console.error('Error saving novel:', novelData.title, error)
                }
            }

            const finalCount = await Novel.countDocuments()
            console.log(`Crawling completed. Total novels in database: ${finalCount}`)

        } catch (error) {
            console.error('Crawler error:', error)
            if (browser) {
                const pages = await browser.pages()
                if (pages.length > 0) {
                    await pages[0].screenshot({ path: 'error.png' })
                }
            }
            throw error
        } finally {
            await browser.close()
        }
    }

    async crawlNovelChapters(novelId: string, novelUrl: string, browser: any): Promise<void> {
        try {
            const page = await browser.newPage()
            await page.goto(novelUrl, {  // 注意这里直接使用完整的 URL
                waitUntil: 'networkidle0',
                timeout: 60000
            })

            const chapters = await page.evaluate(() => {
                const items = document.querySelectorAll('#list dd a')
                return Array.from(items).map((item, index) => ({
                    title: item.textContent?.trim() || '',
                    content: '章节内容需要登录后查看',
                    chapterNumber: index + 1,
                    createdAt: new Date(),
                    chapterUrl: item.getAttribute('href') || ''
                }))
            })

            if (chapters.length > 0) {
                await Novel.findByIdAndUpdate(novelId, {
                    chapters: chapters,
                    lastUpdated: new Date()
                })
            }

            await page.close()
        } catch (error) {
            console.error('Error crawling chapters:', error)
        }
    }
} 