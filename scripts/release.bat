@echo off
chcp 65001 >nul
echo ========================================
echo CapStep 一键发布脚本
echo ========================================
echo.

REM 切换到项目根目录
cd /d "%~dp0\.."

REM 从命令行参数获取版本号
if "%~1"=="" (
    set VERSION=1.0.0
    echo [提示] 使用默认版本号: %VERSION%
    echo [提示] 可通过命令指定版本: release.bat 1.0.1
) else (
    set VERSION=%~1
    echo [提示] 使用指定版本号: %VERSION%
)

echo.
echo ========================================
echo 步骤 1/3: 编译程序
echo ========================================
set NO_PAUSE=1
call scripts\build.bat
if %ERRORLEVEL% NEQ 0 (
    echo [错误] 编译失败！
    pause
    exit /b 1
)

echo.
echo ========================================
echo 步骤 2/3: 打包发布文件
echo ========================================
call scripts\deploy.bat %VERSION%
if errorlevel 1 (
    echo [错误] 打包失败！
    pause
    exit /b 1
)
echo [完成] 打包成功！

echo.
echo ========================================
echo 步骤 3/5: 关闭运行中的程序和清理文件锁
echo ========================================

REM 检查并关闭运行中的 CapStep 进程
echo 正在检查运行中的CapStep进程...
tasklist /FI "IMAGENAME eq CapStep.exe" 2>NUL | find /I /N "CapStep.exe">NUL

if "%ERRORLEVEL%"=="0" (
    echo [警告] 发现运行中的CapStep进程，正在关闭...
    taskkill /F /IM CapStep.exe /T >NUL 2>&1
    
    REM 等待进程完全关闭和文件句柄释放
    echo [信息] 等待文件句柄释放...
    timeout /t 5 /nobreak >NUL
    echo [完成] 进程已关闭
) else (
    echo [信息] 没有运行中的进程
)

REM 关闭可能打开 dist 目录的 explorer 窗口
echo [信息] 关闭可能占用文件的 explorer 窗口...
taskkill /FI "WINDOWTITLE eq dist*" /IM explorer.exe 2>NUL >NUL

REM 清理旧的安装程序文件（避免被占用）
if exist "dist\CapStep-%VERSION%-Setup.exe" (
    echo [信息] 删除旧的安装程序...
    del /F /Q "dist\CapStep-%VERSION%-Setup.exe" 2>NUL
    if exist "dist\CapStep-%VERSION%-Setup.exe" (
        echo [警告] 无法删除旧安装程序，文件可能被占用
        echo [提示] 请关闭所有访问 dist 目录的程序
    )
)

REM 额外等待，确保文件系统释放所有锁
echo [信息] 等待文件系统就绪...
timeout /t 5 /nobreak >NUL

echo.
echo ========================================
echo 步骤 4/5: 创建安装程序
echo ========================================

REM 检查是否安装了 Inno Setup
set "ISCC_PATH="
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" (
    set "ISCC_PATH=C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
) else if exist "C:\Program Files\Inno Setup 6\ISCC.exe" (
    set "ISCC_PATH=C:\Program Files\Inno Setup 6\ISCC.exe"
) else (
    echo [警告] 未找到 Inno Setup，跳过安装程序创建
    echo [提示] 请从 https://jrsoftware.org/isdl.php 下载安装
    goto SKIP_INSTALLER
)

echo 找到 Inno Setup: %ISCC_PATH%

REM 删除旧的安装程序文件（避免被占用）
set "SETUP_FILE=dist\CapStep-%VERSION%-Setup.exe"
if exist "%SETUP_FILE%" (
    echo [信息] 删除旧的安装程序...
    del /F /Q "%SETUP_FILE%" 2>NUL
    timeout /t 2 /nobreak >NUL
    if exist "%SETUP_FILE%" (
        echo [警告] 无法删除旧安装程序，可能被占用
        echo [提示] 请手动删除 %SETUP_FILE% 后重试
        goto SKIP_INSTALLER
    )
)

echo 正在创建安装程序...
"%ISCC_PATH%" /DMyAppVersion=%VERSION% scripts\installer.iss
if %ERRORLEVEL% EQU 0 (
    echo [成功] 安装程序创建完成！
    echo 位置: dist\CapStep-%VERSION%-Setup.exe
) else (
    echo [错误] 安装程序创建失败！
)

:SKIP_INSTALLER

echo.
echo ========================================
echo 步骤 5/5: 创建 ZIP 压缩包
echo ========================================

set "ZIP_NAME=dist\CapStep-%VERSION%-Portable.zip"
set "SOURCE_DIR=dist\CapStep-%VERSION%"

REM 检查源目录是否存在
if not exist "%SOURCE_DIR%" (
    echo [错误] 找不到发布目录: %SOURCE_DIR%
    goto SKIP_ZIP
)

REM 删除旧的 ZIP 文件
if exist "%ZIP_NAME%" (
    echo 删除旧的 ZIP 文件...
    del "%ZIP_NAME%"
)

echo 正在创建 ZIP 压缩包...
echo 源目录: %SOURCE_DIR%
echo 目标文件: %ZIP_NAME%
echo.

REM 使用 PowerShell 压缩
powershell -Command "Compress-Archive -Path '%SOURCE_DIR%\*' -DestinationPath '%ZIP_NAME%' -CompressionLevel Optimal" 2>nul

if exist "%ZIP_NAME%" (
    echo [成功] ZIP 压缩包创建完成！
    
    REM 显示文件大小
    for %%F in ("%ZIP_NAME%") do (
        set "SIZE=%%~zF"
        set /a "SIZE_MB=!SIZE! / 1048576"
        echo 文件大小: !SIZE_MB! MB
    )
) else (
    echo [警告] PowerShell 压缩失败，尝试使用 tar...
    
    REM Windows 10+ 内置 tar 命令
    cd dist
    tar -a -c -f "CapStep-%VERSION%-Portable.zip" "CapStep-%VERSION%" 2>nul
    cd ..
    
    if exist "%ZIP_NAME%" (
        echo [成功] 使用 tar 创建 ZIP 完成！
    ) else (
        echo [失败] 无法自动创建 ZIP
        echo [提示] 请手动压缩 %SOURCE_DIR%
    )
)

:SKIP_ZIP

echo.
echo ========================================
echo 步骤 6/6: 生成文件校验和
echo ========================================
echo.
echo 正在生成 SHA256 校验和...
echo.

cd dist

if exist "CapStep-%VERSION%-Setup.exe" (
    echo [CapStep-%VERSION%-Setup.exe]
    for /f "skip=1 delims=" %%H in ('certutil -hashfile "CapStep-%VERSION%-Setup.exe" SHA256') do (
        if not "%%H"=="" (
            echo SHA256: %%H
            goto :setup_hash_done
        )
    )
    :setup_hash_done
    echo.
)

if exist "CapStep-%VERSION%-Portable.zip" (
    echo [CapStep-%VERSION%-Portable.zip]
    for /f "skip=1 delims=" %%H in ('certutil -hashfile "CapStep-%VERSION%-Portable.zip" SHA256') do (
        if not "%%H"=="" (
            echo SHA256: %%H
            goto :zip_hash_done
        )
    )
    :zip_hash_done
    echo.
)

cd ..

echo.
echo ========================================
echo 发布完成！
echo ========================================
echo.
echo 版本号: %VERSION%
echo.
echo 生成的文件:
echo   1. 绿色便携版: dist\CapStep-%VERSION%\
if exist "dist\CapStep-%VERSION%-Portable.zip" (
    echo   2. ZIP 压缩包: dist\CapStep-%VERSION%-Portable.zip
)
if exist "dist\CapStep-%VERSION%-Setup.exe" (
    echo   3. 安装程序: dist\CapStep-%VERSION%-Setup.exe
)
echo.
echo 接下来可以:
echo   1. 复制上述 SHA256 校验和到 README.md
echo   2. 上传文件到 Gitee Releases（附带校验和）
echo   3. 更新 latest.json 版本信息
echo   4. 在 Release 说明中添加：如遇杀毒软件拦截请参考 docs\ADD_TO_WHITELIST.md
echo.

REM 打开发布目录
explorer dist

pause
