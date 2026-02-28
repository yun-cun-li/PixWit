#!/bin/bash

# PixWit 网站启动脚本
# 用于在服务器上快速启动HTTP服务测试网站

echo "🚀 启动 PixWit 网站服务..."

# 检查构建产物是否存在
if [ ! -d "docs/.vitepress/dist" ]; then
    echo "❌ 构建产物不存在，请先运行: npm run build"
    exit 1
fi

# 进入构建目录
cd docs/.vitepress/dist

echo "📁 服务目录: $(pwd)"
echo "🌐 服务地址: http://8.148.147.74:4173/"
echo "📝 按 Ctrl+C 停止服务"
echo ""

# 使用 Python 启动HTTP服务器（推荐）
if command -v python3 &> /dev/null; then
    echo "🐍 使用 Python3 启动服务..."
    python3 -m http.server 4173
elif command -v python &> /dev/null && python --version 2>&1 | grep -q "Python 2"; then
    echo "🐍 使用 Python2 启动服务..."
    python -m SimpleHTTPServer 4173
else
    echo "❌ 未找到 Python，请安装 Python 或使用其他方法启动服务"
    echo "安装 Python: apt-get install python3"
    exit 1
fi
