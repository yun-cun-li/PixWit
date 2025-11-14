# 📥 下载 PixWit

::: tip 当前版本
PixWit v0.2.x - 持续更新中
:::

<Download />

## 📦 版本说明

### 便携版 vs 安装版

| 特性 | 便携版 💚 | 安装版 |
|------|---------|--------|
| 安装方式 | 解压即用 | 需要安装 |
| 管理员权限 | ❌ 不需要 | ✅ 需要 |
| 开机自启 | ⚠️ 手动设置 | ✅ 自动支持 |
| 系统集成 | 基础 | 完整 |
| 杀软拦截 | 较少 | 可能较多 |
| 适用场景 | 快速体验、临时使用 | 长期使用 |

::: info 推荐选择
**大多数用户推荐下载便携版**，无需处理杀毒软件问题，使用更方便。
:::

## 🔍 文件校验

为确保文件完整性，您可以验证 SHA256 校验值：

```bash
# Windows PowerShell
Get-FileHash PixWit-Portable-v0.2.x.zip -Algorithm SHA256

# 命令提示符（需要安装 certutil）
certutil -hashfile PixWit-Portable-v0.2.x.zip SHA256
```

## 💻 系统要求

- **操作系统**：Windows 10 / 11（不支持 Windows 7）
- **架构**：x64 (64位)
- **内存**：建议 4GB 以上
- **磁盘空间**：约 100MB

::: warning Windows 7 不支持
PixWit 基于 Qt 6 框架开发，仅支持 Windows 10 (1809+) 和 Windows 11。
如果您使用 Windows 7，建议升级系统或使用其他工具。
:::

## 🚀 安装步骤

### 便携版安装

1. 下载 `PixWit-Portable-vX.X.X.zip`
2. 解压到任意文件夹（建议：`C:\Program Files\PixWit` 或 `D:\Software\PixWit`）
3. 运行 `运行PixWit.bat` 或直接运行 `PixWit.exe`
4. 程序会最小化到系统托盘，点击托盘图标查看菜单

::: tip 提示
便携版的所有配置和历史记录都保存在 `%APPDATA%\PixWit` 目录。
:::

### 安装版安装

1. 下载 `PixWit-Setup-vX.X.X.exe`
2. 右键选择"以管理员身份运行"
3. 按照安装向导提示完成安装
4. 安装完成后自动启动

::: warning 杀毒软件拦截
如遇 Windows Defender 或其他杀毒软件拦截：
- 点击"更多信息" → "仍要运行"
- 或添加到白名单
:::

## 🔄 更新日志

### v0.2.x (最新)

**新功能**
- ✨ 视频剪辑：头尾裁剪 + 删除中间段
- ✨ 专业时间轴：缩略图 + 音频波形
- ✨ GIF 转换：优化调色板生成
- ✨ 智能音频检测

**改进**
- 🎨 深色主题优化
- ⚡ 性能提升
- 🐛 修复已知问题

[查看完整更新日志 →](https://github.com/yun-cun-li/PixWit/releases)

## ❓ 常见问题

### 下载问题

**Q: 下载速度慢怎么办？**
- GitHub 下载慢：可尝试 Gitee 镜像
- 使用下载工具（如 IDM、迅雷）加速

**Q: 浏览器提示"不安全"？**
- 这是因为未购买代码签名证书导致的误报
- 软件完全开源，可在 GitHub 查看所有代码
- 点击"保留"或"仍要下载"继续

### 安装问题

**Q: 双击无反应？**
- 检查是否被杀毒软件拦截
- 确认系统版本（需 Windows 10+）
- 尝试以管理员身份运行

**Q: 启动后崩溃？**
- 确认安装了 VC++ Redistributable
- 检查系统是否为 Windows 10 1809 或更高版本
- 查看错误日志：`%APPDATA%\PixWit\logs\`

**Q: 如何卸载？**
- 便携版：直接删除文件夹即可
- 安装版：控制面板 → 程序和功能 → 卸载

## 📞 获取支持

如果您在下载或安装过程中遇到问题：

- 📖 查看 [使用手册](./guide/)
- 💬 加入 QQ 群：**133125470**
- 🐛 提交 Issue：[GitHub](https://github.com/yun-cun-li/PixWit/issues) / [Gitee](https://gitee.com/bjs1999/PixWit/issues)

---

**感谢下载 PixWit！** 🎉

