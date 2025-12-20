# PixWit Nginx SSL 部署文档

本文档详细说明如何使用域名证书在服务器上部署 PixWit 网站的 HTTPS 访问。

## 📋 前置条件

- ✅ 已有域名证书文件（pixwit.cn.key 和 pixwit.cn.pem）
- ✅ 已安装 Nginx（当前版本：nginx/1.20.1）
- ✅ 已有 Nginx SSL 配置文件（nginx_ssl.conf）
- ✅ 域名已解析到服务器 IP

## 🚀 部署步骤

### 第 1 步：复制证书文件到 Nginx SSL 目录

```bash
# 复制证书文件
cp /root/pro/PixWit/21964674_pixwit.cn_nginx/pixwit.cn.key /etc/nginx/ssl/
cp /root/pro/PixWit/21964674_pixwit.cn_nginx/pixwit.cn.pem /etc/nginx/ssl/

# 验证证书文件已复制
ls -la /etc/nginx/ssl/
```

预期输出应包含：
```
pixwit.cn.key
pixwit.cn.pem
```

### 第 2 步：设置证书文件权限（安全性）

```bash
# 设置证书文件权限，确保安全
chmod 600 /etc/nginx/ssl/pixwit.cn.key
chmod 644 /etc/nginx/ssl/pixwit.cn.pem

# 验证权限设置
ls -la /etc/nginx/ssl/pixwit.cn.*
```

### 第 3 步：构建网站静态文件

```bash
# 进入项目目录
cd /root/pro/PixWit

# 安装依赖（如果还没有安装）
npm install

# 构建生产版本
npm run build
```

构建完成后，静态文件将位于 `docs/.vitepress/dist/` 目录。

### 第 4 步：创建网站根目录并复制文件

```bash
# 创建网站根目录
mkdir -p /var/www/pixwit

# 复制构建后的文件到网站根目录
cp -r /root/pro/PixWit/docs/.vitepress/dist/* /var/www/pixwit/

# 设置目录权限
chmod -R 755 /var/www/pixwit
chown -R nginx:nginx /var/www/pixwit  # 或使用 www-data:www-data，取决于系统
```

### 第 5 步：复制 Nginx 配置文件

```bash
# 复制 SSL 配置文件到 Nginx 配置目录
cp /root/pro/PixWit/nginx_ssl.conf /etc/nginx/conf.d/pixwit.conf

# 或者如果使用 sites-available/sites-enabled 结构：
# cp /root/pro/PixWit/nginx_ssl.conf /etc/nginx/sites-available/pixwit.conf
# ln -s /etc/nginx/sites-available/pixwit.conf /etc/nginx/sites-enabled/
```

### 第 6 步：测试 Nginx 配置

```bash
# 测试配置文件语法是否正确
nginx -t
```

预期输出：
```
nginx: the configuration file /etc/nginx/nginx.conf syntax is ok
nginx: configuration file /etc/nginx/nginx.conf test is successful
```

### 第 7 步：重启 Nginx 服务

```bash
# 重启 Nginx 使配置生效
systemctl restart nginx

# 或使用
# service nginx restart

# 检查 Nginx 运行状态
systemctl status nginx
```

### 第 8 步：配置防火墙开放端口

```bash
# 开放 HTTP (80) 和 HTTPS (443) 端口
firewall-cmd --permanent --add-service=http
firewall-cmd --permanent --add-service=https
firewall-cmd --reload

# 或者使用阿里云/腾讯云安全组规则
# 在云控制台添加以下规则：
# - 入站规则：TCP 80 端口，源：0.0.0.0/0
# - 入站规则：TCP 443 端口，源：0.0.0.0/0
```

### 第 9 步：验证部署

```bash
# 本地测试 HTTP 重定向
curl -I http://pixwit.cn

# 本地测试 HTTPS
curl -I https://pixwit.cn

# 查看 Nginx 访问日志
tail -f /var/log/nginx/pixwit_access.log

# 查看 Nginx 错误日志
tail -f /var/log/nginx/pixwit_error.log
```

在浏览器访问：
- http://pixwit.cn （应自动重定向到 HTTPS）
- https://pixwit.cn （应正常访问并显示安全锁）
- https://www.pixwit.cn （也应正常访问）

## 📝 配置说明

当前 nginx_ssl.conf 配置文件已包含以下特性：

### SSL 配置
- ✅ 自动将 HTTP 重定向到 HTTPS
- ✅ 支持 TLS 1.2 和 TLS 1.3
- ✅ 安全的加密套件配置
- ✅ SSL 会话缓存优化
- ✅ HSTS 安全头（强制 HTTPS）

### 性能优化
- ✅ Gzip 压缩启用
- ✅ 静态资源缓存（1年）
- ✅ HTML 文件缓存（1小时）
- ✅ HTTP/2 支持

### 安全配置
- ✅ 安全响应头配置
- ✅ XSS 保护
- ✅ 点击劫持保护
- ✅ 内容类型嗅探保护
- ✅ 隐藏 Nginx 版本信息

### 路由配置
- ✅ 支持 VitePress SPA 路由
- ✅ 404 页面正确处理

## 🔧 常见问题排查

### 问题 1：Nginx 配置测试失败

**错误信息**：`nginx: [emerg] cannot load certificate`

**解决方案**：
```bash
# 检查证书文件路径是否正确
ls -la /etc/nginx/ssl/pixwit.cn.pem
ls -la /etc/nginx/ssl/pixwit.cn.key

# 检查文件权限
chmod 600 /etc/nginx/ssl/pixwit.cn.key
chmod 644 /etc/nginx/ssl/pixwit.cn.pem
```

### 问题 2：端口被占用

**错误信息**：`nginx: [emerg] bind() to 0.0.0.0:443 failed`

**解决方案**：
```bash
# 查看端口占用情况
netstat -tulnp | grep :443
lsof -i :443

# 停止占用端口的进程
kill -9 [进程PID]
```

### 问题 3：防火墙阻止访问

**症状**：外网无法访问网站

**解决方案**：
```bash
# 检查防火墙状态
firewall-cmd --list-all

# 开放端口
firewall-cmd --permanent --add-port=80/tcp
firewall-cmd --permanent --add-port=443/tcp
firewall-cmd --reload

# 同时检查云服务商的安全组设置
```

### 问题 4：SSL 证书错误

**症状**：浏览器显示"您的连接不是私密连接"

**解决方案**：
```bash
# 验证证书是否正确
openssl x509 -in /etc/nginx/ssl/pixwit.cn.pem -text -noout

# 检查证书有效期
openssl x509 -in /etc/nginx/ssl/pixwit.cn.pem -noout -dates

# 验证证书和私钥是否匹配
openssl x509 -noout -modulus -in /etc/nginx/ssl/pixwit.cn.pem | openssl md5
openssl rsa -noout -modulus -in /etc/nginx/ssl/pixwit.cn.key | openssl md5
# 两个 MD5 值应该相同
```

### 问题 5：页面显示 502 Bad Gateway

**原因**：网站文件不存在或权限问题

**解决方案**：
```bash
# 检查网站根目录
ls -la /var/www/pixwit/

# 确保有 index.html 文件
ls -la /var/www/pixwit/index.html

# 设置正确权限
chmod -R 755 /var/www/pixwit
chown -R nginx:nginx /var/www/pixwit
```

### 问题 6：静态资源 404

**原因**：文件路径配置问题

**解决方案**：
```bash
# 检查 VitePress 构建输出
ls -la /root/pro/PixWit/docs/.vitepress/dist/

# 确保所有文件都已复制
cp -r /root/pro/PixWit/docs/.vitepress/dist/* /var/www/pixwit/
```

## 🔄 更新网站内容

当你修改了源代码（如 .vue、.md、.ts 文件）后，需要重新构建并部署才能让更改生效。

### 方法一：标准流程（推荐，带备份）

```bash
# 1. 进入项目目录
cd /root/pro/PixWit

# 2. 拉取最新代码（如果使用 Git）
git pull

# 3. 重新构建（根据最新的源代码生成静态文件）
npm run build

# 4. 备份当前版本（推荐，方便回滚）
mv /var/www/pixwit /var/www/pixwit.backup.$(date +%Y%m%d_%H%M%S)

# 5. 创建新目录并复制新构建的文件
mkdir -p /var/www/pixwit
cp -r /root/pro/PixWit/docs/.vitepress/dist/* /var/www/pixwit/

# 6. 设置权限
chmod -R 755 /var/www/pixwit
chown -R nginx:nginx /var/www/pixwit

# 7. 重新加载 Nginx（可选，静态文件通常不需要）
systemctl reload nginx

echo "✅ 更新完成！访问 https://pixwit.cn 查看效果"
```

### 方法二：快速更新（不备份）

适合频繁更新、测试阶段使用：

```bash
cd /root/pro/PixWit && \
npm run build && \
cp -r /root/pro/PixWit/docs/.vitepress/dist/* /var/www/pixwit/ && \
echo "✅ 更新完成！"
```

### 方法三：使用 rsync（推荐，更高效）

rsync 只会同步变化的文件，速度更快：

```bash
# 进入项目目录并构建
cd /root/pro/PixWit
npm run build

# 使用 rsync 同步文件（--delete 会删除目标目录中多余的文件）
rsync -av --delete /root/pro/PixWit/docs/.vitepress/dist/ /var/www/pixwit/

echo "✅ 使用 rsync 更新完成！"
```

### 方法四：创建自动化更新脚本（最方便，推荐）

创建一个更新脚本，以后只需运行一条命令：

```bash
# 创建更新脚本
cat > /root/update_pixwit.sh <<'EOF'
#!/bin/bash
echo "======================================"
echo "开始更新 PixWit 网站..."
echo "======================================"

cd /root/pro/PixWit

echo "📦 1. 构建项目..."
npm run build

if [ $? -eq 0 ]; then
    echo "✅ 构建成功"
    
    echo "📋 2. 备份当前版本..."
    if [ -d /var/www/pixwit ]; then
        BACKUP_DIR="/var/www/pixwit.backup.$(date +%Y%m%d_%H%M%S)"
        mv /var/www/pixwit $BACKUP_DIR
        echo "   备份已保存到: $BACKUP_DIR"
    fi
    
    echo "📂 3. 部署新文件..."
    mkdir -p /var/www/pixwit
    # 使用 yes 命令自动确认覆盖，并忽略错误输出
    yes | cp -r /root/pro/PixWit/docs/.vitepress/dist/* /var/www/pixwit/ 2>/dev/null || true
    
    echo "🔐 4. 设置权限..."
    chmod -R 755 /var/www/pixwit
    chown -R nginx:nginx /var/www/pixwit
    
    echo "======================================"
    echo "✅ 更新完成！"
    echo "======================================"
    echo "访问 https://pixwit.cn 查看效果"
    echo "提示：浏览器可能需要强制刷新（Ctrl+Shift+R）"
else
    echo "❌ 构建失败，请检查错误信息"
    exit 1
fi
EOF

# 添加执行权限
chmod +x /root/update_pixwit.sh

echo "✅ 更新脚本已创建：/root/update_pixwit.sh"
```

以后每次修改源代码后，只需运行：
```bash
/root/update_pixwit.sh
```

### ⚠️ 重要提示：配置文件位置

如果你需要修改下载配置（版本号、下载链接等），请编辑：
```bash
/root/pro/PixWit/docs/public/latest.json
```

修改后重新构建部署即可。**不要直接修改 `/var/www/pixwit/latest.json`**，因为下次部署时会被覆盖。

### 清理旧备份

如果备份文件太多，可以清理：

```bash
# 查看所有备份
ls -lh /var/www/ | grep pixwit.backup

# 删除 7 天前的备份
find /var/www/ -name "pixwit.backup.*" -mtime +7 -exec rm -rf {} \;

# 或者只保留最近 3 个备份
ls -dt /var/www/pixwit.backup.* | tail -n +4 | xargs rm -rf
```

### 🌐 浏览器缓存处理

更新后如果浏览器还显示旧内容，需要清除缓存：

#### 方法 1：强制刷新（最常用）
- **Windows/Linux**: `Ctrl + Shift + R` 或 `Ctrl + F5`
- **Mac**: `Cmd + Shift + R` 或 `Cmd + Option + R`

#### 方法 2：开发者工具清除缓存
1. 打开开发者工具（F12）
2. 右键点击刷新按钮
3. 选择"清空缓存并硬性重新加载"

#### 方法 3：浏览器设置清除缓存
- Chrome: 设置 → 隐私和安全 → 清除浏览数据
- Firefox: 选项 → 隐私与安全 → Cookie 和网站数据

#### 验证文件是否已更新

```bash
# 查看网站文件的最新修改时间
ls -lht /var/www/pixwit/ | head -20

# 在构建输出中搜索特定内容，确认是否是最新版本
grep -r "AI 分析" /var/www/pixwit/assets/*.js

# 查看某个文件的最后修改时间
stat /var/www/pixwit/index.html
```

## 📊 监控和维护

### 查看访问日志
```bash
# 实时查看访问日志
tail -f /var/log/nginx/pixwit_access.log

# 查看最近的错误
tail -n 100 /var/log/nginx/pixwit_error.log

# 统计访问量
awk '{print $1}' /var/log/nginx/pixwit_access.log | sort | uniq -c | sort -rn | head -20
```

### 性能测试
```bash
# 使用 curl 测试响应时间
curl -o /dev/null -s -w "Time: %{time_total}s\n" https://pixwit.cn

# 测试 HTTPS 配置
curl -I -v https://pixwit.cn
```

### SSL 证书续期提醒

SSL 证书通常有效期为 1 年，需要定期续期：

```bash
# 查看证书过期时间
openssl x509 -in /etc/nginx/ssl/pixwit.cn.pem -noout -enddate

# 设置提醒（在证书过期前 30 天）
# 可以使用 cron 任务或系统监控工具
```

证书到期前需要：
1. 在域名提供商处重新申请或续期证书
2. 下载新证书文件
3. 替换 `/etc/nginx/ssl/` 目录下的证书文件
4. 重启 Nginx：`systemctl reload nginx`

## 🛡️ 安全建议

1. **定期更新 Nginx**
   ```bash
   yum update nginx  # CentOS/RHEL
   apt update && apt upgrade nginx  # Ubuntu/Debian
   ```

2. **配置自动备份**
   ```bash
   # 创建备份脚本
   cat > /root/backup_pixwit.sh <<'EOF'
   #!/bin/bash
   BACKUP_DIR="/root/backups"
   DATE=$(date +%Y%m%d_%H%M%S)
   mkdir -p $BACKUP_DIR
   tar -czf $BACKUP_DIR/pixwit_$DATE.tar.gz /var/www/pixwit
   # 保留最近 7 天的备份
   find $BACKUP_DIR -name "pixwit_*.tar.gz" -mtime +7 -delete
   EOF
   
   chmod +x /root/backup_pixwit.sh
   
   # 添加到 crontab（每天凌晨 2 点备份）
   echo "0 2 * * * /root/backup_pixwit.sh" | crontab -
   ```

3. **启用访问限制（可选）**
   
   如需限制某些 IP 访问，可在 nginx 配置中添加：
   ```nginx
   # 允许特定 IP
   allow 192.168.1.0/24;
   # 拒绝其他所有 IP
   deny all;
   ```

## 📞 技术支持

- **项目仓库**: https://github.com/yun-cun-li/PixWit
- **QQ 群**: 133125470

---

**文档创建时间**: 2024-12-14
**适用版本**: PixWit v1.0

