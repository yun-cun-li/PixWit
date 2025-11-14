# PixWit 官方网站

基于 VitePress 构建的 PixWit 官方网站，包含产品介绍、使用文档和下载页面。

## 🚀 快速开始

### 安装依赖

```bash
npm install
```

### 本地开发

```bash
npm run dev
```

访问 http://localhost:5173 查看网站。

### 构建生产版本

```bash
npm run build
```

构建产物在 `docs/.vitepress/dist` 目录。

### 预览生产版本

```bash
npm run preview
```

## 📁 项目结构

```
pixwit-web/
├── docs/                          # VitePress 文档目录
│   ├── .vitepress/               # VitePress 配置
│   │   ├── config.ts            # 网站配置
│   │   └── theme/               # 自定义主题
│   │       ├── index.ts         # 主题入口
│   │       ├── style.css        # 自定义样式
│   │       └── components/      # Vue 组件
│   │           ├── Hero.vue     # 首页英雄区
│   │           ├── Features.vue # 特性展示
│   │           └── Download.vue # 下载组件
│   ├── index.md                 # 首页
│   ├── download.md              # 下载页面
│   ├── guide/                   # 使用手册
│   │   ├── index.md            # 手册首页
│   │   ├── installation.md     # 安装指南
│   │   ├── screenshot.md       # 截图功能
│   │   ├── recording.md        # 录屏功能
│   │   ├── image.md           # 图片处理
│   │   ├── video.md           # 视频处理
│   │   ├── ocr.md             # OCR 识别
│   │   ├── shortcuts.md       # 快捷键
│   │   ├── tips.md            # 使用技巧
│   │   └── faq.md             # 常见问题
│   └── public/                 # 静态资源
├── package.json                # 项目配置
└── README_WEB.md              # 本文件
```

## 🎨 自定义

### 修改主题颜色

编辑 `docs/.vitepress/theme/style.css`：

```css
:root {
  --vp-c-brand-1: #3eaf7c;  /* 主色调 */
  --vp-c-brand-2: #42b883;  /* 辅助色 */
}
```

### 修改导航栏

编辑 `docs/.vitepress/config.ts` 中的 `themeConfig.nav`。

### 修改侧边栏

编辑 `docs/.vitepress/config.ts` 中的 `themeConfig.sidebar`。

### 添加自定义组件

1. 在 `docs/.vitepress/theme/components/` 创建 Vue 组件
2. 在 `docs/.vitepress/theme/index.ts` 中注册组件
3. 在 Markdown 文件中使用 `<ComponentName />`

## 📝 内容更新

### 更新文档

直接编辑 `docs/guide/` 目录下的 Markdown 文件。VitePress 会自动热更新。

### 添加新页面

1. 在相应目录创建 `.md` 文件
2. 在 `config.ts` 中添加到导航或侧边栏
3. 编写内容

### 更新首页

编辑 `docs/index.md` 和相关 Vue 组件。

## 🚀 部署

### 部署到 GitHub Pages

1. 修改 `docs/.vitepress/config.ts` 中的 `base` 配置：
   ```ts
   export default defineConfig({
     base: '/pixwit-web/',  // 仓库名
     // ...
   })
   ```

2. 构建：
   ```bash
   npm run build
   ```

3. 推送 `docs/.vitepress/dist` 目录到 `gh-pages` 分支

### 部署到 Vercel

1. 导入 GitHub 仓库
2. 构建命令：`npm run build`
3. 输出目录：`docs/.vitepress/dist`
4. 自动部署

### 部署到 Netlify

1. 导入 GitHub 仓库
2. 构建命令：`npm run build`
3. 发布目录：`docs/.vitepress/dist`
4. 自动部署

### 部署到服务器

```bash
# 构建
npm run build

# 将 docs/.vitepress/dist 目录上传到服务器
# 配置 Nginx/Apache 指向该目录
```

## 🛠️ 技术栈

- **VitePress** - 静态站点生成器
- **Vue 3** - 前端框架
- **TypeScript** - 类型支持
- **Vite** - 构建工具

## 📖 相关链接

- [VitePress 文档](https://vitepress.dev/)
- [Vue 3 文档](https://vuejs.org/)
- [PixWit 主仓库](https://github.com/yun-cun-li/PixWit)

## 📞 联系我们

- QQ 群：133125470
- GitHub：https://github.com/yun-cun-li/PixWit
- Gitee：https://gitee.com/bjs1999/PixWit

---

**License:** GPL-3.0

