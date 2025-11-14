import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'PixWit',
  description: 'PixWit 官方网站 - 截图 · 录屏 · 简易剪辑',
  lang: 'zh-CN',
  
  head: [
    ['link', { rel: 'icon', href: '/favicon.ico' }],
    ['meta', { name: 'theme-color', content: '#42b883' }]
  ],

  themeConfig: {
    logo: '/logo.svg',
    
    nav: [
      { text: '首页', link: '/' },
      { text: '使用指南', link: '/guide/' },
      { text: '下载', link: '/download' },
      {
        text: 'GitHub',
        link: 'https://github.com/yun-cun-li/PixWit'
      }
    ],
    
    sidebar: {
      '/guide/': [
        {
          text: '开始使用',
          items: [
            { text: '简介', link: '/guide/' },
            { text: '安装说明', link: '/guide/installation' }
          ]
        },
        {
          text: '功能指南',
          items: [
            { text: '截图功能', link: '/guide/screenshot' },
            { text: '图片标注', link: '/guide/image' },
            { text: 'OCR识别', link: '/guide/ocr' },
            { text: '录屏功能', link: '/guide/recording' },
            { text: '视频剪辑', link: '/guide/video' }
          ]
        },
        {
          text: '其他',
          items: [
            { text: '快捷键', link: '/guide/shortcuts' },
            { text: '使用技巧', link: '/guide/tips' },
            { text: '常见问题', link: '/guide/faq' }
          ]
        }
      ]
    },
    
    socialLinks: [
      { icon: 'github', link: 'https://github.com/yun-cun-li/PixWit' }
    ],
    
    footer: {
      message: '基于 GPL-3.0 许可发布',
      copyright: 'Copyright © 2024 PixWit Team'
    },

    search: {
      provider: 'local'
    },

    outline: {
      label: '页面导航',
      level: [2, 3]
    },

    docFooter: {
      prev: '上一页',
      next: '下一页'
    },

    lastUpdated: {
      text: '最后更新于',
      formatOptions: {
        dateStyle: 'short',
        timeStyle: 'short'
      }
    }
  }
})

