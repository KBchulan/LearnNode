import { Router } from 'express'
import * as CrawlerController from '../controllers/crawler'

const router = Router()

router.post('/start', CrawlerController.startCrawling)

export default router 