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
 * @file WindowDetector.cpp
 * @brief 窗口检测工具类实现
 * 
 * @author CapStep Team
 * @version 1.0.0
 * @date 2025-01-27
 */

#include "WindowDetector.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

WindowDetector::WindowDetector(QObject *parent)
    : QObject(parent)
    , m_isSupported(false)
{
#ifdef Q_OS_WIN
    m_isSupported = true;
    qDebug() << "[WindowDetector] Windows platform detected, window detection supported";
#else
    qDebug() << "[WindowDetector] Non-Windows platform, window detection not supported";
#endif
}

WindowDetector::~WindowDetector()
{
    qDebug() << "[WindowDetector] Destroyed";
}

WindowInfo WindowDetector::getWindowAt(const QPoint &point)
{
    if (!m_isSupported) {
        qDebug() << "[WindowDetector] Window detection not supported on this platform";
        return WindowInfo();
    }

#ifdef Q_OS_WIN
    return getWindowAtWin(point);
#else
    return WindowInfo();
#endif
}

QList<WindowInfo> WindowDetector::getAllWindows()
{
    if (!m_isSupported) {
        qDebug() << "[WindowDetector] Window detection not supported on this platform";
        return QList<WindowInfo>();
    }

#ifdef Q_OS_WIN
    return getAllWindowsWin();
#else
    return QList<WindowInfo>();
#endif
}

bool WindowDetector::isSupported()
{
    return m_isSupported;
}

#ifdef Q_OS_WIN
WindowInfo WindowDetector::getWindowAtWin(const QPoint &point)
{
    WindowInfo windowInfo;
    
    // 使用 WindowFromPoint API 获取窗口句柄
    POINT winPoint = {point.x(), point.y()};
    HWND hwnd = WindowFromPoint(winPoint);
    
    if (hwnd == NULL) {
        qDebug() << "[WindowDetector] No window found at point:" << point;
        return windowInfo;
    }
    
    // 获取窗口信息
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        windowInfo.geometry = QRect(rect.left, rect.top, 
                                   rect.right - rect.left, 
                                   rect.bottom - rect.top);
    }
    
    // 获取窗口标题
    wchar_t title[256];
    if (GetWindowTextW(hwnd, title, 256) > 0) {
        windowInfo.title = QString::fromWCharArray(title);
    }
    
    // 获取窗口类名
    wchar_t className[256];
    if (GetClassNameW(hwnd, className, 256) > 0) {
        windowInfo.className = QString::fromWCharArray(className);
    }
    
    // 检查窗口状态
    windowInfo.isVisible = IsWindowVisible(hwnd) != 0;
    windowInfo.isMinimized = IsIconic(hwnd) != 0;
    
    // 获取Z轴顺序
    windowInfo.zOrder = GetWindowLong(hwnd, GWL_EXSTYLE);
    
    qDebug() << "[WindowDetector] Found window at" << point 
             << "title:" << windowInfo.title
             << "geometry:" << windowInfo.geometry
             << "visible:" << windowInfo.isVisible;
    
    return windowInfo;
}

QList<WindowInfo> WindowDetector::getAllWindowsWin()
{
    QList<WindowInfo> windows;
    EnumWindowsData data;
    data.windows = windows;
    
    // 枚举所有顶级窗口
    EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&data));
    
    qDebug() << "[WindowDetector] Found" << data.windows.size() << "windows";
    return data.windows;
}

BOOL CALLBACK WindowDetector::enumWindowsProc(HWND hwnd, LPARAM lParam)
{
    EnumWindowsData* data = reinterpret_cast<EnumWindowsData*>(lParam);
    
    // 跳过不可见窗口
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }
    
    // 跳过最小化窗口
    if (IsIconic(hwnd)) {
        return TRUE;
    }
    
    // 获取窗口矩形
    RECT rect;
    if (!GetWindowRect(hwnd, &rect)) {
        return TRUE;
    }
    
    // 跳过太小的窗口
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    if (width < 50 || height < 50) {
        return TRUE;
    }
    
    WindowInfo windowInfo;
    windowInfo.geometry = QRect(rect.left, rect.top, width, height);
    
    // 获取窗口标题
    wchar_t title[256];
    if (GetWindowTextW(hwnd, title, 256) > 0) {
        windowInfo.title = QString::fromWCharArray(title);
    }
    
    // 跳过没有标题的窗口
    if (windowInfo.title.isEmpty()) {
        return TRUE;
    }
    
    // 获取窗口类名
    wchar_t className[256];
    if (GetClassNameW(hwnd, className, 256) > 0) {
        windowInfo.className = QString::fromWCharArray(className);
    }
    
    windowInfo.isVisible = true;
    windowInfo.isMinimized = false;
    windowInfo.zOrder = GetWindowLong(hwnd, GWL_EXSTYLE);
    
    data->windows.append(windowInfo);
    return TRUE;
}
#endif
