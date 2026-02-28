<template>
  <nav v-if="breadcrumbs.length > 1" class="breadcrumb" aria-label="面包屑导航">
    <ol class="breadcrumb-list" itemscope itemtype="https://schema.org/BreadcrumbList">
      <li 
        v-for="(item, index) in breadcrumbs" 
        :key="item.path"
        class="breadcrumb-item"
        itemprop="itemListElement" 
        itemscope 
        itemtype="https://schema.org/ListItem"
      >
        <a 
          v-if="index < breadcrumbs.length - 1" 
          :href="item.path" 
          class="breadcrumb-link"
          itemprop="item"
        >
          <span itemprop="name">{{ item.text }}</span>
        </a>
        <span v-else class="breadcrumb-current" itemprop="name">
          {{ item.text }}
        </span>
        <meta itemprop="position" :content="String(index + 1)" />
        <span v-if="index < breadcrumbs.length - 1" class="breadcrumb-separator" aria-hidden="true"> / </span>
      </li>
    </ol>
  </nav>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { useData, useRoute } from 'vitepress'

const { page, theme } = useData()
const route = useRoute()

interface Breadcrumb {
  text: string
  path: string
}

const breadcrumbs = computed<Breadcrumb[]>(() => {
  const path = route.path
  const crumbs: Breadcrumb[] = [
    { text: '首页', path: '/' }
  ]

  // 如果是首页，不显示面包屑
  if (path === '/' || path === '/index.html') {
    return []
  }

  const segments = path.split('/').filter(Boolean)
  
  // 处理路径段
  segments.forEach((segment, index) => {
    // 移除 .html 后缀
    const cleanSegment = segment.replace('.html', '')
    
    // 构建当前路径
    const currentPath = '/' + segments.slice(0, index + 1).join('/')
    
    // 获取显示文本
    let text = getPageTitle(currentPath, cleanSegment)
    
    crumbs.push({
      text,
      path: currentPath
    })
  })

  return crumbs
})

// 根据路径获取页面标题
function getPageTitle(path: string, segment: string): string {
  // 映射关系
  const titleMap: Record<string, string> = {
    'guide': '使用手册',
    'download': '下载',
    'screenshot': '截图功能',
    'recording': '录屏功能',
    'video': '视频处理',
    'image': '图片处理',
    'ocr': 'OCR 识别',
    'translation': '图片翻译',
    'table_extraction_guide': '表格提取',
    'ai_analysis_guide': '智能解读',
    'screenshot-hist': '历史截图',
    'installation': '安装指南',
    'shortcuts': '快捷键',
    'tips': '使用技巧',
    'faq': '常见问题'
  }

  return titleMap[segment] || segment
}
</script>

<style scoped>
.breadcrumb {
  padding: 12px 24px;
  margin-bottom: 16px;
  background-color: var(--vp-c-bg-soft);
  border-radius: 8px;
  font-size: 14px;
}

.breadcrumb-list {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  list-style: none;
  margin: 0;
  padding: 0;
  gap: 6px;
}

.breadcrumb-item {
  display: flex;
  align-items: center;
  gap: 6px;
}

.breadcrumb-link {
  color: var(--vp-c-text-2);
  text-decoration: none;
  transition: color 0.2s;
}

.breadcrumb-link:hover {
  color: var(--vp-c-brand-1);
  text-decoration: underline;
}

.breadcrumb-current {
  color: var(--vp-c-text-1);
  font-weight: 500;
}

.breadcrumb-separator {
  color: var(--vp-c-text-3);
  user-select: none;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .breadcrumb {
    padding: 8px 16px;
    font-size: 13px;
  }
}

/* 暗色模式优化 */
.dark .breadcrumb {
  background-color: var(--vp-c-bg-soft);
}
</style>

