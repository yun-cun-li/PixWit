@echo off
chcp 65001 > nul
echo ========================================
echo CapStep 截图工具 - 打包发布脚本
echo ========================================
echo.

REM 切换到项目根目录
cd /d "%~dp0\.."

set APP_NAME=CapStep

REM 从命令行参数获取版本号，如果没有则使用默认值
if "%~1"=="" (
    set VERSION=1.0.0
    echo [提示] 使用默认版本号: %VERSION%
    echo [提示] 可通过命令指定版本: deploy.bat 1.0.1
) else (
    set VERSION=%~1
    echo [提示] 使用指定版本号: %VERSION%
)

set BUILD_DIR=build\bin\Release
set DIST_DIR=dist\%APP_NAME%-%VERSION%

REM 检查是否已编译
if not exist "%BUILD_DIR%\CapStep.exe" (
    echo [错误] 未找到编译后的可执行文件，请先运行 build.bat
    pause
    exit /b 1
)

echo [1/5] 清理旧的发布目录...
if exist "dist" rd /s /q "dist"
mkdir "dist"
mkdir "%DIST_DIR%"

echo [2/5] 复制可执行文件...
copy "%BUILD_DIR%\CapStep.exe" "%DIST_DIR%\" >nul

echo [3/5] 使用 windeployqt 收集Qt依赖...
REM 尝试查找windeployqt
where windeployqt >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [警告] windeployqt 未在PATH中找到，尝试手动复制DLL...
    goto MANUAL_COPY
)

windeployqt --release --no-translations --no-system-d3d-compiler --no-opengl-sw "%DIST_DIR%\CapStep.exe"
goto COPY_RESOURCES

:MANUAL_COPY
echo 手动复制Qt DLL文件...
xcopy /E /I /Y "%BUILD_DIR%\*.dll" "%DIST_DIR%\" >nul
xcopy /E /I /Y "%BUILD_DIR%\platforms" "%DIST_DIR%\platforms\" >nul

:COPY_RESOURCES
echo [4/5] 复制资源文件...
if not exist "%DIST_DIR%\resources" mkdir "%DIST_DIR%\resources"
if exist "resources" xcopy /E /I /Y "resources" "%DIST_DIR%\resources\" >nul
if exist "README.md" copy "README.md" "%DIST_DIR%\说明.txt" >nul
if exist "LICENSE" copy "LICENSE" "%DIST_DIR%\LICENSE.txt" >nul

echo [5/5] 创建启动快捷方式...
REM 创建一个简单的启动批处理文件
(
echo @echo off
echo start CapStep.exe
) > "%DIST_DIR%\运行CapStep.bat"

echo.
echo ========================================
echo 打包完成！
echo ========================================
echo.
echo 发布目录: %DIST_DIR%
echo 可执行文件: %DIST_DIR%\CapStep.exe
echo.
echo 现在可以将 %DIST_DIR% 目录打包成ZIP文件分发
echo 或者运行 create_installer.bat 创建安装程序
echo.

REM 打开文件管理器显示结果
explorer "%DIST_DIR%"

REM 返回成功状态码
exit /b 0

