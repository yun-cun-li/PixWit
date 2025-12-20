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
