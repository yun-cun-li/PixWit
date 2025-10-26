# CapStep - 智能截图工具

一个现代化的Windows截图工具，支持单屏快速截图、历史记录、丰富编辑功能和便签式贴图。

## ✨ 主要特性

- 🖥️ **单屏截图** - 在哪个屏幕按F1就在哪个屏幕截图，简单快捷
- 📜 **历史截图** - 每次截图自动保存，系统托盘一键查看历史
- 🎯 **智能选区** - 拖拽选择特定区域，支持拖拽边框微调
- 🎨 **可视化调整** - 8个调整手柄，实时预览效果
- ✏️ **丰富编辑工具** - 画笔、直线、箭头、圆形、矩形、马赛克、文字
- 🖼️ **SVG矢量图标** - 清晰美观，适配任何DPI
- 📌 **便签贴图** - 创建可移动、可缩放的桌面贴图
- 💾 **快速保存** - 支持PNG格式
- 📋 **剪贴板支持** - 一键复制到剪贴板
- ⚡ **高性能** - 多屏混合DPI环境完美支持

## 🖼️ 界面预览

### 主界面
- 现代化渐变背景设计
- 简洁的双按钮布局
- 无边框窗口，支持最小化

### 截图编辑窗口
- 截图后立即显示操作按钮
- 保存、复制、创建贴图、关闭四个功能
- 按钮紧贴图片，无多余空白

### 便签贴图
- 保持截图原始尺寸
- 支持拖拽移动
- 支持调整大小
- 滚轮缩放功能
- 右键菜单操作

## 🚀 快速开始

### 📥 下载安装

#### 💚 推荐：绿色便携版（无需安装，无拦截）

**适合快速体验、不想处理杀毒软件问题的用户**

**下载地址：**
- 🌐 GitHub: [Releases](https://github.com/yun-cun-li/CapStep/releases) - 国际用户推荐
- 🇨🇳 Gitee: [Releases](https://gitee.com/bjs1999/CapStep/releases) - 国内用户推荐

**使用方法：**
1. 下载 `CapStep-0.1.3-Portable.zip`
2. 解压到任意文件夹
3. 运行 `运行CapStep.bat` 或 `CapStep.exe`
4. 完成！无需安装，无需管理员权限

**SHA256 校验：**
```
013c4519727a61ca5fd4bd641330873039aa906413972d14dcc0b82ec698a846
```

---

#### 🔧 可选：安装程序版

**适合需要开机自启、系统集成的用户**

**下载地址：**
- 🌐 GitHub: [Releases](https://github.com/yun-cun-li/CapStep/releases)
- 🇨🇳 Gitee: [Releases](https://gitee.com/bjs1999/CapStep/releases)

下载 `CapStep-0.1.3-Setup.exe`

**SHA256 校验：**
```
[CapStep-0.1.3-Setup.exe]
SHA256: 20f437069b7f2ea9a3cda1c448d625760e12e7939191bac9dff85a63a7ae0451

[CapStep-0.1.3-Portable.zip]
SHA256: 5a189bad2dabbcea1f85e9c6fa6f325befcc86da4c4fef0b03fefb75a0ab61a3```

#### ⚠️ 关于杀毒软件拦截

由于本软件未购买代码签名证书，部分杀毒软件可能会误报。

**✅ 为什么是安全的？**
- 🌐 源代码完全开放：[GitHub](https://github.com/yun-cun-li/CapStep) / [Gitee](https://gitee.com/bjs1999/CapStep)
- 🔒 无恶意行为：仅用于截图，不收集任何信息
- 🔍 开源透明：欢迎任何人审查代码
- 🔐 校验文件：可通过 SHA256 验证文件未被篡改

**🛡️ 如何安全使用？**

1. **推荐方式**：下载绿色便携版（见上方，误报更少）
2. **如遇拦截**：点击"允许运行"
   - Windows Defender: 点击"更多信息" → "仍要运行"
   - 360/电脑管家: 点击"信任此程序"或"允许运行"
3. **添加到白名单**：[查看详细教程](docs/ADD_TO_WHITELIST.md)

**💡 帮助我们改进**

我们正在申请各大杀毒软件白名单。您可以：
- ⭐ 给项目加星（增加信誉）
- 📢 分享给朋友使用（增加下载量）

---

### 🔐 文件校验（可选）

下载后可验证文件完整性，确保未被篡改：

**Windows 系统：**
```cmd
certutil -hashfile CapStep-0.1.3-Setup.exe SHA256
certutil -hashfile CapStep-0.1.3-Portable.zip SHA256
```

将输出的校验和与上方提供的官方值对比，应完全一致。

---

### 基本操作

1. **区域截图**：运行程序后，拖拽选择截图区域
2. **微调选区**：选择完成后，拖拽边框调整区域，右键或 Enter 确认
3. **编辑工具**：选择画笔、直线、箭头等工具进行标注
4. **保存截图**：点击保存按钮或按 Ctrl+S
5. **复制截图**：点击复制按钮或按 Ctrl+C
6. **创建贴图**：点击贴图按钮，将截图钉在桌面上
7. **撤销操作**：点击撤销按钮或按 Ctrl+Z

### 编辑工具

- **画笔** - 自由绘制
- **直线** - 画直线
- **箭头** - 标注方向
- **圆形** - 画圆或椭圆
- **矩形** - 画矩形框
- **马赛克** - 打码遮挡
- **文字** - 添加文字说明

### 快捷键

**选区调整**
- `左键拖拽边框` - 调整选区大小
- `左键拖拽内部` - 移动选区
- `右键` / `Enter` / `Space` - 确认选区
- `ESC` - 取消截图

**编辑工具**
- `Ctrl+Z` - 撤销上一步操作
- `Ctrl+S` - 保存截图
- `Ctrl+C` - 复制到剪贴板
- `ESC` - 关闭编辑窗口

## 🛠️ 开发环境

### 技术栈
- **语言**：C++17
- **框架**：Qt 6.8.3
- **构建系统**：CMake 3.16+
- **编译器**：MSVC 2022

### 环境要求
- Windows 10/11 (x64)
- Visual Studio 2022
- Qt 6.8.3 (MSVC 2022 64-bit)
- CMake 3.16+

## 📚 文档

- [📖 构建说明](docs/BUILD.md) - 详细的编译和构建指南
- [🚀 发布指南](docs/RELEASE_GUIDE.md) - 版本发布流程说明

## 📦 编译构建

### 快速编译
```bash
# 使用自动构建脚本
scripts\build.bat
```

### 手动编译
```bash
# 设置Qt环境
set PATH=C:\Qt\6.8.3\msvc2022_64\bin;%PATH%

# 创建构建目录
mkdir build
cd build

# 配置CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# 编译项目
cmake --build . --config Release

# 复制依赖
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6*.dll" "bin\Release\"
mkdir "bin\Release\platforms"
copy "C:\Qt\6.8.3\msvc2022_64\plugins\platforms\qwindows.dll" "bin\Release\platforms\"
```

详细编译说明请参考 [编译说明.md](编译说明.md)

## 📁 项目结构

```
CapStep/
├── src/                              # 源代码目录
│   ├── main_modular.cpp             # 主程序入口
│   ├── MainWindow.cpp               # 主窗口
│   ├── ScreenshotEditWindow.cpp     # 编辑窗口
│   ├── StylePopover.cpp             # 颜色/粗细选择器
│   └── ...                          # 其他模块
├── resources/                        # 资源文件
│   └── icons/                       # SVG图标
├── build/                    # 构建目录
├── dist/                            # 发布目录
├── CMakeLists.txt                   # CMake配置
├── scripts\build.bat                        # 构建脚本
├── scripts\deploy.bat                       # 打包脚本
├── quick_scripts\release.bat                # 一键发布
└── README.md                        # 项目说明
```

## 🎯 功能特色

### 单屏快速截图
- 在哪个屏幕按F1就在哪截图
- 无需跨屏，简单直观
- 避免多屏DPI问题
- 响应快速不卡顿

### 历史截图管理
- 每次截图自动保存
- 文件名：日期_时间.png
- 系统托盘一键打开历史文件夹
- 保存路径：%APPDATA%\CapStep\ScreenshotHistory\images\

### 丰富编辑工具
- 7种绘图工具
- 6种颜色选择
- 3档粗细调节
- 撤销/重做功能

### SVG矢量图标
- 清晰美观
- 自适应DPI
- 占用空间小
- 加载速度快

### 便捷操作
- 截图后立即编辑
- 一键保存/复制/贴图
- 快捷键支持
- 智能布局自适应

## 🐛 问题反馈与常见问题

### ❓ 常见问题

#### 1. 杀毒软件报毒怎么办？

这是误报，原因是程序未购买代码签名证书。解决方法：
- **方法 1**：使用绿色便携版（不需要安装，误报更少）
- **方法 2**：点击"允许运行"或"信任此程序"
- **方法 3**：[添加到白名单](docs/ADD_TO_WHITELIST.md)

#### 2. 如何验证软件安全？

1. 查看完整源代码：[GitHub](https://github.com/yun-cun-li/CapStep) / [Gitee](https://gitee.com/bjs1999/CapStep)
2. 使用 SHA256 校验文件完整性（见上方下载部分）
3. 查看项目信息和用户评价

#### 3. 为什么不购买代码签名证书？

作为开源项目，暂时没有购买代码签名证书。如果项目获得足够支持，我们会考虑购买。

### 📮 其他问题

如果您在使用过程中遇到其他问题，请：

1. 查看 [编译说明](docs/BUILD.md) 中的故障排除
2. 检查是否缺少Qt运行时库
3. 确认系统兼容性（Windows 10/11）
4. 在 [GitHub](https://github.com/yun-cun-li/CapStep/issues) 或 [Gitee](https://gitee.com/bjs1999/CapStep/issues) 提交 Issue

## 📄 许可证

本项目采用 GNU General Public License v3.0 许可证 - 详见 [LICENSE](LICENSE) 文件

这是自由软件，您可以自由地重新分发和修改它。这些自由通过要求修改版本使用相同许可证来保护。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

**项目地址：**
- 🌐 GitHub: [https://github.com/yun-cun-li/CapStep](https://github.com/yun-cun-li/CapStep)
- 🇨🇳 Gitee: [https://gitee.com/bjs1999/CapStep](https://gitee.com/bjs1999/CapStep)

### 开发指南
1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📊 版本历史

- **v0.1.2** - 当前版本
  - ✅ 单屏快速截图（简化操作）
  - ✅ 历史截图自动保存
  - ✅ 系统托盘历史管理
  - ✅ 7种编辑工具
  - ✅ SVG矢量图标
  - ✅ 便签贴图功能
  - ✅ 快捷键支持（F1/F2/F3可选）

## 🎉 致谢

感谢 Qt 框架提供的强大GUI支持！

---

**CapStep** - 捕捉每一步精彩！ 📸