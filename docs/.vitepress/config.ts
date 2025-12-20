import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'PixWit',
  description: '截图 · 录屏 · 简易剪辑',
  lang: 'zh-CN',
  // 直接服务器部署使用根路径
  base: '/',
  
  appearance: 'dark', // 默认暗黑模式
  
  head: [
    ['link', { rel: 'icon', href: '/favicon.ico' }],
    ['meta', { name: 'keywords', content: 'PixWit,截图,录屏,视频剪辑,OCR,屏幕录制,Windows截图工具' }],
  ],

  sitemap: {
    hostname: 'https://pixwit.cn'
  },

  themeConfig: {
    logo: '/logo.svg',
    
    nav: [
      { text: '首页', link: '/' },
      { text: '使用手册', link: '/guide/' },
      { text: '下载', link: '/download' },
      { 
        text: '社区',
        items: [
          { text: 'QQ 群：133125470', link: '#' },
          { text: 'GitHub', link: 'https://github.com/yun-cun-li/PixWit' },
          { text: 'Gitee', link: 'https://gitee.com/bjs1999/PixWit' },
        ]
      }
    ],

    sidebar: {
      '/guide/': [
        {
          text: '快速开始',
          items: [
            { text: '介绍', link: '/guide/' },
            { text: '安装指南', link: '/guide/installation' },
          ]
        },
        {
          text: '核心功能',
          items: [
            { text: '📸 截图功能', link: '/guide/screenshot' },
            { text: '🔍 文字识别', link: '/guide/ocr' },
            { text: '🌐 图片翻译', link: '/guide/translation' },
            { text: '🎬 录屏功能', link: '/guide/recording' },
            { text: '🖼️ 图片处理', link: '/guide/image' },
            { text: '🎞️ 视频处理', link: '/guide/video' },  
          
          
            {
              text: '▦⤴提取表格',
              link: '/guide/table_extraction_guide'
            },
            {
              text: '🤖 深度解读',
              link: '/guide/ai_analysis_guide'
            },
 
            {
              text: '🕒历史记录',
              link: '/guide/screenshot-hist'
            }
          ]
        },
        
      ]
    },

    outline: {
      level: [2, 3],
      label: '本页目录'
    },

    docFooter: {
      prev: '上一页',
      next: '下一页'
    },

    darkModeSwitchLabel: '外观',
    sidebarMenuLabel: '菜单',
    returnToTopLabel: '返回顶部',

    footer: {
      message: '© 2025 PixWit|粤ICP备2025481742号-2',
      copyright: ''
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/yun-cun-li/PixWit' }
    ]
  }
})

