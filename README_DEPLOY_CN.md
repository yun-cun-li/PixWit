# PixWit 官方网站部署指南

本文档介绍如何启动、构建和部署 PixWit 官方网站。

## 📋 环境要求

- **Node.js**: 16.0.0 或更高版本
- **npm**: 7.0.0 或更高版本（或使用 yarn/pnpm）
- **操作系统**: Windows / macOS / Linux

## 🚀 本地开发

### 1. 安装依赖

```bash
# 进入项目目录
cd /root/pro/PixWit

# 安装依赖
npm install
```

### 2. 启动开发服务器

```bash
# 启动开发服务器
npm run dev
```

访问 `http://localhost:5173` 查看网站。

### 3. 开发说明

- 开发模式下，网站根路径为 `/`
- 支持热重载，修改文件后自动刷新
- 所有 Markdown 文件和 Vue 组件都会自动更新

## 🏗️ 构建生产版本

### 基本构建

```bash
# 构建生产版本
npm run build
```

构建完成后，静态文件位于 `docs/.vitepress/dist/` 目录。

### 预览构建结果

```bash
# 预览生产版本
npm run preview
```

访问 `http://localhost:4173` 预览构建后的网站。

## 🌐 部署方式

### 方法一：GitHub Pages（推荐）

#### 1. 准备工作

确保项目已推送到 GitHub 仓库，仓库名为 `PixWit`。

#### 2. 构建并部署

```bash
# 构建生产版本
npm run build

# 进入构建目录
cd docs/.vitepress/dist

# 初始化 git 仓库（如果还没有）
git init
git add -A
git commit -m "deploy"

# 推送到 gh-pages 分支
git push -f git@github.com:yun-cun-li/PixWit.git main:gh-pages
```

#### 3. 访问网站

部署完成后，访问 `https://yun-cun-li.github.io/PixWit/` 查看网站。

### 方法二：Vercel（自动部署）

#### 1. 导入项目

1. 访问 [Vercel](https://vercel.com)
2. 点击 "Import Project"
3. 连接 GitHub 仓库 `yun-cun-li/PixWit`

#### 2. 配置部署

| 配置项 | 值 |
|--------|-----|
| Framework Preset | VitePress |
| Root Directory | `/` |
| Build Command | `npm run build` |
| Output Directory | `docs/.vitepress/dist` |

#### 3. 部署完成

Vercel 会自动构建并部署，每次推送代码都会触发自动部署。

### 方法三：Netlify（自动部署）

#### 1. 导入项目

1. 访问 [Netlify](https://netlify.com)
2. 点击 "Add new site" → "Import an existing project"
3. 连接 GitHub 仓库

#### 2. 配置构建

| 配置项 | 值 |
|--------|-----|
| Base directory | `/` |
| Build command | `npm run build` |
| Publish directory | `docs/.vitepress/dist` |

#### 3. 部署完成

Netlify 支持自动部署和自定义域名。

### 方法四：传统服务器部署

#### 1. 构建静态文件

```bash
npm run build
```

#### 2. 上传文件

将 `docs/.vitepress/dist/` 目录下的所有文件上传到服务器。

#### 3. 配置服务器

**Nginx 配置示例：**

```nginx
server {
    listen 80;
    server_name your-domain.com;
    root /path/to/dist;
    index index.html;

    location / {
        try_files $uri $uri/ /index.html;
    }

    # 缓存静态资源
    location ~* \.(js|css|png|jpg|jpeg|gif|ico|svg)$ {
        expires 1y;
        add_header Cache-Control "public, immutable";
    }
}
```

**Apache 配置示例：**

```apache
<VirtualHost *:80>
    ServerName your-domain.com
    DocumentRoot /path/to/dist

    <Directory "/path/to/dist">
        AllowOverride All
        Require all granted
    </Directory>

    RewriteEngine On
    RewriteRule ^index\.html$ - [L]
    RewriteCond %{REQUEST_FILENAME} !-f
    RewriteCond %{REQUEST_FILENAME} !-d
    RewriteRule . /index.html [L]
</VirtualHost>
```

## 🔧 高级配置

### 自定义域名

#### GitHub Pages
1. 在仓库 Settings → Pages 中设置自定义域名
2. 在 DNS 提供商处添加 CNAME 记录

#### Vercel/Netlify
在控制台的域名设置中添加自定义域名。

### CDN 加速

推荐使用 CDN 服务加速静态资源分发：

- **Cloudflare**: 免费 CDN，支持自动压缩
- **阿里云 CDN**: 国内用户推荐
- **腾讯云 CDN**: 国内用户推荐

### SEO 优化

网站已配置基本的 SEO：

- HTML meta 标签
- 网站描述和关键词
- 结构化数据（可进一步优化）

## 🐛 故障排除

### 构建失败

**问题**: `npm run build` 失败

**解决方案**:
1. 确保 Node.js 版本 >= 16
2. 清除缓存：`rm -rf node_modules/.vite docs/.vitepress/cache`
3. 重新安装依赖：`npm install`

### 部署后样式丢失

**问题**: 部署后 CSS/JS 文件无法加载

**解决方案**:
1. 检查 `base` 配置是否正确
2. 确保所有资源路径使用相对路径
3. 检查 CDN 或服务器配置

### 页面无法访问

**问题**: 404 错误

**解决方案**:
1. 检查服务器是否正确配置了 SPA 路由重写
2. 确保 `index.html` 能正确返回
3. 检查防火墙和安全组设置

### 开发服务器无法启动

**问题**: 端口被占用

**解决方案**:
```bash
# 使用不同端口
npm run dev -- --port 3000
```

## 📊 性能优化

### 构建优化

- 启用压缩：VitePress 默认启用 Gzip 压缩
- 代码分割：自动按页面分割代码
- 图片优化：使用 WebP 格式，启用懒加载

### 监控建议

- 使用 Google Analytics 或百度统计监控访问
- 使用 Google Search Console 监控 SEO
- 定期检查页面性能（Lighthouse）

## 📞 获取帮助

- **项目仓库**: [GitHub](https://github.com/yun-cun-li/PixWit)
- **问题反馈**: [提交 Issue](https://github.com/yun-cun-li/PixWit/issues)
- **QQ 群**: 133125470

---

**最后更新**: 2024-12-14
