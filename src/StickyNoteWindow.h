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
 * @file StickyNoteWindow.h
 * @brief 便签式贴图窗口类
 * 
 * 提供便签式贴图功能，支持拖拽、缩放、调整大小等操作
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#ifndef STICKYNOTEWINDOW_H
#define STICKYNOTEWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QSize>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QMenu>
#include <QAction>
#include <QVariantAnimation>
#include <QEasingCurve>

/**
 * @class StickyNoteWindow
 * @brief 便签式贴图窗口类
 * 
 * 提供便签式贴图功能，支持以下特性：
 * - 显示截图内容，保持原始尺寸
 * - 支持窗口拖拽移动
 * - 支持鼠标拖拽调整窗口大小（8个控制点）
 * - 支持鼠标滚轮缩放（10%-500%）
 * - 支持键盘快捷键（Ctrl+S保存，Ctrl+C复制，Ctrl+0重置缩放等）
 * - 右键菜单提供完整操作选项
 * - 可选择是否始终置顶显示
 * - 高质量图片缩放和渲染
 */
class StickyNoteWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param pixmap 要显示的截图
     * @param parent 父窗口指针
     */
    explicit StickyNoteWindow(const QPixmap &pixmap, QWidget *parent = nullptr);

    /**
     * @brief 获取当前截图
     * @return 截图的QPixmap对象
     */
    QPixmap getPixmap() const;
    
    /**
     * @brief 设置截图
     * @param pixmap 新的截图
     */
    void setPixmap(const QPixmap &pixmap);

    /**
     * @brief 设置始终置顶
     * @param onTop 是否置顶
     */
    void setAlwaysOnTop(bool onTop);

    /**
     * @brief 切换始终置顶状态
     */
    void toggleAlwaysOnTop();

signals:
    /**
     * @brief 关闭请求信号
     */
    void closeRequested();
    
    /**
     * @brief 保存请求信号
     */
    void saveRequested();
    
    /**
     * @brief 复制请求信号
     */
    void copyRequested();

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
     * @brief 鼠标双击事件处理
     * @param event 鼠标事件对象
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件对象
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief 上下文菜单事件处理
     * @param event 上下文菜单事件对象
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

    /**
     * @brief 滚轮事件处理
     * @param event 滚轮事件对象
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * @brief 窗口大小改变事件处理
     * @param event 大小改变事件对象
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief 关闭按钮点击处理
     */
    void onCloseClicked();

    /**
     * @brief 缩放动画完成处理
     */
    void onScaleAnimationFinished();

private:
    /**
     * @brief 设置上下文菜单
     */
    void setupContextMenu();

    /**
     * @brief 更新控制点
     */
    void updateHandles();

    /**
     * @brief 绘制控制点
     * @param painter 绘图器
     */
    void drawHandles(QPainter &painter);

    /**
     * @brief 检查点是否在控制点内
     * @param point 要检查的点
     * @param handle 控制点
     * @return 是否在控制点内
     */
    bool isPointInHandle(const QPoint &point, const QPoint &handle) const;

    /**
     * @brief 更新光标
     * @param pos 鼠标位置
     */
    void updateCursor(const QPoint &pos);

    /**
     * @brief 调整窗口大小
     * @param pos 鼠标位置
     */
    void resizeWindow(const QPoint &pos);

    /**
     * @brief 设置缩放动画
     */
    void setupScaleAnimation();

    /**
     * @brief 开始平滑缩放
     * @param targetScale 目标缩放因子
     */
    void startSmoothScale(float targetScale);

    /**
     * @brief 设置缩放因子
     * @param scale 缩放因子
     */
    void setScaleFactor(float scale);

    /**
     * @brief 更新窗口大小
     */
    void updateWindowSize();

    /**
     * @brief 应用缩放因子
     * @param scale 缩放因子
     * @param duringAnimation 是否在动画中
     */
    void applyScaleFactor(float scale, bool duringAnimation);

    /**
     * @brief 保存按钮点击处理
     */
    void onSave();

    /**
     * @brief 复制按钮点击处理
     */
    void onCopy();

    /**
     * @brief 最小化按钮点击处理
     */
    void onMinimize();

    /**
     * @brief 最大化按钮点击处理
     */
    void onMaximize();

    /**
     * @brief 切换置顶按钮点击处理
     */
    void onToggleAlwaysOnTop();

    /**
     * @brief 重置缩放按钮点击处理
     */
    void onResetScale();

private:
    // 图片相关成员变量
    QPixmap m_pixmap;              // 当前显示的截图
    QSize m_originalSize;          // 截图原始尺寸
    float m_scaleFactor;           // 当前缩放比例（1.0为原始大小）
    
    // 拖拽相关成员变量
    bool m_isDragging;             // 是否正在拖拽窗口
    QPoint m_dragOffset;           // 拖拽偏移量
    
    // 调整大小相关成员变量
    bool m_isResizing;             // 是否正在调整大小
    int m_resizeHandle;            // 当前调整大小的控制点索引（-1表示无）
    QPoint m_startPoint;           // 调整大小的起始点
    QList<QPoint> m_handles;       // 调整大小的控制点列表（8个点）
    int m_handleSize;              // 控制点的尺寸（像素）
    QSize m_minSize;               // 窗口最小尺寸限制
    
    // UI相关成员变量
    QMenu *m_contextMenu;          // 右键上下文菜单

    // 平滑缩放
    bool m_smoothScaling;
    float m_targetScaleFactor;
    QVariantAnimation *m_scaleAnimation;
    bool m_animationInProgress;
};

#endif // STICKYNOTEWINDOW_H