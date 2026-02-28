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
 * @file GlobalHotkey.h
 * @brief 全局热键管理类
 * 
 * 负责注册和管理全局热键，支持跨应用程序的快捷键响应
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#ifndef GLOBALHOTKEY_H
#define GLOBALHOTKEY_H

#include <QObject>
#include <QWidget>
#include <QAbstractNativeEventFilter>

/**
 * @class GlobalHotkey
 * @brief 全局热键管理类
 * 
 * 使用Windows API实现全局热键注册和响应
 */
class GlobalHotkey : public QWidget, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit GlobalHotkey(QWidget *parent = nullptr);
    ~GlobalHotkey();

    /**
     * @brief 注册全局热键
     * @param key 虚拟键码
     * @param modifiers 修饰键组合
     * @return 是否注册成功
     */
    bool registerHotkey(int key, int modifiers);

    /**
     * @brief 注销全局热键
     */
    void unregisterHotkey();
    
    /**
     * @brief 设置热键
     * @param key 热键代码
     */
    void setHotkey(int key);
    
    /**
     * @brief 设置F1热键
     */
    void setKeyF1();
    
    /**
     * @brief 设置F2热键
     */
    void setKeyF2();
    
    /**
     * @brief 设置F3热键
     */
    void setKeyF3();

signals:
    /**
     * @brief 热键被按下时发出的信号
     */
    void hotkeyPressed();

protected:
    /**
     * @brief Windows消息处理
     * @param message 消息类型
     * @param result 处理结果
     * @return 是否处理了消息
     */
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    /**
     * @brief 原生事件过滤器
     * @param eventType 事件类型
     * @param message 消息指针
     * @param result 结果指针
     * @return 是否处理了事件
     */
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

private:
    bool m_registered;  ///< 是否已注册热键
    int m_hotkeyId;     ///< 热键ID
};

#endif // GLOBALHOTKEY_H
