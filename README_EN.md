# CapStep - Smart Screenshot Tool

A modern Windows screenshot tool with single-screen quick capture, history management, rich editing features, and sticky note functionality.

## ✨ Key Features

- 🖥️ **Single-Screen Capture** - Press F1 on any screen to capture that screen - simple and fast
- 📜 **Screenshot History** - Automatically save every screenshot, view history from system tray
- 🎯 **Smart Selection** - Drag to select specific area, fine-tune with border adjustment
- 🎨 **Visual Adjustment** - 8 adjustment handles with real-time preview
- ✏️ **Rich Editing Tools** - Pen, line, arrow, circle, rectangle, mosaic, text
- 🖼️ **SVG Vector Icons** - Crisp and beautiful at any DPI
- 📌 **Sticky Notes** - Create movable and resizable desktop pins
- 💾 **Quick Save** - PNG format support
- 📋 **Clipboard Support** - One-click copy to clipboard
- ⚡ **High Performance** - Perfect multi-monitor mixed DPI support

## 🖼️ Interface Preview

### Main Window
- Modern gradient background design
- Clean dual-button layout
- Borderless window with minimize support

### Screenshot Editor
- Operation buttons appear immediately after capture
- Four functions: Save, Copy, Create Pin, Close
- Buttons stick to image with no extra space

### Sticky Notes
- Maintain original screenshot dimensions
- Support drag and move
- Support resize
- Mouse wheel zoom
- Right-click menu operations

## 🚀 Quick Start

### 📥 Download & Install

#### 💚 Recommended: Portable Version (No installation, no antivirus issues)

**Ideal for quick trial and users who don't want to deal with antivirus software**

**Download Links:**
- 🌐 GitHub: [Releases](https://github.com/yun-cun-li/CapStep/releases) - Recommended for international users
- 🇨🇳 Gitee: [Releases](https://gitee.com/bjs1999/CapStep/releases) - Recommended for China users

**How to Use:**
1. Download `CapStep-0.1.3-Portable.zip`
2. Extract to any folder
3. Run `运行CapStep.bat` or `CapStep.exe`
4. Done! No installation, no admin rights required

**SHA256 Checksum:**
```
013c4519727a61ca5fd4bd641330873039aa906413972d14dcc0b82ec698a846
```

---

#### 🔧 Optional: Setup Installer

**Ideal for users who need auto-start and system integration**

**Download Links:**
- 🌐 GitHub: [Releases](https://github.com/yun-cun-li/CapStep/releases)
- 🇨🇳 Gitee: [Releases](https://gitee.com/bjs1999/CapStep/releases)

Download `CapStep-0.1.3-Setup.exe`

**SHA256 Checksums:**
```
[CapStep-0.1.3-Setup.exe]
SHA256: 20f437069b7f2ea9a3cda1c448d625760e12e7939191bac9dff85a63a7ae0451

[CapStep-0.1.3-Portable.zip]
SHA256: 5a189bad2dabbcea1f85e9c6fa6f325befcc86da4c4fef0b03fefb75a0ab61a3```

#### ⚠️ About Antivirus Software Warnings

Some antivirus software may flag this software because it doesn't have a code signing certificate.

**✅ Why is it safe?**
- 🌐 Source code fully open: [GitHub](https://github.com/yun-cun-li/CapStep) / [Gitee](https://gitee.com/bjs1999/CapStep)
- 🔒 No malicious behavior: Only for screenshots, no data collection
- 🔍 Open and transparent: Anyone can review the code
- 🔐 File verification: Use SHA256 to verify file integrity

**🛡️ How to use safely?**

1. **Recommended**: Download the portable version (see above, fewer false positives)
2. **If blocked**: Click "Allow" or "Run anyway"
   - Windows Defender: Click "More info" → "Run anyway"
   - 360/PC Manager: Click "Trust this program" or "Allow to run"
3. **Add to whitelist**: [View detailed tutorial](docs/ADD_TO_WHITELIST.md)

**💡 Help us improve**

We are applying for whitelist status from major antivirus vendors. You can:
- ⭐ Star the project (increases credibility)
- 📢 Share with friends (increases download count)

---

### 🔐 File Verification (Optional)

After downloading, verify file integrity to ensure it hasn't been tampered with:

**Windows:**
```cmd
certutil -hashfile CapStep-0.1.3-Setup.exe SHA256
certutil -hashfile CapStep-0.1.3-Portable.zip SHA256
```

Compare the output checksum with the official values above - they should match exactly.

---

### Basic Usage

1. **Area Screenshot**: Run the program, drag to select screenshot area
2. **Fine-tune Selection**: After selection, drag borders to adjust area, right-click or press Enter to confirm
3. **Editing Tools**: Select pen, line, arrow and other tools for annotation
4. **Save Screenshot**: Click save button or press Ctrl+S
5. **Copy Screenshot**: Click copy button or press Ctrl+C
6. **Create Pin**: Click pin button to pin screenshot on desktop
7. **Undo**: Click undo button or press Ctrl+Z

### Editing Tools

- **Pen** - Free drawing
- **Line** - Draw straight lines
- **Arrow** - Mark directions
- **Circle** - Draw circles or ellipses
- **Rectangle** - Draw rectangular frames
- **Mosaic** - Blur/pixelate areas
- **Text** - Add text annotations

### Keyboard Shortcuts

**Selection Adjustment**
- `Left-click drag border` - Adjust selection size
- `Left-click drag inside` - Move selection
- `Right-click` / `Enter` / `Space` - Confirm selection
- `ESC` - Cancel screenshot

**Editing Tools**
- `Ctrl+Z` - Undo last operation
- `Ctrl+S` - Save screenshot
- `Ctrl+C` - Copy to clipboard
- `ESC` - Close editor window

## 🛠️ Development

### Tech Stack
- **Language**: C++17
- **Framework**: Qt 6.8.3
- **Build System**: CMake 3.16+
- **Compiler**: MSVC 2022

### Requirements
- Windows 10/11 (x64)
- Visual Studio 2022
- Qt 6.8.3 (MSVC 2022 64-bit)
- CMake 3.16+

## 📚 Documentation

- [📖 Build Guide](docs/BUILD.md) - Detailed compilation and build instructions
- [🚀 Release Guide](docs/RELEASE_GUIDE.md) - Version release process

## 📦 Build & Compile

### Quick Build
```bash
# Use automated build script
scripts\build.bat
```

### Manual Build
```bash
# Set Qt environment
set PATH=C:\Qt\6.8.3\msvc2022_64\bin;%PATH%

# Create build directory
mkdir build
cd build

# Configure CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build project
cmake --build . --config Release

# Copy dependencies
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6*.dll" "bin\Release\"
mkdir "bin\Release\platforms"
copy "C:\Qt\6.8.3\msvc2022_64\plugins\platforms\qwindows.dll" "bin\Release\platforms\"
```

For detailed build instructions, see [BUILD.md](docs/BUILD.md)

## 📁 Project Structure

```
CapStep/
├── src/                              # Source code directory
│   ├── main_modular.cpp             # Main entry point
│   ├── MainWindow.cpp               # Main window
│   ├── ScreenshotEditWindow.cpp     # Editor window
│   ├── StylePopover.cpp             # Color/width picker
│   └── ...                          # Other modules
├── resources/                        # Resource files
│   └── icons/                       # SVG icons
├── build/                            # Build directory
├── dist/                            # Distribution directory
├── CMakeLists.txt                   # CMake configuration
├── scripts\build.bat                # Build script
├── scripts\deploy.bat               # Package script
├── quick_scripts\release.bat        # One-click release
└── README.md                        # Project documentation
```

## 🎯 Feature Highlights

### Single-Screen Quick Capture
- Press F1 on any screen to capture that screen
- No cross-screen needed, simple and intuitive
- Avoids multi-monitor DPI issues
- Fast response without lag

### Screenshot History Management
- Automatically save every screenshot
- Filename: date_time.png
- Open history folder from system tray
- Save path: %APPDATA%\CapStep\ScreenshotHistory\images\

### Rich Editing Tools
- 7 drawing tools
- 6 color options
- 3 thickness levels
- Undo/redo functionality

### SVG Vector Icons
- Crisp and beautiful
- DPI adaptive
- Small footprint
- Fast loading

### Convenient Operations
- Edit immediately after capture
- One-click save/copy/pin
- Keyboard shortcuts support
- Smart adaptive layout

## 🐛 Feedback & FAQ

### ❓ Frequently Asked Questions

#### 1. What about antivirus warnings?

This is a false positive because the program doesn't have a code signing certificate. Solutions:
- **Method 1**: Use the portable version (no installation needed, fewer false positives)
- **Method 2**: Click "Allow to run" or "Trust this program"
- **Method 3**: [Add to whitelist](docs/ADD_TO_WHITELIST.md)

#### 2. How to verify the software is safe?

1. View full source code: [GitHub](https://github.com/yun-cun-li/CapStep) / [Gitee](https://gitee.com/bjs1999/CapStep)
2. Use SHA256 to verify file integrity (see download section above)
3. Check project information and user reviews

#### 3. Why not purchase a code signing certificate?

As an open-source project, we haven't purchased a code signing certificate yet. If the project receives enough support, we'll consider purchasing one.

### 📮 Other Issues

If you encounter other issues while using:

1. Check troubleshooting in [Build Documentation](docs/BUILD.md)
2. Verify Qt runtime libraries are present
3. Confirm system compatibility (Windows 10/11)
4. Submit an Issue on [GitHub](https://github.com/yun-cun-li/CapStep/issues) or [Gitee](https://gitee.com/bjs1999/CapStep/issues)

## 📄 License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

This is free software: you are free to redistribute and modify it. These freedoms are protected by requiring modified versions to use the same license.

## 🤝 Contributing

Issues and Pull Requests are welcome!

**Project Links:**
- 🌐 GitHub: [https://github.com/yun-cun-li/CapStep](https://github.com/yun-cun-li/CapStep)
- 🇨🇳 Gitee: [https://gitee.com/bjs1999/CapStep](https://gitee.com/bjs1999/CapStep)

### Development Guide
1. Fork this repository
2. Create feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📊 Version History

- **v0.1.2** - Current version
  - ✅ Single-screen quick capture (simplified operation)
  - ✅ Automatic screenshot history
  - ✅ System tray history management
  - ✅ 7 editing tools
  - ✅ SVG vector icons
  - ✅ Sticky note functionality
  - ✅ Keyboard shortcuts support (F1/F2/F3 options)

## 🎉 Acknowledgments

Thanks to the Qt framework for the powerful GUI support!

---

**CapStep** - Capture Every Step! 📸
