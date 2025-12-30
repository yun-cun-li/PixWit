import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'PixWit',
  description: '截图 · 录屏 · 简易剪辑',
  lang: 'zh-CN',
  // 直接服务器部署使用根路径
  base: '/',
  
  // 启用简洁 URL（移除 .html 后缀）
  cleanUrls: true,
  
  appearance: 'dark', // 默认暗黑模式
  
  head: [
    ['link', { rel: 'icon', href: '/favicon.ico' }],
    
    // SEO Keywords
    ['meta', { 
      name: 'keywords', 
      content: 'PixWit,截图工具,录屏软件,视频剪辑,OCR文字识别,图片翻译,表格提取,免费截图,Windows截图,屏幕录制,GIF录制,视频编辑工具,免费录屏软件,截图标注,屏幕捕捉' 
    }],
    
    // Canonical URL
    ['link', { rel: 'canonical', href: 'https://pixwit.cn/' }],
    
    // Open Graph / Facebook
    ['meta', { property: 'og:type', content: 'website' }],
    ['meta', { property: 'og:site_name', content: 'PixWit' }],
    ['meta', { property: 'og:title', content: 'PixWit - 免费截图录屏工具 | 支持OCR、视频编辑' }],
    ['meta', { property: 'og:description', content: '免费开源的截图、录屏、视频编辑工具。支持OCR文字识别、图片翻译、表格提取、AI图片解读等功能。绿色免安装，功能强大。' }],
    ['meta', { property: 'og:url', content: 'https://pixwit.cn' }],
    ['meta', { property: 'og:image', content: 'https://pixwit.cn/logo.svg' }],
    ['meta', { property: 'og:locale', content: 'zh_CN' }],
    
    // Twitter Card
    ['meta', { name: 'twitter:card', content: 'summary_large_image' }],
    ['meta', { name: 'twitter:site', content: '@PixWit' }],
    ['meta', { name: 'twitter:title', content: 'PixWit - 免费截图录屏工具' }],
    ['meta', { name: 'twitter:description', content: '免费开源的截图、录屏、视频编辑工具。支持OCR文字识别、图片翻译、表格提取等功能。' }],
    ['meta', { name: 'twitter:image', content: 'https://pixwit.cn/logo.svg' }],
    
    // Additional SEO
    ['meta', { name: 'author', content: 'PixWit Team' }],
    ['meta', { name: 'robots', content: 'index, follow' }],
    
    // Schema.org Structured Data (JSON-LD)
    ['script', { type: 'application/ld+json' }, JSON.stringify({
      '@context': 'https://schema.org',
      '@type': 'SoftwareApplication',
      'name': 'PixWit',
      'applicationCategory': 'UtilitiesApplication',
      'applicationSubCategory': 'Screenshot and Screen Recording Software',
      'operatingSystem': 'Windows 10, Windows 11',
      'offers': {
        '@type': 'Offer',
        'price': '0',
        'priceCurrency': 'CNY',
        'availability': 'https://schema.org/InStock'
      },
      'aggregateRating': {
        '@type': 'AggregateRating',
        'ratingValue': '4.8',
        'ratingCount': '150',
        'bestRating': '5',
        'worstRating': '1'
      },
      'description': '免费开源的截图、录屏、视频编辑工具。支持OCR文字识别、图片翻译、表格提取、AI图片解读等功能。绿色免安装，功能强大。',
      'softwareVersion': '1.2.5',
      'datePublished': '2024-01-01',
      'dateModified': '2025-01-01',
      'author': {
        '@type': 'Organization',
        'name': 'PixWit Team'
      },
      'downloadUrl': 'https://pixwit.cn/download',
      'screenshot': 'https://pixwit.cn/guide/截图主界面_PixWit_20251214_221948.webp',
      'featureList': [
        '多屏截图',
        '区域录制',
        '视频编辑',
        'OCR文字识别',
        '图片翻译',
        '表格提取',
        'AI智能解读',
        '历史记录管理'
      ],
      'requirements': 'Windows 10 (1809+) / Windows 11',
      'softwareHelp': {
        '@type': 'CreativeWork',
        'url': 'https://pixwit.cn/guide/'
      },
      'maintainer': {
        '@type': 'Organization',
        'name': 'PixWit Team',
        'url': 'https://pixwit.cn'
      },
      'license': 'https://www.gnu.org/licenses/gpl-3.0.html'
    })],
    
    // Organization Schema
    ['script', { type: 'application/ld+json' }, JSON.stringify({
      '@context': 'https://schema.org',
      '@type': 'Organization',
      'name': 'PixWit',
      'url': 'https://pixwit.cn',
      'logo': 'https://pixwit.cn/logo.svg',
      'sameAs': [
        'https://github.com/yun-cun-li/PixWit',
        'https://gitee.com/bjs1999/PixWit'
      ],
      'contactPoint': {
        '@type': 'ContactPoint',
        'contactType': 'Customer Support',
        'availableLanguage': ['Chinese']
      }
    })],
    
    // WebSite Schema for search box
    ['script', { type: 'application/ld+json' }, JSON.stringify({
      '@context': 'https://schema.org',
      '@type': 'WebSite',
      'name': 'PixWit',
      'url': 'https://pixwit.cn',
      'description': '免费开源的截图、录屏、视频编辑工具',
      'inLanguage': 'zh-CN',
      'publisher': {
        '@type': 'Organization',
        'name': 'PixWit Team'
      }
    })],
  ],

  sitemap: {
    hostname: 'https://pixwit.cn',
    transformItems(items) {
      const now = new Date().toISOString()
      
      return items.map((item) => {
        const url = item.url
        
        // 首页 - 最高优先级，每天更新
        if (url === '/') {
          return {
            ...item,
            lastmod: now,
            changefreq: 'daily',
            priority: 1.0
          }
        }
        
        // 下载页 - 高优先级，每周更新
        if (url === '/download.html') {
          return {
            ...item,
            lastmod: now,
            changefreq: 'weekly',
            priority: 0.9
          }
        }
        
        // 使用手册首页 - 高优先级
        if (url === '/guide/') {
          return {
            ...item,
            lastmod: now,
            changefreq: 'weekly',
            priority: 0.9
          }
        }
        
        // 核心功能页面 - 中高优先级
        if (url.includes('/guide/screenshot') || 
            url.includes('/guide/recording') || 
            url.includes('/guide/ocr') || 
            url.includes('/guide/translation') ||
            url.includes('/guide/video') ||
            url.includes('/guide/table_extraction')) {
          return {
            ...item,
            lastmod: now,
            changefreq: 'monthly',
            priority: 0.8
          }
        }
        
        // 其他指南页面 - 中等优先级
        if (url.includes('/guide/')) {
          return {
            ...item,
            lastmod: now,
            changefreq: 'monthly',
            priority: 0.7
          }
        }
        
        // 默认 - 低优先级
        return {
          ...item,
          lastmod: now,
          changefreq: 'monthly',
          priority: 0.5
        }
      })
    }
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

