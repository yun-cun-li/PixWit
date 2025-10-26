/*
 * CapStep - Smart Screenshot Tool
 * Copyright (C) 2024-2025 CapStep Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file main_modular.cpp
 * @brief 模块化截图工具主文件
 * 
 * 使用模块化结构的截图工具主程序
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QIcon>
#include <QLockFile>
#include <QDir>
#include <QLocalSocket>

// 模块化头文件
#include "MainWindow.h"
#include "ScreenshotTool.h"
#include "GlobalHotkey.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("CapStep");
    app.setApplicationVersion("0.1.3");
    app.setOrganizationName("CapStep Team");
    
    // 单实例锁文件 - 防止多次启动
    QString lockFilePath = QDir::temp().absoluteFilePath("CapStep.lock");
    QLockFile lockFile(lockFilePath);
    lockFile.setStaleLockTime(0);  // 立即检测
    
    if (!lockFile.tryLock(100)) {
        // 已有实例在运行，通知它显示主窗口
        qDebug() << "CapStep is already running. Notifying the first instance...";
        
        // 尝试连接到已运行的实例
        QLocalSocket socket;
        socket.connectToServer("CapStepInstance");
        
        if (socket.waitForConnected(1000)) {
            // 发送显示窗口的消息
            socket.write("SHOW");
            socket.flush();
            socket.waitForBytesWritten(1000);
            socket.disconnectFromServer();
            qDebug() << "Notified first instance to show window";
        } else {
            qDebug() << "Failed to connect to first instance:" << socket.errorString();
        }
        
        return 0;  // 退出第二个实例
    }
    
    qDebug() << "Starting CapStep (Single Instance)...";
    qDebug() << "Lock file:" << lockFilePath;
    
    // 设置应用程序图标
    app.setWindowIcon(QIcon(":/icons/resources/icons/app.ico"));
    
    // 设置关闭最后一个窗口时不退出应用（托盘应用需要）
    app.setQuitOnLastWindowClosed(false);
    
    // 创建主窗口
    MainWindow mainWindow;
    mainWindow.show();
    
    int result = app.exec();
    
    // 程序退出时自动释放锁
    lockFile.unlock();
    
    return result;
}

// 注意：main_modular.cpp 不需要包含 MOC 文件
// 因为此文件没有定义任何 QObject 类
