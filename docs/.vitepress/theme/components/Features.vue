<template>
  <div class="features-section">
    <h2 class="features-title">✨ 核心特性</h2>
    <div class="features-grid">
      <div v-for="feature in features" :key="feature.title" class="feature-card">
        <span class="feature-icon">{{ feature.icon }}</span>
        <h3 class="feature-title">{{ feature.title }}</h3>
        <p class="feature-desc">{{ feature.desc }}</p>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
const features = [
  {
    icon: '🖥️',
    title: '单屏截图',
    desc: '在哪个屏幕按F1就在哪个屏幕截图，多屏环境完美支持'
  },
  {
    icon: '📜',
    title: '历史记录',
    desc: '每次截图自动保存，系统托盘一键查看历史'
  },
  {
    icon: '🎨',
    title: '丰富编辑工具',
    desc: '画笔、箭头、矩形、圆形、马赛克、文字，满足各种标注需求'
  },
  {
    icon: '🎥',
    title: '录屏模式',
    desc: '支持全屏录制和区域录制，可暂停/恢复，录制时实时标注'
  },
  {
    icon: '✂️',
    title: '视频剪辑',
    desc: '头尾裁剪、删除中间段、专业时间轴、格式转换、视频压缩'
  },
  {
    icon: '🔍',
    title: 'OCR 识别',
    desc: '一键识别图片中的文字，自动复制到剪贴板'
  },
  {
    icon: '📌',
    title: '桌面贴图',
    desc: '创建可移动、可缩放的桌面便签，方便对照参考'
  },
  {
    icon: '🎤',
    title: '智能音频',
    desc: '自动检测麦克风和系统音频，支持多种录音模式'
  },
  {
    icon: '⚡',
    title: '高性能',
    desc: '基于 Qt 6 开发，多屏混合DPI环境完美支持'
  }
]
</script>

