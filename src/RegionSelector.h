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
 * @file RegionSelector.h
 * @brief 区域选择器类
 * 
 * 提供区域选择功能，支持鼠标拖拽选择截图区域
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#ifndef REGIONSELECTOR_H
#define REGIONSELECTOR_H

#include <QWidget>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QScreen>
#include <QGuiApplication>

/**
 * @class RegionSelector
 * @brief 区域选择器类
 * 
 * 提供区域选择功能，支持以下特性：
 * - 全屏覆盖选择界面
 * - 鼠标拖拽选择区域
 * - 实时显示选择框
 * - 支持ESC取消选择
 * - 高DPI显示器支持
 * - 多屏环境支持
 */
class RegionSelector : public QWidget
{
    Q_OBJECT

public:
    explicit RegionSelector(QWidget *parent = nullptr);

    /**
     * @brief 获取选择区域
     * @return 选择区域的全局坐标矩形
     */
    QRect getSelectionRect() const;
    
    /**
     * @brief 获取冻结的背景画面
     * @return 冻结的背景图像
     */
    QPixmap getFrozenBackground() const { return m_frozenBackground; }
    
    /**
     * @brief 开始区域选择
     */
    void startSelection();

signals:
    /**
     * @brief 区域选择完成信号
     * @param rect 选择的区域矩形
     */
    void regionSelected(const QRect &rect);

    /**
     * @brief 选择取消信号
     */
    void selectionCancelled();

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

private:
    /**
     * @brief 获取当前屏幕矩形（鼠标所在屏幕）
     * @return 当前屏幕矩形
     */
    QRect getCurrentScreenRect() const;

    /**
     * @brief 将全局坐标矩形转换为本地坐标矩形
     * @param globalRect 全局坐标矩形
     * @return 本地坐标矩形
     */
    QRect globalToLocalRect(const QRect &globalRect) const;

    /**
     * @brief 修复混合DPI环境的坐标转换
     * @param globalRect 全局坐标矩形
     * @return 本地坐标矩形
     */
    QRect globalToLocalRectFixed(const QRect &globalRect) const;

    /**
     * @brief 绘制选择框的四个角
     * @param painter 绘图器
     * @param rect 选择框矩形
     */
    void drawSelectionCorners(QPainter &painter, const QRect &rect);

    /**
     * @brief 绘制调整手柄
     * @param painter 绘图器
     * @param rect 选择框矩形
     */
    void drawResizeHandles(QPainter &painter, const QRect &rect);

    /**
     * @brief 绘制尺寸信息
     * @param painter 绘图器
     * @param rect 选择框矩形
     */
    void drawSizeInfo(QPainter &painter, const QRect &rect);

    /**
     * @brief 绘制十字准星
     * @param painter 绘图器
     */
    void drawCrosshair(QPainter &painter);

    /**
     * @brief 绘制提示信息
     * @param painter 绘图器
     */
    void drawTipInfo(QPainter &painter);

    /**
     * @brief 清理并关闭窗口
     */
    void cleanupAndClose();

    /**
     * @brief 检测鼠标位置在选择框的哪个部分
     * @param pos 鼠标位置（全局坐标）
     * @return 0-无, 1-8边和角, 9-内部
     */
    int hitTest(const QPoint &pos) const;

    /**
     * @brief 根据hit位置更新光标
     * @param hit hit测试结果
     */
    void updateCursor(int hit);

private:
    // 选择状态
    enum SelectState {
        StateIdle,          ///< 空闲状态
        StateSelecting,     ///< 正在选择
        StateSelected,      ///< 选择完成
        StateResizing       ///< 正在调整大小
    };

    SelectState m_state;            ///< 当前状态
    bool m_isSelecting;             ///< 是否正在选择（兼容旧代码）
    QPoint m_startPoint;            ///< 起始点（本地坐标）
    QPoint m_endPoint;              ///< 结束点（本地坐标）
    QRect m_selectionRect;          ///< 选择矩形（本地坐标）
    QPoint m_startGlobal;           ///< 起始点（全局坐标）
    QPoint m_endGlobal;             ///< 结束点（全局坐标）
    QRect m_globalSelection;        ///< 选择矩形（全局坐标）
    
    // 调整大小相关
    int m_resizeHandle;             ///< 当前调整句柄（0-无, 1-8边和角, 9-移动）
    QPoint m_resizeStartPos;        ///< 调整开始位置
    QRect m_resizeStartRect;        ///< 调整开始时的矩形
    
    // 画面冻结相关
    QPixmap m_frozenBackground;     ///< 冻结的背景画面
    QScreen *m_currentScreen;       ///< 当前操作的屏幕
    
    /**
     * @brief 捕获当前屏幕背景（冻结画面）
     */
    void captureScreenBackground();
};

#endif // REGIONSELECTOR_H
