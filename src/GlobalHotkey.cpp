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
 * @file GlobalHotkey.cpp
 * @brief 全局热键管理类实现
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include "GlobalHotkey.h"
#include <QDebug>
#include <QApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

GlobalHotkey::GlobalHotkey(QWidget *parent)
    : QWidget(parent)
    , m_registered(false)
    , m_hotkeyId(1)
{
    // 设置为隐藏窗口，只用于接收热键消息
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    hide();
    
    // 安装事件过滤器到应用程序
    QApplication::instance()->installNativeEventFilter(this);
    
    // 默认注册F1热键
    setKeyF1();
}

GlobalHotkey::~GlobalHotkey()
{
    unregisterHotkey();
}

bool GlobalHotkey::registerHotkey(int key, int modifiers)
{
#ifdef Q_OS_WIN
    if (m_registered) {
        unregisterHotkey();
    }

    // 注册指定的热键，使用窗口句柄
    bool success = RegisterHotKey(reinterpret_cast<HWND>(winId()), m_hotkeyId, modifiers, key);
    if (success) {
        m_registered = true;
    }
    return success;
#else
    Q_UNUSED(key)
    Q_UNUSED(modifiers)
    return false;
#endif
}

void GlobalHotkey::unregisterHotkey()
{
#ifdef Q_OS_WIN
    if (m_registered) {
        UnregisterHotKey(reinterpret_cast<HWND>(winId()), m_hotkeyId);
        m_registered = false;
    }
#endif
}

bool GlobalHotkey::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY) {
            if (msg->wParam == m_hotkeyId) {
                emit hotkeyPressed();
                return true;
            }
        }
    }
#endif
    Q_UNUSED(result)
    return false;
}

bool GlobalHotkey::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY) {
            if (msg->wParam == m_hotkeyId) {
                emit hotkeyPressed();
                return true;
            }
        }
    }
#endif
    Q_UNUSED(result)
    return false;
}

void GlobalHotkey::setHotkey(int key) {
    if (m_registered) {
        unregisterHotkey();
    }
    
    bool success = registerHotkey(key, 0);
    if (success) {
        m_registered = true;
        qDebug() << "[Hotkey] Hotkey registered successfully for key:" << key;
    } else {
        qDebug() << "[Hotkey] Failed to register hotkey for key:" << key;
    }
}

void GlobalHotkey::setKeyF1() {
    setHotkey(VK_F1);
}

void GlobalHotkey::setKeyF2() {
    setHotkey(VK_F2);
}

void GlobalHotkey::setKeyF3() {
    setHotkey(VK_F3);
}
