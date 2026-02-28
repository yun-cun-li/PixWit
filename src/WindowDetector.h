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
 * @file WindowDetector.h
 * @brief 窗口检测工具类
 * 
 * 提供窗口检测和枚举功能，支持：
 * - 获取系统窗口列表
 * - 检测鼠标位置下的窗口
 * - 获取窗口边界和属性
 * 
 * @author CapStep Team
 * @version 1.0.0
 * @date 2025-01-27
 */

#ifndef WINDOWDETECTOR_H
#define WINDOWDETECTOR_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QString>
#include <QList>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

/**
 * @struct WindowInfo
 * @brief 窗口信息结构
 */
struct WindowInfo {
    QRect geometry;        ///< 窗口几何位置
    QString title;         ///< 窗口标题
    QString className;     ///< 窗口类名
    bool isVisible;        ///< 是否可见
    bool isMinimized;      ///< 是否最小化
    int zOrder;           ///< Z轴顺序
};

/**
 * @class WindowDetector
 * @brief 窗口检测工具类
 */
class WindowDetector : public QObject
{
    Q_OBJECT

public:
    explicit WindowDetector(QObject *parent = nullptr);
    ~WindowDetector();

    /**
     * @brief 获取指定位置下的窗口
     * @param point 鼠标位置
     * @return 窗口信息，如果没有窗口则返回空结构
     */
    WindowInfo getWindowAt(const QPoint &point);

    /**
     * @brief 获取所有可见窗口列表
     * @return 窗口信息列表
     */
    QList<WindowInfo> getAllWindows();

    /**
     * @brief 检查是否支持窗口检测
     * @return true支持，false不支持
     */
    bool isSupported();

signals:
    /**
     * @brief 窗口检测完成信号
     * @param windowInfo 检测到的窗口信息
     */
    void windowDetected(const WindowInfo &windowInfo);

private:
#ifdef Q_OS_WIN
    /**
     * @brief Windows平台窗口检测
     */
    WindowInfo getWindowAtWin(const QPoint &point);
    QList<WindowInfo> getAllWindowsWin();
    static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam);
    
    struct EnumWindowsData {
        QList<WindowInfo> windows;
        QPoint targetPoint;
        WindowInfo targetWindow;
    };
#endif

    bool m_isSupported;    ///< 是否支持窗口检测
};

#endif // WINDOWDETECTOR_H
