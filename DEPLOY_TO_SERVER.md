# 🚀 PixWit 网站部署到服务器 http://8.148.147.74/

## 📋 部署概述

将 PixWit 官方网站直接部署到你的服务器 `http://8.148.147.74/` 的根目录。

## 🔧 步骤一：本地构建

### 1. 安装依赖（如果还没安装）

```bash
cd /root/pro/PixWit
npm install
```

### 2. 构建生产版本

```bash
npm run build
```

构建完成后，静态文件位于 `docs/.vitepress/dist/` 目录。

### 3. 验证构建结果

```bash
npm run preview
```

访问 `http://localhost:4173` 确认网站正常工作。

## 📤 步骤二：上传到服务器

### 方法一：使用 SCP 上传（推荐）

```bash
# 从本地上传到服务器（假设服务器IP为8.148.147.74，用户名为root）
scp -r docs/.vitepress/dist/* root@8.148.147.74:/var/www/html/
```

### 方法二：使用 rsync 上传

```bash
# 安装 rsync（如果还没安装）
# Ubuntu/Debian:
sudo apt-get install rsync

# CentOS/RHEL:
sudo yum install rsync

# 上传文件
rsync -avz --delete docs/.vitepress/dist/ root@8.148.147.74:/var/www/html/
```

### 方法三：手动上传

1. 使用 FTP 客户端（如 FileZilla）连接到服务器
2. 上传 `docs/.vitepress/dist/` 目录下的所有文件到服务器的 `/var/www/html/` 目录

## 🌐 步骤三：配置服务器

### 如果服务器使用 Nginx

创建或修改 Nginx 配置文件：

```bash
# 连接到服务器
ssh root@8.148.147.74

# 编辑 Nginx 配置（假设配置文件路径为 /etc/nginx/sites-available/pixwit）
sudo nano /etc/nginx/sites-available/pixwit
```

**Nginx 配置内容：**

```nginx
server {
    listen 80;
    server_name 8.148.147.74;

    root /var/www/html;
    index index.html;

    # 启用 gzip 压缩
    gzip on;
    gzip_types text/plain text/css application/json application/javascript text/xml application/xml application/xml+rss text/javascript;

    # 处理 Vue Router 的 history 模式
    location / {
        try_files $uri $uri/ /index.html;
    }

    # 缓存静态资源
    location ~* \.(js|css|png|jpg|jpeg|gif|ico|svg|woff|woff2|ttf|eot)$ {
        expires 1y;
        add_header Cache-Control "public, immutable";
        access_log off;
    }

    # 安全头
    add_header X-Frame-Options "SAMEORIGIN" always;
    add_header X-XSS-Protection "1; mode=block" always;
    add_header X-Content-Type-Options "nosniff" always;
    add_header Referrer-Policy "no-referrer-when-downgrade" always;
    add_header Content-Security-Policy "default-src 'self' http: https: data: blob: 'unsafe-inline'" always;

    # 日志
    access_log /var/log/nginx/pixwit_access.log;
    error_log /var/log/nginx/pixwit_error.log;
}
```

**启用配置并重启 Nginx：**

```bash
# 创建符号链接（如果使用 sites-available/sites-enabled 结构）
sudo ln -s /etc/nginx/sites-available/pixwit /etc/nginx/sites-enabled/

# 测试配置
sudo nginx -t

# 重启 Nginx
sudo systemctl restart nginx
```

### 如果服务器使用 Apache

创建或修改 Apache 虚拟主机配置：

```bash
sudo nano /etc/apache2/sites-available/pixwit.conf
```

**Apache 配置内容：**

```apache
<VirtualHost *:80>
    ServerName 8.148.147.74
    DocumentRoot /var/www/html

    <Directory "/var/www/html">
        AllowOverride All
        Require all granted
        Options -Indexes +FollowSymLinks

        # 启用压缩
        <IfModule mod_deflate.c>
            AddOutputFilterByType DEFLATE text/plain
            AddOutputFilterByType DEFLATE text/html
            AddOutputFilterByType DEFLATE text/xml
            AddOutputFilterByType DEFLATE text/css
            AddOutputFilterByType DEFLATE application/xml
            AddOutputFilterByType DEFLATE application/xhtml+xml
            AddOutputFilterByType DEFLATE application/rss+xml
            AddOutputFilterByType DEFLATE application/javascript
            AddOutputFilterByType DEFLATE application/x-javascript
        </IfModule>

        # 缓存静态资源
        <IfModule mod_expires.c>
            ExpiresActive On
            ExpiresByType image/jpg "access plus 1 year"
            ExpiresByType image/jpeg "access plus 1 year"
            ExpiresByType image/gif "access plus 1 year"
            ExpiresByType image/png "access plus 1 year"
            ExpiresByType text/css "access plus 1 month"
            ExpiresByType application/pdf "access plus 1 month"
            ExpiresByType application/javascript "access plus 1 year"
            ExpiresByType application/x-javascript "access plus 1 year"
            ExpiresByType application/x-shockwave-flash "access plus 1 month"
            ExpiresByType image/x-icon "access plus 1 year"
        </IfModule>
    </Directory>

    # 处理 SPA 路由
    RewriteEngine On
    RewriteBase /
    RewriteRule ^index\.html$ - [L]
    RewriteCond %{REQUEST_FILENAME} !-f
    RewriteCond %{REQUEST_FILENAME} !-d
    RewriteRule . /index.html [L]

    # 日志
    ErrorLog ${APACHE_LOG_DIR}/pixwit_error.log
    CustomLog ${APACHE_LOG_DIR}/pixwit_access.log combined
</VirtualHost>
```

**启用配置并重启 Apache：**

```bash
# 启用站点
sudo a2ensite pixwit

# 启用必要的模块
sudo a2enmod rewrite
sudo a2enmod deflate
sudo a2enmod expires

# 测试配置
sudo apache2ctl configtest

# 重启 Apache
sudo systemctl restart apache2
```

## 🔍 步骤四：验证部署

### 1. 检查网站访问

打开浏览器访问 `http://8.148.147.74/`，确认网站正常显示。

### 2. 检查所有页面

测试以下页面是否正常工作：
- 首页：`http://8.148.147.74/`
- 下载页：`http://8.148.147.74/download`
- 指南首页：`http://8.148.147.74/guide/`
- 其他子页面

### 3. 检查控制台错误

打开浏览器开发者工具（F12），查看 Console 是否有错误信息。

## 🔄 步骤五：更新部署

当需要更新网站时：

```bash
# 本地重新构建
cd /root/pro/PixWit
npm run build

# 上传更新文件
rsync -avz --delete docs/.vitepress/dist/ root@8.148.147.74:/var/www/html/

# 重启服务器（如果需要）
sudo systemctl restart nginx  # 或 apache2
```

## 🐛 故障排除

### 问题：网站无法访问

**检查步骤：**
1. 确认服务器防火墙开放了80端口
2. 检查 Nginx/Apache 服务状态：`sudo systemctl status nginx`
3. 查看日志文件

### 问题：页面显示不正常（样式丢失）

**解决方案：**
1. 确认所有文件都正确上传
2. 检查文件权限：`sudo chown -R www-data:www-data /var/www/html/`
3. 确认 base 路径配置正确（已修改为 '/'）

### 问题：路由不工作（404错误）

**解决方案：**
1. 确认 Nginx/Apache 配置中的重写规则正确
2. 检查是否启用了必要的模块（Apache 的 mod_rewrite）

## 📊 性能优化

### 启用 HTTPS（推荐）

```bash
# 使用 Let's Encrypt 免费证书
sudo apt install certbot python3-certbot-nginx  # Nginx
sudo certbot --nginx -d 8.148.147.74

# 或 Apache
sudo apt install certbot python3-certbot-apache
sudo certbot --apache -d 8.148.147.74
```

### 配置 CDN（可选）

考虑使用 CDN 服务加速静态资源：
- Cloudflare（免费）
- 阿里云 CDN
- 腾讯云 CDN

## 📞 获取帮助

如果遇到问题，请检查：
1. 服务器日志：`/var/log/nginx/` 或 `/var/log/apache2/`
2. 浏览器开发者工具的 Network 和 Console 标签
3. 确认所有文件权限正确

---

**部署完成！** 🎉

现在你可以通过 `http://8.148.147.74/` 访问 PixWit 官方网站了。
