<template>
  <div class="features-section">
    <h2 class="features-title">✨ 核心特性</h2>
    <div class="features-grid">
      <a 
        v-for="feature in features" 
        :key="feature.title" 
        :href="feature.link"
        class="feature-card"
      >
        <span class="feature-icon">{{ feature.icon }}</span>
        <h3 class="feature-title">{{ feature.title }}</h3>
        <p class="feature-desc">{{ feature.desc }}</p>
      </a>
    </div>
  </div>
</template>

<script setup lang="ts">
const features = [
  {
    icon: '🖥️',
    title: '多屏截图',
    desc: '在哪个屏幕按F1就在哪个屏幕截图，多屏环境完美支持',
    link: '/guide/screenshot'
  },  

  {
    icon: '🎥',
    title: '录屏模式',
    desc: '支持全屏录制和区域录制，可暂停/恢复，录制时实时标注',
    link: '/guide/recording'
  },
  
  {
    icon: '✂️',
    title: '视频剪辑',
    desc: '头尾裁剪、删除中间段、专业时间轴、格式转换、视频压缩',
    link: '/guide/video'
  },
  {
    icon: '🔍',
    title: 'OCR 识别',
    desc: '一键识别图片中的文字，自动复制到剪贴板',
    link: '/guide/ocr'
  },

  {
    icon: '▦⤴',
    title: '提取表格',
    desc: '一键识别图片中的表格，可复制到excel中',
    link: '/guide/table_extraction_guide'
  },
  {
    icon: '🤖',
    title: '深度解读',
    desc: '一键解读图片中的内容，能够深度理解和解读图片中的文本内容',
    link: '/guide/ai_analysis_guide'
  },
  {
    icon: '🌐',
    title: '图片翻译',
    desc: '一键识别并翻译图片文字，支持多种语言翻译成中文',
    link: '/guide/translation'
  },
  {
    icon: '📜',
    title: '历史记录',
    desc: '每次截图自动保存，系统托盘一键查看历史',
    link: '/guide/screenshot#历史记录'
  } , {
    icon: '🎨',
    title: '丰富编辑工具',
    desc: '画笔、箭头、矩形、圆形、马赛克、文字，满足各种标注需求',
    link: '/guide/screenshot#编辑工具-🎨'
  }
]
</script>

