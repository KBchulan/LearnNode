<template>
    <div class="novel-list">
        <div class="filter-section">
            <el-row :gutter="20">
                <el-col :span="24">
                    <el-card>
                        <div class="filter-content">
                            <div class="filter-item">
                                <span class="label">分类：</span>
                                <el-radio-group v-model="filter.category" @change="handleFilter">
                                    <el-radio-button label="">全部</el-radio-button>
                                    <el-radio-button v-for="category in categories" :key="category" :label="category">
                                        {{ category }}
                                    </el-radio-button>
                                </el-radio-group>
                            </div>
                            <div class="filter-item">
                                <span class="label">状态：</span>
                                <el-radio-group v-model="filter.status" @change="handleFilter">
                                    <el-radio-button label="">全部</el-radio-button>
                                    <el-radio-button label="ongoing">连载中</el-radio-button>
                                    <el-radio-button label="completed">已完结</el-radio-button>
                                </el-radio-group>
                            </div>
                        </div>
                    </el-card>
                </el-col>
            </el-row>
        </div>

        <div class="novels-grid">
            <el-row :gutter="20">
                <el-col v-for="novel in novels" :key="novel.id" :span="6">
                    <el-card :body-style="{ padding: '0px' }" class="novel-card" @click="goToNovel(novel.id)">
                        <div class="novel-cover">
                            <img :src="novel.cover || IMAGES.DEFAULT_COVER" />
                            <div class="novel-status">
                                <el-tag size="small" :type="novel.status === 'completed' ? 'success' : 'warning'">
                                    {{ novel.status === 'completed' ? '已完结' : '连载中' }}
                                </el-tag>
                            </div>
                        </div>
                        <div class="novel-info">
                            <h3>{{ novel.title }}</h3>
                            <p class="author">{{ novel.author }}</p>
                            <p class="description">{{ novel.description }}</p>
                            <div class="categories">
                                <el-tag v-for="cat in novel.categories" :key="cat" size="small" effect="plain">
                                    {{ cat }}
                                </el-tag>
                            </div>
                        </div>
                    </el-card>
                </el-col>
            </el-row>
        </div>

        <div class="pagination">
            <el-pagination v-model:current-page="currentPage" v-model:page-size="pageSize" :total="total"
                :page-sizes="[12, 24, 36, 48]" layout="total, sizes, prev, pager, next" @size-change="handleSizeChange"
                @current-change="handleCurrentChange" />
        </div>
    </div>
</template>

<script setup lang="ts">
import { ref, reactive, onMounted, watch } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { useNovelStore } from '@/stores/novel'
import { IMAGES } from '@/assets/images'
import type { Novel } from '@/types/novel'

const route = useRoute()
const router = useRouter()
const novelStore = useNovelStore()

const novels = ref<Novel[]>([])
const total = ref(0)
const currentPage = ref(1)
const pageSize = ref(12)
const categories = ['奇幻', '异世界', '冒险', '校园', '恋爱', '日常', '科幻', '游戏']

const filter = reactive({
    category: route.query.category as string || '',
    status: '',
})

const loadNovels = async () => {
    const response = await novelStore.fetchNovelList(currentPage.value)
    if (response?.data?.items) {
        novels.value = response.data.items
        total.value = response.data.total
    }
}

const handleFilter = () => {
    currentPage.value = 1
    loadNovels()
}

const handleSizeChange = (val: number) => {
    pageSize.value = val
    loadNovels()
}

const handleCurrentChange = (val: number) => {
    currentPage.value = val
    loadNovels()
}

const goToNovel = (id: string) => {
    router.push({ name: 'novel-detail', params: { id } })
}

watch(
    () => route.query,
    (query) => {
        if (query.category) {
            filter.category = query.category as string
        }
        loadNovels()
    },
    { immediate: true }
)

onMounted(() => {
    if (!route.query.category) {
        loadNovels()
    }
})
</script>

<style scoped lang="scss">
.novel-list {
    .filter-section {
        margin-bottom: 20px;

        .filter-content {
            .filter-item {
                margin-bottom: 15px;

                &:last-child {
                    margin-bottom: 0;
                }

                .label {
                    display: inline-block;
                    width: 60px;
                    color: #666;
                }
            }
        }
    }

    .novels-grid {
        margin-bottom: 20px;
    }

    .novel-card {
        cursor: pointer;
        transition: all 0.3s;
        margin-bottom: 20px;
        border: none;
        box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);

        &:hover {
            transform: translateY(-5px);
            box-shadow: 0 4px 16px rgba(0, 0, 0, 0.15);
        }

        .novel-cover {
            position: relative;
            height: 220px;

            img {
                width: 100%;
                height: 100%;
                object-fit: cover;
                border-radius: 4px 4px 0 0;
            }

            .novel-status {
                position: absolute;
                top: 10px;
                right: 10px;
            }
        }

        .novel-info {
            padding: 12px;
            background: #fff;
            border-radius: 0 0 4px 4px;

            h3 {
                margin: 0 0 8px;
                font-size: 1.1em;
                white-space: nowrap;
                overflow: hidden;
                text-overflow: ellipsis;
                color: #333;
            }

            .author {
                color: #666;
                margin-bottom: 8px;
                font-size: 0.9em;
            }

            .description {
                color: #999;
                font-size: 0.9em;
                height: 38px;
                overflow: hidden;
                text-overflow: ellipsis;
                display: -webkit-box;
                -webkit-line-clamp: 2;
                -webkit-box-orient: vertical;
                line-height: 1.4;
                margin-bottom: 8px;
            }

            .categories {
                display: flex;
                flex-wrap: wrap;
                gap: 5px;
            }
        }
    }

    .pagination {
        text-align: center;
        margin-top: 20px;
    }
}
</style>