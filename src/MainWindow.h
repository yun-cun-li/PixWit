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
 * @file MainWindow.h
 * @brief 应用程序主窗口类
 * 
 * 应用程序的主界面，提供截图模式选择和系统托盘功能
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QLocalServer>

class ScreenshotTool;
class GlobalHotkey;
class UpdateChecker;

/**
 * @class MainWindow
 * @brief 应用程序主窗口类
 * 
 * 应用程序的主界面，提供以下功能：
 * - 现代化的渐变背景设计
 * - 全屏截图和区域截图模式选择
 * - 无边框窗口设计，支持最小化
 * - 支持鼠标拖拽移动窗口
 * - 默认位置在桌面右边缘中间
 * - 截图完成后自动隐藏，编辑完成后重新显示
 * - 与截图工具核心类的信号槽通信
 * - 始终置顶显示，方便快速访问
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief 绘制事件处理
     * @param event 绘制事件对象
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件对象
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标移动事件处理
     * @param event 鼠标事件对象
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件对象
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件对象
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief 关闭事件处理
     * @param event 关闭事件对象
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param obj 目标对象
     * @param event 事件对象
     * @return 是否处理了事件
     */
    bool eventFilter(QObject *obj, QEvent *event) override;
    
    /**
     * @brief 显示事件处理
     * @param event 显示事件对象
     */
    void showEvent(QShowEvent *event) override;
    
    /**
     * @brief 隐藏事件处理
     * @param event 隐藏事件对象
     */
    void hideEvent(QHideEvent *event) override;

private slots:
    /**
     * @brief 全屏截图按钮点击处理
     */
    void onFullScreenCapture();

    /**
     * @brief 区域截图按钮点击处理
     */
    void onRegionCapture();
    
    /**
     * @brief 处理来自其他实例的连接
     */
    void onNewConnection();

    /**
     * @brief 延迟截图处理（用于截取弹出窗口）
     */
    void onDelayedCapture();

    /**
     * @brief 截图完成处理
     * @param screenshot 截图结果
     */
    void onScreenshotCaptured(const QPixmap &screenshot);

    /**
     * @brief 编辑窗口关闭处理
     */
    void onEditWindowClosed();

    /**
     * @brief 系统托盘图标激活处理
     * @param reason 激活原因
     */
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    
    /**
     * @brief 设置按钮样式
     * @param regionBtn 区域截图按钮
     * @param minimizeBtn 最小化按钮
     */
    void setupStyles(QPushButton *regionBtn, QPushButton *minimizeBtn);
    
    /**
     * @brief 显示关闭菜单
     * @param pos 菜单显示位置
     */
    void showCloseMenu(const QPoint &pos);

    /**
     * @brief 退出应用程序
     */
    void onQuit();

    /**
     * @brief 显示主窗口
     */
    void onShow();
    
    /**
     * @brief 打开历史截图文件夹
     */
    void onOpenHistoryFolder();

private:
    /**
     * @brief 初始化用户界面
     */
    void setupUI();

    /**
     * @brief 连接信号槽
     */
    void setupConnections();

    /**
     * @brief 设置系统托盘
     */
    void setupTray();

    /**
     * @brief 设置自动更新检查器
     */
    void setupUpdateChecker();

    /**
     * @brief 移动到桌面右边缘中间位置
     */
    void moveToRightEdge();

    /**
     * @brief 绘制渐变背景
     * @param painter 绘图器
     */
    void drawGradientBackground(QPainter &painter);

    /**
     * @brief 绘制窗口边框
     * @param painter 绘图器
     */
    void drawWindowBorder(QPainter &painter);

    /**
     * @brief 检查是否启用开机启动
     * @return true 已启用，false 未启用
     */
    bool isAutoStartEnabled();

    /**
     * @brief 设置开机启动
     * @param enabled true 启用，false 禁用
     */
    void setAutoStartEnabled(bool enabled);
    
    /**
     * @brief 获取历史截图文件夹路径
     * @return 历史截图文件夹路径
     */
    static QString getHistoryFolderPath();

private:
    ScreenshotTool *m_screenshotTool;    ///< 截图工具核心
    GlobalHotkey *m_hotkey;                   ///< 全局热键
    UpdateChecker *m_updateChecker;           ///< 自动更新检查器

    // UI组件
    QPushButton *m_fullScreenBtn;             ///< 全屏截图按钮
    QPushButton *m_regionBtn;                 ///< 区域截图按钮
    QPushButton *m_minimizeBtn;               ///< 最小化按钮
    QLabel *m_titleLabel;                     ///< 标题标签

    // 系统托盘
    QSystemTrayIcon *m_trayIcon;              ///< 系统托盘图标
    QMenu *m_trayMenu;                        ///< 托盘菜单
    QAction *m_showAction;                    ///< 显示动作
    QAction *m_quitAction;                    ///< 退出动作
    QAction *m_toggleShowAction;              ///< 切换显示/隐藏动作

    // 拖拽状态
    bool m_isDragging;                        ///< 是否正在拖拽
    QPoint m_dragOffset;                      ///< 拖拽偏移量
    QPoint m_lastPosition;                    ///< 记录隐藏前的位置

    // 窗口状态
    bool m_isMinimized;                       ///< 是否已最小化
    
    // 截图相关
    QPixmap m_currentScreenshot;              ///< 当前截图
    
    // 系统托盘（兼容性）
    QSystemTrayIcon *m_tray;                  ///< 系统托盘图标
    
    // 单实例通信
    QLocalServer *m_localServer;              ///< 本地服务器，用于接收其他实例的通知
};

#endif // MAINWINDOW_H