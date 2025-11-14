import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'PixWit',
  description: '截图 · 录屏 · 简易剪辑',
  lang: 'zh-CN',
  // GitHub Pages 部署时需要 base 路径，本地开发时使用 '/'
  base: process.env.NODE_ENV === 'production' ? '/PixWit/' : '/',
  
  head: [
    ['link', { rel: 'icon', href: '/favicon.ico' }],
    ['meta', { name: 'keywords', content: 'PixWit,截图,录屏,视频剪辑,OCR,屏幕录制,Windows截图工具' }],
  ],

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
            { text: '🎬 录屏功能', link: '/guide/recording' },
            { text: '🖼️ 图片处理', link: '/guide/image' },
            { text: '🎞️ 视频处理', link: '/guide/video' },
            { text: '🔍 OCR 文字识别', link: '/guide/ocr' },
          ]
        },
        {
          text: '参考',
          items: [
            { text: '⌨️ 快捷键汇总', link: '/guide/shortcuts' },
            { text: '💡 使用技巧', link: '/guide/tips' },
            { text: '❓ 常见问题', link: '/guide/faq' },
          ]
        }
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
      message: '开源免费 | 安全可靠',
      copyright: 'Copyright © 2024 PixWit'
    },

    search: {
      provider: 'local',
      options: {
        locales: {
          root: {
            translations: {
              button: {
                buttonText: '搜索文档',
                buttonAriaLabel: '搜索文档'
              },
              modal: {
                noResultsText: '无法找到相关结果',
                resetButtonTitle: '清除查询条件',
                footer: {
                  selectText: '选择',
                  navigateText: '切换'
                }
              }
            }
          }
        }
      }
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/yun-cun-li/PixWit' }
    ]
  }
})

