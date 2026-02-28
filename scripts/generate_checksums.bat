@echo off
chcp 65001 >nul
echo ========================================
echo 生成文件校验和
echo ========================================

set VERSION=%1
if "%VERSION%"=="" set VERSION=0.1.2

cd /d "%~dp0\..\dist"

echo.
echo 正在计算文件 SHA256 校验和...
echo.

if exist "CapStep-%VERSION%-Setup.exe" (
    echo [CapStep-%VERSION%-Setup.exe]
    certutil -hashfile "CapStep-%VERSION%-Setup.exe" SHA256 | findstr /v "SHA256 CertUtil"
    echo.
) else (
    echo [警告] 未找到 Setup.exe
    echo.
)

if exist "CapStep-%VERSION%-Portable.zip" (
    echo [CapStep-%VERSION%-Portable.zip]
    certutil -hashfile "CapStep-%VERSION%-Portable.zip" SHA256 | findstr /v "SHA256 CertUtil"
    echo.
) else (
    echo [警告] 未找到 Portable.zip
    echo.
)

echo ========================================
echo 请将上述校验和复制到：
echo   1. README.md 的下载说明中
echo   2. Gitee Release 页面
echo ========================================
echo.

cd /d "%~dp0\.."
pause

