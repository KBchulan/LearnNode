// 书签导航增强功能

// 初始化函数
function initBookmarkManager() {
    setupSearch();
    setupCollapsible();
    setupIconFallback();
    setupThemeSwitcher();
    setupBookmarkStats();
    migrateOldVisitHistory(); // 迁移旧版本历史记录
    setupLastVisited();
}

// 搜索功能增强
function setupSearch() {
    const searchInput = document.getElementById('search');
    if (!searchInput) return;

    searchInput.addEventListener('input', function (e) {
        const searchQuery = e.target.value.toLowerCase().trim();
        const bookmarks = document.querySelectorAll('.bookmark');
        let visibleBookmarksCount = 0;

        // 处理空搜索查询
        if (searchQuery === '') {
            bookmarks.forEach(bookmark => {
                bookmark.style.display = 'flex';
                bookmark.classList.remove('search-highlight');
            });
            document.querySelectorAll('.category').forEach(category => {
                category.style.display = 'block';
            });
            updateSearchStatus('');
            return;
        }

        // 搜索逻辑
        bookmarks.forEach(bookmark => {
            const title = bookmark.querySelector('.bookmark-title').textContent.toLowerCase();
            const url = bookmark.getAttribute('href')?.toLowerCase() || '';
            const visible = title.includes(searchQuery) || url.includes(searchQuery);

            bookmark.style.display = visible ? 'flex' : 'none';

            // 添加高亮效果
            if (visible) {
                visibleBookmarksCount++;
                bookmark.classList.add('search-highlight');
            } else {
                bookmark.classList.remove('search-highlight');
            }
        });

        // 检查每个分类是否有可见的书签
        document.querySelectorAll('.category').forEach(category => {
            const visibleBookmarks = category.querySelectorAll('.bookmark[style="display: flex"]').length;
            category.style.display = visibleBookmarks > 0 ? 'block' : 'none';
        });

        // 更新搜索状态
        updateSearchStatus(`找到 ${visibleBookmarksCount} 个匹配的书签`);
    });
}

// 更新搜索状态
function updateSearchStatus(message) {
    let statusElem = document.getElementById('search-status');
    if (!statusElem) {
        statusElem = document.createElement('div');
        statusElem.id = 'search-status';
        document.querySelector('.search-container').appendChild(statusElem);
    }

    statusElem.textContent = message;
    statusElem.style.display = message ? 'block' : 'none';
}

// 折叠功能
function setupCollapsible() {
    document.querySelectorAll('.collapse-btn').forEach(button => {
        button.addEventListener('click', function () {
            const category = this.closest('.category');
            category.classList.toggle('collapsed');
            this.textContent = category.classList.contains('collapsed') ? '+' : '-';

            // 保存折叠状态
            const categoryId = category.querySelector('.category-title').textContent.trim();
            const collapsedCategories = JSON.parse(localStorage.getItem('collapsed-categories') || '[]');

            if (category.classList.contains('collapsed')) {
                if (!collapsedCategories.includes(categoryId)) {
                    collapsedCategories.push(categoryId);
                }
            } else {
                const index = collapsedCategories.indexOf(categoryId);
                if (index > -1) {
                    collapsedCategories.splice(index, 1);
                }
            }

            localStorage.setItem('collapsed-categories', JSON.stringify(collapsedCategories));
        });
    });

    // 恢复折叠状态
    const collapsedCategories = JSON.parse(localStorage.getItem('collapsed-categories') || '[]');
    if (collapsedCategories.length > 0) {
        document.querySelectorAll('.category').forEach(category => {
            const categoryId = category.querySelector('.category-title').textContent.trim();
            if (collapsedCategories.includes(categoryId)) {
                category.classList.add('collapsed');
                category.querySelector('.collapse-btn').textContent = '+';
            }
        });
    }
}

// 图标加载失败处理
function setupIconFallback() {
    document.querySelectorAll('.bookmark-icon').forEach(icon => {
        if (icon.tagName.toLowerCase() === 'img') {
            icon.addEventListener('error', function () {
                handleIconError(this);
            });

            // 检查图片是否已经加载失败
            if (icon.complete && icon.naturalHeight === 0) {
                handleIconError(icon);
            }
        }
    });
}

function handleIconError(iconElement) {
    try {
        const bookmark = iconElement.closest('a');

        // 检查是否存在预定义的备用图标
        const fallbackIcon = bookmark.querySelector('.bookmark-icon-fallback');
        if (fallbackIcon) {
            // 使用预定义的备用图标
            iconElement.style.display = 'none';
            fallbackIcon.style.display = 'flex';
            return;
        }

        // 如果没有预定义的备用图标，则创建新的图标
        const domain = new URL(bookmark.href).hostname;
        const firstLetter = domain.split('.')[0].charAt(0).toUpperCase();
        const color = getRandomColor();

        const newIcon = document.createElement('div');
        newIcon.className = 'bookmark-icon';
        newIcon.style.backgroundColor = color;
        newIcon.style.color = 'white';
        newIcon.textContent = firstLetter;

        iconElement.parentNode.replaceChild(newIcon, iconElement);
    } catch (e) {
        console.error('Error handling icon fallback:', e);
    }
}

// 随机颜色生成
function getRandomColor() {
    const colors = [
        '#4285f4', '#ea4335', '#fbbc05', '#34a853', // Google colors
        '#5f6368', '#1aa260', '#e1382b', '#9d38bd', // More Google colors
        '#1877f2', '#45bd62', '#f7b928', '#ff7a59', // Facebook & other brand colors
        '#00acee', '#0077b5', '#ff0000', '#6441a5'  // Twitter, LinkedIn, YouTube, Twitch
    ];
    return colors[Math.floor(Math.random() * colors.length)];
}

// 主题切换功能
function setupThemeSwitcher() {
    document.querySelectorAll('.theme-option').forEach(option => {
        option.addEventListener('click', function () {
            const theme = this.getAttribute('data-theme');

            // 移除所有主题类
            document.body.classList.remove('dark-theme', 'green-theme', 'blue-theme', 'orange-theme');

            // 添加选中的主题类
            if (theme !== 'light') {
                document.body.classList.add(`${theme}-theme`);
            }

            // 更新活动状态
            document.querySelectorAll('.theme-option').forEach(opt => {
                opt.classList.remove('active');
            });
            this.classList.add('active');

            // 保存主题到本地存储
            localStorage.setItem('preferred-theme', theme);
        });
    });

    // 从本地存储加载主题
    const savedTheme = localStorage.getItem('preferred-theme');
    if (savedTheme) {
        const themeOption = document.querySelector(`.theme-option[data-theme="${savedTheme}"]`);
        if (themeOption) {
            themeOption.click();
        }
    }
}

// 书签统计功能
function setupBookmarkStats() {
    const statsContainer = document.createElement('div');
    statsContainer.className = 'stats-container';

    const categories = document.querySelectorAll('.category');
    const bookmarks = document.querySelectorAll('.bookmark');

    statsContainer.innerHTML = `
        <div class="stats-item">分类: <span>${categories.length}</span></div>
        <div class="stats-item">书签: <span>${bookmarks.length}</span></div>
    `;

    // 将统计信息添加到页脚
    const footer = document.querySelector('footer');
    if (footer) {
        footer.insertBefore(statsContainer, footer.firstChild);
    }
}

// 最近访问记录
function setupLastVisited() {
    // 获取本地存储中的访问历史
    const visitHistory = JSON.parse(localStorage.getItem('bookmark-visit-history') || '[]');

    // 为每个书签添加点击事件
    document.querySelectorAll('.bookmark').forEach(bookmark => {
        bookmark.addEventListener('click', function () {
            const title = this.querySelector('.bookmark-title').textContent;
            const url = this.getAttribute('href');

            // 获取域名用于图标
            let iconText = '';
            try {
                const domain = new URL(url).hostname;
                iconText = domain.split('.')[0].charAt(0).toUpperCase();
            } catch (e) {
                iconText = title.charAt(0).toUpperCase();
            }

            const timestamp = new Date().getTime();

            // 更新访问历史
            let history = JSON.parse(localStorage.getItem('bookmark-visit-history') || '[]');

            // 检查是否已存在该书签记录
            const existingIndex = history.findIndex(item => item.url === url);
            if (existingIndex > -1) {
                history.splice(existingIndex, 1);
            }

            // 添加到历史开头
            history.unshift({ title, url, iconText, timestamp });

            // 限制历史记录数量
            if (history.length > 10) {
                history = history.slice(0, 10);
            }

            localStorage.setItem('bookmark-visit-history', JSON.stringify(history));
        });
    });

    // 如果有访问历史，创建最近访问分类
    if (visitHistory.length > 0) {
        createRecentlyVisitedCategory(visitHistory);
    }
}

// 创建最近访问分类
function createRecentlyVisitedCategory(visitHistory) {
    // 检查是否已存在最近访问分类
    if (document.querySelector('.category[data-type="recent"]')) {
        return;
    }

    const recentCategory = document.createElement('div');
    recentCategory.className = 'category';
    recentCategory.setAttribute('data-type', 'recent');

    // 创建分类头部
    const categoryHeader = document.createElement('div');
    categoryHeader.className = 'category-header';
    categoryHeader.innerHTML = `
        <div class="category-icon">🕒</div>
        <h2 class="category-title">最近访问</h2>
        <button class="collapse-btn">-</button>
    `;

    // 创建书签容器
    const bookmarksContainer = document.createElement('div');
    bookmarksContainer.className = 'bookmarks';

    // 添加最近访问的书签
    visitHistory.forEach(item => {
        const bookmark = document.createElement('a');
        bookmark.className = 'bookmark';
        bookmark.href = item.url;

        // 格式化时间
        const date = new Date(item.timestamp);
        const formattedDate = `${date.getMonth() + 1}/${date.getDate()} ${date.getHours()}:${date.getMinutes().toString().padStart(2, '0')}`;

        // 创建图标
        let bookmarkInnerHTML = '';

        // 使用新格式（iconText）或从标题/URL中提取
        if (item.iconText) {
            const color = getRandomColor();
            bookmarkInnerHTML = `
                <div class="bookmark-icon" style="background-color: ${color}; color: white; display: flex; align-items: center; justify-content: center;">${item.iconText}</div>
                <div class="bookmark-title">${item.title}</div>
                <div class="bookmark-time">${formattedDate}</div>
            `;
        } else {
            // 尝试从URL中提取首字母
            let iconText = '';
            try {
                const domain = new URL(item.url).hostname;
                iconText = domain.split('.')[0].charAt(0).toUpperCase();
            } catch (e) {
                iconText = item.title.charAt(0).toUpperCase();
            }

            const color = getRandomColor();
            bookmarkInnerHTML = `
                <div class="bookmark-icon" style="background-color: ${color}; color: white; display: flex; align-items: center; justify-content: center;">${iconText}</div>
                <div class="bookmark-title">${item.title}</div>
                <div class="bookmark-time">${formattedDate}</div>
            `;
        }

        bookmark.innerHTML = bookmarkInnerHTML;
        bookmarksContainer.appendChild(bookmark);
    });

    // 组装并添加到页面
    recentCategory.appendChild(categoryHeader);
    recentCategory.appendChild(bookmarksContainer);

    const categoriesContainer = document.querySelector('.categories');
    categoriesContainer.insertBefore(recentCategory, categoriesContainer.firstChild);

    // 添加折叠功能
    const collapseBtn = recentCategory.querySelector('.collapse-btn');
    collapseBtn.addEventListener('click', function () {
        recentCategory.classList.toggle('collapsed');
        this.textContent = recentCategory.classList.contains('collapsed') ? '+' : '-';
    });
}

// 迁移旧版本的访问历史记录到新格式
function migrateOldVisitHistory() {
    const visitHistory = JSON.parse(localStorage.getItem('bookmark-visit-history') || '[]');
    let needsMigration = false;

    // 检查是否需要迁移（查找任何包含icon HTML的记录）
    const migratedHistory = visitHistory.map(item => {
        if (typeof item.icon === 'string' && !item.iconText) {
            needsMigration = true;
            // 尝试从URL提取图标文本
            let iconText = '';
            try {
                const domain = new URL(item.url).hostname;
                iconText = domain.split('.')[0].charAt(0).toUpperCase();
            } catch (e) {
                iconText = item.title.charAt(0).toUpperCase();
            }

            // 创建新的迁移后的记录
            return {
                ...item,
                iconText: iconText
            };
        }
        return item;
    });

    // 如果需要迁移，保存迁移后的历史记录
    if (needsMigration) {
        localStorage.setItem('bookmark-visit-history', JSON.stringify(migratedHistory));
        console.log('已迁移旧版本访问历史记录');
    }
}

// 初始化
document.addEventListener('DOMContentLoaded', initBookmarkManager);