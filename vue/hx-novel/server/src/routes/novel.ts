import { Router } from 'express'
import * as NovelController from '../controllers/novel'

const router = Router()

router.get('/', NovelController.getNovelList)
router.get('/search', NovelController.searchNovels)
router.get('/:id', NovelController.getNovelDetail)
router.get('/:id/chapters/:chapterId', NovelController.getChapterContent)

router.post('/', NovelController.createNovel)
router.post('/:id/chapters', NovelController.addChapter)

export default router 