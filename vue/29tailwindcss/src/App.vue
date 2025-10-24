<template>
  <div class="min-h-screen bg-gray-900 text-gray-100">
    <!-- 顶部导航 -->
    <nav class="fixed top-0 w-full bg-gray-800/95 backdrop-blur z-50">
      <div class="container mx-auto px-4 h-16 flex items-center justify-between">
        <div class="flex items-center space-x-8">
          <h1 class="text-2xl font-bold bg-gradient-to-r from-purple-500 to-pink-500 bg-clip-text text-transparent">
            漫画世界
          </h1>
          <div class="flex items-center space-x-6">
            <a href="#" class="hover:text-purple-400 transition-colors">首页</a>
            <a href="#" class="hover:text-purple-400 transition-colors">分类</a>
            <a href="#" class="hover:text-purple-400 transition-colors">排行榜</a>
            <a href="#" class="hover:text-purple-400 transition-colors">收藏夹</a>
          </div>
        </div>

        <div class="flex items-center space-x-4">
          <div class="relative">
            <input type="text" placeholder="搜索漫画..."
              class="w-64 px-4 py-2 bg-gray-700 rounded-lg text-sm border-none focus:ring-2 focus:ring-purple-500 focus:outline-none" />
            <i class="fas fa-search absolute right-3 top-1/2 -translate-y-1/2 text-gray-400"></i>
          </div>
          <button
            class="!rounded-button px-4 py-2 bg-purple-600 hover:bg-purple-700 transition-colors whitespace-nowrap">
            登录 / 注册
          </button>
        </div>
      </div>
    </nav>

    <!-- 主要内容区 -->
    <main class="container mx-auto px-4 pt-24">
      <!-- 轮播图 -->
      <div class="relative mb-8 rounded-xl overflow-hidden h-[400px]">
        <swiper :modules="swiperModules" :slides-per-view="1" :loop="true" :autoplay="{
          delay: 3000,
          disableOnInteraction: false
        }" :pagination="{ clickable: true }">
          <swiper-slide v-for="(slide, index) in slides" :key="index">
            <div class="relative h-[400px]">
              <img :src="slide.image" :alt="slide.title" class="w-full h-full object-cover object-top" />
              <div class="absolute inset-0 bg-gradient-to-r from-gray-900/90 to-transparent flex items-center">
                <div class="max-w-lg px-8">
                  <h2 class="text-4xl font-bold mb-4">{{ slide.title }}</h2>
                  <p class="text-lg text-gray-200 mb-6">{{ slide.description }}</p>
                  <button
                    class="!rounded-button px-6 py-3 bg-purple-600 hover:bg-purple-700 transition-colors whitespace-nowrap">
                    立即阅读
                  </button>
                </div>
              </div>
            </div>
          </swiper-slide>
        </swiper>
      </div>

      <!-- 分类导航 -->
      <div class="grid grid-cols-4 gap-4 mb-8">
        <div v-for="category in categories" :key="category.id"
          class="relative h-32 rounded-xl overflow-hidden group cursor-pointer">
          <img :src="category.image" :alt="category.name"
            class="w-full h-full object-cover object-top transition-transform group-hover:scale-110" />
          <div class="absolute inset-0 bg-gradient-to-t from-gray-900/90 to-transparent flex items-end p-4">
            <span class="text-lg font-medium">{{ category.name }}</span>
          </div>
        </div>
      </div>

      <!-- 最新更新 -->
      <div class="mb-12">
        <div class="flex items-center justify-between mb-6">
          <h2 class="text-2xl font-bold">最新更新</h2>
          <button class="!rounded-button px-4 py-2 bg-gray-700 hover:bg-gray-600 transition-colors whitespace-nowrap">
            查看更多
          </button>
        </div>
        <div class="grid grid-cols-6 gap-4">
          <div v-for="manga in latestManga" :key="manga.id" class="group">
            <div class="relative aspect-[3/4] rounded-lg overflow-hidden mb-2">
              <img :src="manga.cover" :alt="manga.title" class="w-full h-full object-cover object-top" />
              <div class="absolute top-2 right-2 bg-purple-600 text-xs px-2 py-1 rounded">NEW</div>
              <div
                class="absolute inset-0 bg-gray-900/80 opacity-0 group-hover:opacity-100 transition-opacity flex items-center justify-center">
                <button
                  class="!rounded-button px-4 py-2 bg-purple-600 hover:bg-purple-700 transition-colors whitespace-nowrap">
                  开始阅读
                </button>
              </div>
            </div>
            <h3 class="font-medium truncate">{{ manga.title }}</h3>
            <p class="text-sm text-gray-400">{{ manga.updateTime }}</p>
          </div>
        </div>
      </div>

      <!-- 热门排行 -->
      <div class="grid grid-cols-3 gap-8 mb-12">
        <div class="bg-gray-800 rounded-xl p-6">
          <h2 class="text-xl font-bold mb-4">日榜 TOP 5</h2>
          <div class="space-y-4">
            <div v-for="(manga, index) in dailyRank" :key="manga.id" class="flex items-center space-x-3">
              <span class="text-2xl font-bold" :class="index < 3 ? 'text-purple-500' : 'text-gray-600'">{{ index + 1
                }}</span>
              <img :src="manga.cover" :alt="manga.title" class="w-16 h-20 object-cover object-top rounded" />
              <div>
                <h3 class="font-medium">{{ manga.title }}</h3>
                <p class="text-sm text-gray-400">{{ manga.views }} 次阅读</p>
              </div>
            </div>
          </div>
        </div>
        <div class="bg-gray-800 rounded-xl p-6">
          <h2 class="text-xl font-bold mb-4">周榜 TOP 5</h2>
          <div class="space-y-4">
            <div v-for="(manga, index) in weeklyRank" :key="manga.id" class="flex items-center space-x-3">
              <span class="text-2xl font-bold" :class="index < 3 ? 'text-purple-500' : 'text-gray-600'">{{ index + 1
                }}</span>
              <img :src="manga.cover" :alt="manga.title" class="w-16 h-20 object-cover object-top rounded" />
              <div>
                <h3 class="font-medium">{{ manga.title }}</h3>
                <p class="text-sm text-gray-400">{{ manga.views }} 次阅读</p>
              </div>
            </div>
          </div>
        </div>
        <div class="bg-gray-800 rounded-xl p-6">
          <h2 class="text-xl font-bold mb-4">月榜 TOP 5</h2>
          <div class="space-y-4">
            <div v-for="(manga, index) in monthlyRank" :key="manga.id" class="flex items-center space-x-3">
              <span class="text-2xl font-bold" :class="index < 3 ? 'text-purple-500' : 'text-gray-600'">{{ index + 1
                }}</span>
              <img :src="manga.cover" :alt="manga.title" class="w-16 h-20 object-cover object-top rounded" />
              <div>
                <h3 class="font-medium">{{ manga.title }}</h3>
                <p class="text-sm text-gray-400">{{ manga.views }} 次阅读</p>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- 评论区 -->
      <div class="bg-gray-800 rounded-xl p-6 mb-12">
        <h2 class="text-2xl font-bold mb-6">读者评论</h2>
        <div class="mb-6">
          <textarea placeholder="发表你的评论..."
            class="w-full h-24 px-4 py-3 bg-gray-700 rounded-lg text-sm border-none resize-none focus:ring-2 focus:ring-purple-500 focus:outline-none"></textarea>
          <div class="flex justify-end mt-2">
            <button
              class="!rounded-button px-4 py-2 bg-purple-600 hover:bg-purple-700 transition-colors whitespace-nowrap">
              发表评论
            </button>
          </div>
        </div>
        <div class="space-y-6">
          <div v-for="comment in comments" :key="comment.id" class="flex space-x-4">
            <img :src="comment.avatar" :alt="comment.username" class="w-10 h-10 rounded-full" />
            <div>
              <div class="flex items-center space-x-2 mb-1">
                <span class="font-medium">{{ comment.username }}</span>
                <span class="text-sm text-gray-400">{{ comment.time }}</span>
              </div>
              <p class="text-gray-300">{{ comment.content }}</p>
            </div>
          </div>
        </div>
      </div>
    </main>

    <!-- 底部导航 -->
    <footer class="bg-gray-800 py-12">
      <div class="container mx-auto px-4">
        <div class="grid grid-cols-4 gap-8 mb-8">
          <div>
            <h3 class="text-lg font-bold mb-4">关于我们</h3>
            <ul class="space-y-2 text-gray-400">
              <li><a href="#" class="hover:text-purple-400">关于漫画世界</a></li>
              <li><a href="#" class="hover:text-purple-400">联系我们</a></li>
              <li><a href="#" class="hover:text-purple-400">加入我们</a></li>
              <li><a href="#" class="hover:text-purple-400">用户协议</a></li>
            </ul>
          </div>
          <div>
            <h3 class="text-lg font-bold mb-4">帮助中心</h3>
            <ul class="space-y-2 text-gray-400">
              <li><a href="#" class="hover:text-purple-400">常见问题</a></li>
              <li><a href="#" class="hover:text-purple-400">意见反馈</a></li>
              <li><a href="#" class="hover:text-purple-400">漫画投稿</a></li>
              <li><a href="#" class="hover:text-purple-400">举报中心</a></li>
            </ul>
          </div>
          <div>
            <h3 class="text-lg font-bold mb-4">友情链接</h3>
            <ul class="space-y-2 text-gray-400">
              <li><a href="#" class="hover:text-purple-400">动漫之家</a></li>
              <li><a href="#" class="hover:text-purple-400">漫画社区</a></li>
              <li><a href="#" class="hover:text-purple-400">二次元世界</a></li>
              <li><a href="#" class="hover:text-purple-400">动漫资讯</a></li>
            </ul>
          </div>
          <div>
            <h3 class="text-lg font-bold mb-4">关注我们</h3>
            <div class="flex space-x-4">
              <a href="#" class="text-2xl hover:text-purple-400"><i class="fab fa-weibo"></i></a>
              <a href="#" class="text-2xl hover:text-purple-400"><i class="fab fa-weixin"></i></a>
              <a href="#" class="text-2xl hover:text-purple-400"><i class="fab fa-qq"></i></a>
            </div>
          </div>
        </div>
        <div class="text-center text-gray-400 text-sm">
          <p>© 2024 漫画世界 版权所有</p>
        </div>
      </div>
    </footer>

    <!-- 返回顶部 -->
    <button @click="scrollToTop"
      class="!rounded-button fixed bottom-8 right-8 w-12 h-12 bg-purple-600 hover:bg-purple-700 transition-colors flex items-center justify-center text-xl whitespace-nowrap"
      :class="{ 'opacity-0': !showBackToTop }">
      <i class="fas fa-arrow-up"></i>
    </button>
  </div>
</template>

<script lang="ts" setup>
import { ref, onMounted, onUnmounted } from 'vue';
import { Swiper, SwiperSlide } from 'swiper/vue';
import { Pagination, Autoplay } from 'swiper/modules';

const swiperModules = [Pagination, Autoplay];

const slides = [
  {
    title: '斗破苍穹：命运之战',
    description: '少年萧炎在经历了家族大变后，踏上了一段传奇般的修炼之路。在这充满危机与机遇的世界里，他将如何突破自我，成就巅峰？',
    image: 'https://ai-public.mastergo.com/ai/img_res/c8a019ca2d8d9deba3127c2d627aeafc.jpg'
  },
  {
    title: '镇魂街：守护者崛起',
    description: '都市之中暗藏着另一个维度的世界，守护者们为保护人间而战。一场关乎两个维度命运的大战即将展开！',
    image: 'https://ai-public.mastergo.com/ai/img_res/9f88851273011cf6f0a0d874e6b5128c.jpg'
  }
];

const categories = [
  {
    id: 1,
    name: '热血格斗',
    image: 'https://ai-public.mastergo.com/ai/img_res/f14d857e1f977a3576c591cca3078486.jpg'
  },
  {
    id: 2,
    name: '奇幻冒险',
    image: 'https://ai-public.mastergo.com/ai/img_res/9947686f827f8467ec3192f1a9e3519a.jpg'
  },
  {
    id: 3,
    name: '恋爱日常',
    image: 'https://ai-public.mastergo.com/ai/img_res/48ba07e15952e082875a60de0de31e43.jpg'
  },
  {
    id: 4,
    name: '悬疑推理',
    image: 'https://ai-public.mastergo.com/ai/img_res/b272684af5ad6a7f5d18fd2ed4c555e3.jpg'
  }
];

const latestManga = [
  {
    id: 1,
    title: '龙族：血脉觉醒',
    updateTime: '10分钟前',
    cover: 'https://ai-public.mastergo.com/ai/img_res/ab4db33a8509273675d0cb1786736cbd.jpg'
  },
  {
    id: 2,
    title: '都市之巅',
    updateTime: '30分钟前',
    cover: 'https://ai-public.mastergo.com/ai/img_res/364b648355de0570be4352931f8da9de.jpg'
  },
  {
    id: 3,
    title: '星海传说',
    updateTime: '1小时前',
    cover: 'https://ai-public.mastergo.com/ai/img_res/137ad7c4e360d5299cd0f53ea1f2650e.jpg'
  },
  {
    id: 4,
    title: '武道巅峰',
    updateTime: '2小时前',
    cover: 'https://ai-public.mastergo.com/ai/img_res/bb3da638fda3fd675c0cb65f6cd42690.jpg'
  },
  {
    id: 5,
    title: '幻想学院',
    updateTime: '3小时前',
    cover: 'https://ai-public.mastergo.com/ai/img_res/2ebb2a39219fa12594e3962fc0b21d8e.jpg'
  },
  {
    id: 6,
    title: '末日求生',
    updateTime: '4小时前',
    cover: 'https://ai-public.mastergo.com/ai/img_res/e7f66e745a5e6b2c4423cc9971f97d6d.jpg'
  }
];

const dailyRank = [
  {
    id: 1,
    title: '御兽战纪',
    views: '128.5万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/2a265571fd83be7a54de58d6c98f3292.jpg'
  },
  {
    id: 2,
    title: '灵剑尊',
    views: '98.3万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/d912d9af01b590379dbcd292802c8a7d.jpg'
  },
  {
    id: 3,
    title: '仙武世界',
    views: '76.2万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/63475511f00f8a5cc089421531d3a57d.jpg'
  },
  {
    id: 4,
    title: '魔法学徒',
    views: '65.8万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/09f6169f1f25a015b66b34ea9c879417.jpg'
  },
  {
    id: 5,
    title: '机甲纪元',
    views: '54.1万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/15930406bdb8f51f8430cba14815f7a9.jpg'
  }
];

const weeklyRank = [
  {
    id: 1,
    title: '修真聊天群',
    views: '892.3万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/f687dde1ac70089226bb194830dbd1df.jpg'
  },
  {
    id: 2,
    title: '全职法师',
    views: '756.8万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/b00d14cd96664bfdd2d725146329b048.jpg'
  },
  {
    id: 3,
    title: '星际猎人',
    views: '634.5万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/ff8945ef28eb8213ae065bd11c8ef513.jpg'
  },
  {
    id: 4,
    title: '妖怪学院',
    views: '521.7万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/5fbe73f242dc068f996a730312b73543.jpg'
  },
  {
    id: 5,
    title: '都市修仙',
    views: '498.2万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/8a5889453db56ae5d2151dda30e708a5.jpg'
  }
];

const monthlyRank = [
  {
    id: 1,
    title: '一剑独尊',
    views: '2891.5万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/89e333adf7795884bddc5c9a53bf4fbe.jpg'
  },
  {
    id: 2,
    title: '妖精的尾巴',
    views: '2567.8万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/2258ad590b6b9c8e2fa344fe2e9b9a79.jpg'
  },
  {
    id: 3,
    title: '武神主宰',
    views: '2345.6万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/c603a6ede95a0384710ce4db4112e44e.jpg'
  },
  {
    id: 4,
    title: '元尊',
    views: '2123.4万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/f45fa02dc17638cf07afa2c054dff205.jpg'
  },
  {
    id: 5,
    title: '斗罗大陆',
    views: '1987.2万',
    cover: 'https://ai-public.mastergo.com/ai/img_res/1dd37e46891fdf9dd7538e3cd5aa3f27.jpg'
  }
];

const comments = [
  {
    id: 1,
    username: '北辰星痕',
    avatar: 'https://ai-public.mastergo.com/ai/img_res/134a750b1081d064785aef8e9bcd8a05.jpg',
    content: '最新一话的展开太精彩了，没想到主角竟然觉醒了这样的能力！期待下一话的发展。',
    time: '5分钟前'
  },
  {
    id: 2,
    username: '墨雨飘香',
    avatar: 'https://ai-public.mastergo.com/ai/img_res/90df2388d97f4b357b8fa422181c0f3c.jpg',
    content: '画风越来越精致了，特别是战斗场景的细节处理，真的很震撼。',
    time: '15分钟前'
  },
  {
    id: 3,
    username: '刀光剑影',
    avatar: 'https://ai-public.mastergo.com/ai/img_res/da68fa3c608a67527afed0ddd2bb8e5f.jpg',
    content: '剧情发展很紧凑，每一话都很吸引人。作者的脑洞真大！',
    time: '30分钟前'
  }
];

const showBackToTop = ref(false);

const handleScroll = () => {
  showBackToTop.value = window.scrollY > 300;
};

const scrollToTop = () => {
  window.scrollTo({ top: 0, behavior: 'smooth' });
};

onMounted(() => {
  window.addEventListener('scroll', handleScroll);
});

onUnmounted(() => {
  window.removeEventListener('scroll', handleScroll);
});
</script>

<style scoped>
.bg-gradient-to-r {
  background-size: 200% auto;
  animation: shine 2s linear infinite;
}

@keyframes shine {
  to {
    background-position: 200% center;
  }
}

.swiper-pagination-bullet {
  background: #ffffff40;
}

.swiper-pagination-bullet-active {
  background: #ffffff;
}
</style>
