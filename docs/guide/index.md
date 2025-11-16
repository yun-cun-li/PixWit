# PixWit 使用手册

欢迎使用 PixWit！本手册涵盖所有主要功能的使用说明。

**适用版本**：PixWit 1.1.x 及以上

---

## 📖 关于本手册

本使用手册将帮助您快速掌握 PixWit 的所有功能，包括：

- 📸 **截图功能** - 区域截图、全屏截图、编辑工具
- 🎬 **录屏功能** - 全屏录制、区域录制、录制标注
- 🖼️ **图片处理** - 打开编辑、格式转换
- 🎞️ **视频处理** - 视频裁剪、压缩、格式转换
- 🔍 **OCR 识别** - 文字识别、结果导出
- ⌨️ **快捷键** - 全局快捷键、自定义设置
- ❓ **常见问题** - 疑难解答

## 🚀 快速入门

### 首次使用

1. **启动程序**：运行 `PixWit.exe`，程序会最小化到系统托盘
2. **查看托盘图标**：在任务栏右下角找到 PixWit 图标
3. **右键菜单**：右键点击托盘图标，查看所有功能
4. **开始使用**：按 **F1** 开始第一次截图！

### 核心快捷键

| 快捷键 | 功能 |
|--------|------|
| **F1** | 区域截图 |
| **F2** | 全屏截图 |
| **F3** | 启动区域录制 |
| **F7** | 暂停/继续录制 |
| **F8** | 停止录制 |

*所有快捷键可在托盘菜单"快捷键设置"中自定义*

## 📚 功能导航

### 核心功能

<div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 1.5rem; margin: 2rem 0;">
  <a href="./screenshot" style="padding: 1.5rem; background: var(--vp-c-bg-soft); border-radius: 8px; text-decoration: none; border: 1px solid var(--vp-c-divider); transition: all 0.3s;">
    <div style="font-size: 2rem; margin-bottom: 0.5rem;">📸</div>
    <h3 style="margin: 0; color: var(--vp-c-text-1);">截图功能</h3>
    <p style="color: var(--vp-c-text-2); margin: 0.5rem 0 0 0;">区域选择、编辑标注、快速分享</p>
  </a>
  <a href="./recording" style="padding: 1.5rem; background: var(--vp-c-bg-soft); border-radius: 8px; text-decoration: none; border: 1px solid var(--vp-c-divider); transition: all 0.3s;">
    <div style="font-size: 2rem; margin-bottom: 0.5rem;">🎬</div>
    <h3 style="margin: 0; color: var(--vp-c-text-1);">录屏功能</h3>
    <p style="color: var(--vp-c-text-2); margin: 0.5rem 0 0 0;">全屏/区域录制、实时标注</p>
  </a>
  <a href="./image" style="padding: 1.5rem; background: var(--vp-c-bg-soft); border-radius: 8px; text-decoration: none; border: 1px solid var(--vp-c-divider); transition: all 0.3s;">
    <div style="font-size: 2rem; margin-bottom: 0.5rem;">🖼️</div>
    <h3 style="margin: 0; color: var(--vp-c-text-1);">图片处理</h3>
    <p style="color: var(--vp-c-text-2); margin: 0.5rem 0 0 0;">图片编辑、格式转换</p>
  </a>
  <a href="./video" style="padding: 1.5rem; background: var(--vp-c-bg-soft); border-radius: 8px; text-decoration: none; border: 1px solid var(--vp-c-divider); transition: all 0.3s;">
    <div style="font-size: 2rem; margin-bottom: 0.5rem;">🎞️</div>
    <h3 style="margin: 0; color: var(--vp-c-text-1);">视频处理</h3>
    <p style="color: var(--vp-c-text-2); margin: 0.5rem 0 0 0;">视频裁剪、压缩、转换</p>
  </a>
  <a href="./ocr" style="padding: 1.5rem; background: var(--vp-c-bg-soft); border-radius: 8px; text-decoration: none; border: 1px solid var(--vp-c-divider); transition: all 0.3s;">
    <div style="font-size: 2rem; margin-bottom: 0.5rem;">🔍</div>
    <h3 style="margin: 0; color: var(--vp-c-text-1);">OCR 识别</h3>
    <p style="color: var(--vp-c-text-2); margin: 0.5rem 0 0 0;">文字识别、自动复制</p>
  </a>
  <a href="./shortcuts" style="padding: 1.5rem; background: var(--vp-c-bg-soft); border-radius: 8px; text-decoration: none; border: 1px solid var(--vp-c-divider); transition: all 0.3s;">
    <div style="font-size: 2rem; margin-bottom: 0.5rem;">⌨️</div>
    <h3 style="margin: 0; color: var(--vp-c-text-1);">快捷键汇总</h3>
    <p style="color: var(--vp-c-text-2); margin: 0.5rem 0 0 0;">所有快捷键一览</p>
  </a>
</div>

## 💡 使用技巧

### 截图技巧
- 使用 **Ctrl+Shift+A/R/T** 快捷键可直接进入对应标注模式
- 截图后按住 **Shift** 拖动可等比缩放选区
- 双击编辑窗口外部区域可快速关闭

### 录屏技巧
- 录制前关闭不必要的通知避免录入
- 使用"标注"功能可实时圈重点
- 暂停功能可用于分段录制再合并

### 效率技巧
- 善用"贴图"功能做临时参考
- OCR 结果自动复制到剪贴板，识别后直接粘贴
- 视频编辑时用 Ctrl+滚轮快速定位精确帧

## 📞 获取帮助

- **QQ 交流群**：133125470
- **项目主页**：[GitHub](https://github.com/yun-cun-li/PixWit) / [Gitee](https://gitee.com/bjs1999/PixWit)
- **问题反馈**：[GitHub Issues](https://github.com/yun-cun-li/PixWit/issues) / [Gitee Issues](https://gitee.com/bjs1999/PixWit/issues)

---

**感谢使用 PixWit！** 🎉

*如有任何问题或建议，欢迎随时反馈！*

