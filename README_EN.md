# PixWit - Screenshot · Screen Recording · Simple Editing

PixWit is a modern Windows tool for screenshots and screen recording with built‑in lightweight video editing. It supports one‑screen quick capture, history, rich annotations, and sticky notes on the desktop; full‑screen/region recording with pause/resume and completion prompts; and video trimming, removing middle segments, compression and format conversion (including GIF).

## ✨ Highlights

- 🖥️ **Single‑screen capture** – Press F1 on any monitor to capture that monitor. Simple and fast
- 📜 **Screenshot history** – Every capture is saved automatically; open history from the tray
- 🎯 **Smart selection** – Drag to select with precise border fine‑tuning
- 🎨 **Visual adjustment** – 8 resize handles with real‑time preview
- ✏️ **Rich editing tools** – Pen, line, arrow, ellipse, rectangle, mosaic, text
- 🖼️ **SVG icons** – Crisp on any DPI
- 📌 **Sticky notes** – Pin movable and resizable screenshots on desktop
- 💾 **Quick save** – PNG support
- 📋 **Clipboard** – One‑click copy to clipboard
- ⚡ **High performance** – Robust in multi‑monitor mixed‑DPI setups

### Recording & Video
- 🎥 **Recording modes** – Full screen or region
- ⏸️ **Pause/Resume** – Via global hotkeys and system tray
- 🧭 **Recording status** – Blinking border around the screen in full‑screen mode (border is not recorded)
- ✅ **Recording complete dialog** – After finishing, a dialog with Open/Edit/Open Folder (no Close button; actions do not auto‑close)
- 🖱️ **Recording annotations** – Toggle “Show mouse clicks” in region recording

### Video Editing & Conversion
- ✂️ **Trim head/tail + remove middle segments** – Mark multiple middle ranges to delete; remaining parts are concatenated automatically
- 🧭 **Pro timeline** – Thumbnails + audio waveform overlay, zoom and scroll, highlighted selection
- 🏷️ **Deletion markers** – Semi‑transparent red overlays for all ranges to delete; current selection highlighted
- ▶️ **Segment preview** – Double‑click any item in the list to instantly preview on the right
- 📜 **Scrollable list** – Bounded height with auto scroll bar
- 🔄 **Format conversion** – MP4 / AVI / MOV / WebM / GIF (GIF uses palettegen/paletteuse two‑stage optimization)
- 🗜️ **Compression** – Dark theme consistent with the Conversion tab
- 🐭 **GIF open strategy** – Drag/open GIF jumps directly to video editor

### Interaction & UX
- 🧭 **Buttons at timeline bottom‑right** – “Play Selection” and “Trim…” fixed at the bottom‑right for visual consistency
- 🧼 **Top whitespace reduction** – Tabs sit closer to the title bar
- 🧹 **Menu bar removed** – Clean UI while keeping feature entry points

## 🖼️ Screenshots

### Main Window
- Modern gradient background
- Minimal two‑button layout
- Borderless window with minimize support

### Screenshot Editor
- Action buttons shown right after capture
- Four actions: Save, Copy, Pin as Sticky Note, Close
- Buttons hug the image without extra whitespace

### Sticky Note
- Keeps original image size
- Draggable and resizable
- Mouse‑wheel zoom
- Context menu operations

## 🚀 Quick Start

### 📥 Download & Install

#### 💚 Recommended: Portable version (no install, fewer AV false positives)

**Best for quick try‑out or if you don’t want to deal with antivirus prompts**

**Download:**
- 🌐 GitHub: [Releases](https://github.com/yun-cun-li/PixWit/releases) – recommended for global users
- 🇨🇳 Gitee: [Releases](https://gitee.com/bjs1999/PixWit/releases) – recommended for China mainland

**How to use:**
1. Download the latest portable ZIP
2. Extract to any folder
3. Run `运行PixWit.bat` or `PixWit.exe`
4. Done! No installation, no admin privileges required


---

#### 🔧 Optional: Installer version

**Best for users who want startup integration and system shortcuts**

**Download:**
- 🌐 GitHub: [Releases](https://github.com/yun-cun-li/PixWit/releases)
- 🇨🇳 Gitee: [Releases](https://gitee.com/bjs1999/PixWit/releases)

Download the latest installer

#### ⚠️ About antivirus warnings

The app is not code‑signed yet, so some antivirus products may flag it by mistake.

**✅ Why is it safe?**
- 🌐 Fully open source: [GitHub](https://github.com/yun-cun-li/PixWit) / [Gitee](https://gitee.com/bjs1999/PixWit)
- 🔒 No malicious behavior: only for screenshots; collects no data
- 🔍 Open and transparent: anyone can review the code
- 🔐 File integrity: verify with SHA256 checksums

**🛡️ How to use safely?**
1. **Recommended:** use the portable version (fewer false positives)
2. **If blocked:** click “Allow to run”
   - Windows Defender: “More info” → “Run anyway”
   - 360/QQ PC Manager: “Trust this program” or “Allow to run”
3. **Add to whitelist:** see [detailed guide](docs/ADD_TO_WHITELIST.md)

**💡 Help us improve**
- ⭐ Star the project (improves credibility)
- 📢 Share with friends (increases downloads)

---

### 🔐 File verification (optional)

Verify integrity after download to ensure files haven’t been tampered with:

**Windows:**
```cmd
certutil -hashfile PixWit-Setup.exe SHA256
certutil -hashfile PixWit-Portable.zip SHA256
```

Compare the output with the official checksums above; they must match exactly.

---

### Basic operations

1. **Region capture:** run the app and drag to select a region
2. **Fine‑tune selection:** drag borders to adjust; right‑click or press Enter to confirm
3. **Editing tools:** annotate with pen, line, arrow, etc.
4. **Save:** click Save or press Ctrl+S
5. **Copy:** click Copy or press Ctrl+C
6. **Pin to desktop:** click Sticky Note to pin the capture on desktop
7. **Undo:** click Undo or press Ctrl+Z

### Editing tools

- **Pen** – free drawing
- **Line** – straight line
- **Arrow** – direction annotation
- **Ellipse** – circle/ellipse
- **Rectangle** – rectangle box
- **Mosaic** – pixelate sensitive areas
- **Text** – add labels

### Hotkeys

**Selection**
- `Drag border` – resize selection
- `Drag inside` – move selection
- `Right‑click` / `Enter` / `Space` – confirm
- `ESC` – cancel capture

**Editing**
- `Ctrl+Z` – undo
- `Ctrl+S` – save
- `Ctrl+C` – copy to clipboard
- `ESC` – close editor

## 🛠️ Development Environment

### Tech stack
- **Language:** C++17
- **Framework:** Qt 6.8.3
- **Build system:** CMake 3.16+
- **Compiler:** MSVC 2022

### Requirements
- Windows 10/11 (x64)
- Visual Studio 2022
- Qt 6.8.3 (MSVC 2022 64‑bit)
- CMake 3.16+

## 📚 Documentation

- [📖 Build Guide](docs/BUILD.md) – detailed compile and build steps
- [🚀 Release Guide](docs/RELEASE_GUIDE.md) – release process

## 📦 Build

### Quick build
```bash
# Use the automated build script
scripts\build.bat
```

### Manual build
```bash
# Set Qt environment
set PATH=C:\Qt\6.8.3\msvc2022_64\bin;%PATH%

# Create build directory
mkdir build
cd build

# Configure CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release

# Copy runtime deps
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6*.dll" "bin\Release\"
mkdir "bin\Release\platforms"
copy "C:\Qt\6.8.3\msvc2022_64\plugins\platforms\qwindows.dll" "bin\Release\platforms\"
```

See `编译说明.md` for more details.

## 📁 Project Structure

```
PixWit/
├── src/                              # source code
│   ├── main_modular.cpp             # app entry
│   ├── MainWindow.cpp               # main window
│   ├── ScreenshotEditWindow.cpp     # editor
│   ├── StylePopover.cpp             # color/width selector
│   └── ...                          # more modules
├── resources/                        # app resources
│   └── icons/                       # SVG icons
├── build/                           # build output
├── dist/                            # release output
├── CMakeLists.txt                   # CMake config
├── scripts\build.bat                # build script
├── scripts\deploy.bat               # packaging script
├── quick_scripts\release.bat        # one‑click release
└── README.md                        # project readme
```

## 🎯 Feature Details

### One‑screen quick capture
- Press F1 on the target monitor to capture there
- No cross‑screen hassle; intuitive and fast
- Avoids mixed‑DPI pitfalls
- Responsive and smooth

### Screenshot history
- Auto‑save on every capture
- Filename: `YYYY_MM_DD_HH_MM_SS.png`
- One‑click open from system tray
- Save path: `%APPDATA%\PixWit\ScreenshotHistory\images\`

### Rich tools
- 7 drawing tools
- 6 color presets
- 3 stroke widths
- Undo/Redo

### SVG icons
- Sharp and beautiful
- DPI‑aware
- Small footprint
- Fast to load

### Convenience
- Edit right after capture
- One‑click Save/Copy/Pin
- Hotkey support
- Smart, adaptive layout

## 🐛 Support & FAQ

### ❓ Frequently Asked

#### 1. Antivirus reports a virus. What should I do?
It’s a false positive because the program isn’t code‑signed yet. Solutions:
- **Option 1:** use the portable version (no install, fewer alerts)
- **Option 2:** click “Allow/Trust this program”
- **Option 3:** [add to whitelist](docs/ADD_TO_WHITELIST.md)

#### 2. How to verify safety?
1. Review the full source: [GitHub](https://github.com/yun-cun-li/PixWit) / [Gitee](https://gitee.com/bjs1999/PixWit)
2. Use SHA256 to verify file integrity (see Download section)
3. Check project information and community feedback

#### 3. Why not buy a code‑signing certificate?
As an open‑source project, we haven’t purchased a certificate yet. We’ll consider it with enough community support.

### 📮 More help
1. See troubleshooting in [docs/BUILD.md](docs/BUILD.md)
2. Check whether Qt runtime DLLs are present
3. Confirm OS compatibility (Windows 10/11)
4. Open an issue on [GitHub](https://github.com/yun-cun-li/PixWit/issues) or [Gitee](https://gitee.com/bjs1999/PixWit/issues)

## 📄 License

This project is licensed under GNU General Public License v3.0 (GPL‑3.0).

You may:
- ✅ Use, copy, and redistribute
- ✅ Modify and distribute modified versions
- ✅ Use for commercial purposes

But you must:
- 📋 Keep the same GPL‑3.0 license
- 📝 Attribute the original authors
- 🔓 Provide full source code of your modifications

See [LICENSE](LICENSE) for the full text.

## 🤝 Contributing

Issues and PRs are welcome!

**Project links:**
- 🌐 GitHub: [https://github.com/yun-cun-li/PixWit](https://github.com/yun-cun-li/PixWit)
- 🇨🇳 Gitee: [https://gitee.com/bjs1999/PixWit](https://gitee.com/bjs1999/PixWit)

### Development guide
1. Fork this repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📊 Changelog

- **v0.2.0‑dev (development)**
  - 🎥 Full‑screen/region recording; pause/resume; tray + hotkey linkage
  - 🧭 Blinking border prompt in full‑screen recording (not captured)
  - ✅ Recording completion dialog (no Close; Open/Open Folder do not close)
  - ✂️ Editor: “Remove middle segments” with auto concat
  - 🧭 Timeline: thumbnails + audio waveform; red overlays for deletions; double‑click preview
  - 🔄 New “Conversion” tab (incl. GIF with high‑quality palette flow)
  - 🗜️ Compression tab matches Conversion styling; all dialogs use dark theme
  - 🖼️ Drag/open GIF goes to the video editor
  - 🧼 Less top whitespace / removed menu bar; actions at timeline bottom‑right
  - 🐞 Stability fixes: stuck on stop, unified ESC behavior, empty‑audio failure, etc.

- **v0.1.2**
  - ✅ One‑screen capture
  - ✅ Auto‑save history
  - ✅ System‑tray history
  - ✅ 7 editing tools
  - ✅ SVG icons
  - ✅ Sticky notes
  - ✅ Hotkeys (F1/F2/F3)

## 🎉 Thanks

Thanks to the Qt framework for powerful GUI support!

---

**PixWit** – Capture every moment! 📸


