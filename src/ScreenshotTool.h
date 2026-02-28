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
 * @file ScreenshotTool.h
 * @brief 截图工具核心类
 * 
 * 负责处理截图的核心逻辑，提供全屏截图、区域截图、多屏截图等功能
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#ifndef SCREENSHOTTOOL_H
#define SCREENSHOTTOOL_H

#include <QObject>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include <QList>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

class RegionSelector;
class ScreenshotEditWindow;
class StickyNoteWindow;

/**
 * @struct ScreenCaptureInfo
 * @brief 屏幕截图信息结构体
 * 
 * 用于存储单个屏幕的截图数据和其在全局坐标系中的位置
 */
struct ScreenCaptureInfo {
    QPixmap pixmap;        ///< 屏幕截图
    QPoint globalPos;      ///< 在全局坐标系中的位置
};

/**
 * @class ScreenshotTool
 * @brief 截图工具核心类
 * 
 * 负责处理截图的核心逻辑，提供以下功能：
 * - 全屏截图功能，支持高DPI显示器
 * - 区域截图功能，通过区域选择器选择特定区域
 * - 多屏截图功能，支持跨屏区域截图
 * - 截图编辑窗口管理，显示截图后的操作界面
 * - 贴图窗口创建和管理
 * - 截图保存到文件系统
 * - 截图复制到系统剪贴板
 * - 信号机制通知主窗口状态变化
 */
class ScreenshotTool : public QObject
{
    Q_OBJECT

public:
    explicit ScreenshotTool(QObject *parent = nullptr);
    ~ScreenshotTool();

    /**
     * @brief 开始延迟截图（用于截取弹窗）
     * @param delayMs 延迟时间（毫秒）
     */
    void startDelayedCapture(int delayMs = 1000);

    /**
     * @brief 开始区域截图
     */
    void startRegionCapture();

    /**
     * @brief 执行全屏截图
     * @return 截图结果
     */
    QPixmap captureFullScreen();

    /**
     * @brief 捕获指定区域
     * @param globalRect 全局坐标矩形
     * @return 截图结果
     */
    QPixmap captureRegion(const QRect &globalRect);

    /**
     * @brief 保存截图到文件
     * @param screenshot 要保存的截图
     * @param filePath 文件路径
     * @return 是否保存成功
     */
    bool saveScreenshot(const QPixmap &screenshot, const QString &filePath);

    /**
     * @brief 复制截图到剪贴板
     * @param screenshot 要复制的截图
     */
    void copyScreenshot(const QPixmap &screenshot);

    /**
     * @brief 创建贴图窗口
     * @param screenshot 要贴图的截图
     * @param position 贴图位置
     */
    void createStickyNote(const QPixmap &screenshot, const QPoint &position);
    
    /**
     * @brief 多屏截图图像合成
     * @param captures 屏幕截图信息列表
     * @param logicalSize 逻辑尺寸
     * @return 合成后的截图
     */
    QPixmap mergeMultiScreenCaptures(const QList<ScreenCaptureInfo> &captures, const QSize &logicalSize);
    
    /**
     * @brief 将截图缩放到逻辑尺寸
     * @param pixmap 原始截图
     * @param logicalSize 目标逻辑尺寸
     * @return 缩放后的截图
     */
    QPixmap scaleToLogicalSize(const QPixmap &pixmap, const QSize &logicalSize);

signals:
    /**
     * @brief 截图完成信号
     * @param screenshot 截图结果
     */
    void screenshotCaptured(const QPixmap &screenshot);

    /**
     * @brief 编辑窗口关闭信号
     */
    void editWindowClosed();

    /**
     * @brief 贴图窗口关闭信号
     */
    void stickyNoteClosed();

private slots:
    /**
     * @brief 区域选择完成处理
     * @param rect 选择的区域
     */
    void onRegionSelected(const QRect &rect);

    /**
     * @brief 区域选择取消处理
     */
    void onSelectionCancelled();

public:
    /**
     * @brief 显示截图编辑窗口
     * @param screenshot 截图
     * @param position 窗口位置
     */
    void showScreenshotEditWindow(const QPixmap &screenshot, const QPoint &position);

private:
    /**
     * @brief 生成默认文件名
     * @return 文件名
     */
    QString generateDefaultFileName() const;

    /**
     * @brief 获取截图保存目录
     * @return 保存目录路径
     */
    QString getScreenshotSaveDir() const;
    
    /**
     * @brief 保存截图到历史文件夹
     * @param screenshot 要保存的截图
     */
    void saveToHistory(const QPixmap &screenshot);

private:
    RegionSelector *m_regionSelector;    ///< 区域选择器
    ScreenshotEditWindow *m_editWindow;        ///< 编辑窗口
    QList<StickyNoteWindow*> m_stickyNotes;    ///< 贴图窗口列表
    QPoint m_lastCaptureTopLeft;               ///< 最近一次选区左上角
    QPoint m_lastEditPos;                      ///< 最近一次编辑窗口位置
    QTimer *m_delayedCaptureTimer;             ///< 延迟截图定时器
};

#endif // SCREENSHOTTOOL_H
