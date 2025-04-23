import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      component: () => import('@/layouts/MainLayout.vue'),
      children: [
        {
          path: '',
          name: 'home',
          component: () => import('@/views/home/index.vue')
        },
        {
          path: 'novels',
          name: 'novel-list',
          component: () => import('@/views/novel/NovelList.vue')
        },
        {
          path: 'novels/:id',
          name: 'novel-detail',
          component: () => import('@/views/novel/NovelDetail.vue')
        },
        {
          path: 'novels/:id/chapter/:chapterId',
          name: 'chapter-reader',
          component: () => import('@/views/novel/ChapterReader.vue')
        }
      ]
    }
  ]
})

export default router
