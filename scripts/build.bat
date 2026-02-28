@echo off
chcp 65001 > nul
echo 开始编译简单截图工具...

REM 切换到项目根目录
cd /d "%~dp0\.."

REM 设置Qt环境
set PATH=C:\Qt\6.8.3\msvc2022_64\bin;%PATH%

REM 保存当前目录
set ORIGINAL_DIR=%CD%

REM 创建构建目录
if not exist build mkdir build
cd build

REM 配置CMake
echo 配置CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo CMake配置失败！
    pause
    exit /b 1
)

REM 编译
echo 开始编译...
cmake --build . --config Release
if errorlevel 1 (
    echo 编译失败！
    pause
    exit /b 1
)

REM 复制依赖库
echo Copying Qt dependencies...

REM 复制 Release 版本依赖
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Core.dll" "bin\Release\" 2>nul
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Gui.dll" "bin\Release\" 2>nul
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Widgets.dll" "bin\Release\" 2>nul
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Network.dll" "bin\Release\" 2>nul

if not exist "bin\Release\platforms" mkdir "bin\Release\platforms"
copy "C:\Qt\6.8.3\msvc2022_64\plugins\platforms\qwindows.dll" "bin\Release\platforms\" 2>nul

REM 复制 Debug 版本依赖
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Cored.dll" "bin\Debug\" 2>nul
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Guid.dll" "bin\Debug\" 2>nul
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Widgetsd.dll" "bin\Debug\" 2>nul
copy "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Networkd.dll" "bin\Debug\" 2>nul

if not exist "bin\Debug\platforms" mkdir "bin\Debug\platforms"
copy "C:\Qt\6.8.3\msvc2022_64\plugins\platforms\qwindowsd.dll" "bin\Debug\platforms\" 2>nul

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo Executable: %CD%\bin\Release\CapStep.exe
echo.

REM 返回原始目录
cd /d "%ORIGINAL_DIR%"

REM 如果不是从 release.bat 调用，则暂停
if not "%NO_PAUSE%"=="1" pause
