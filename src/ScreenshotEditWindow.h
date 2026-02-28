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
 * @file ScreenshotEditWindow.h
 * @brief 截图编辑窗口类
 * 
 * 截图完成后显示的编辑窗口，提供保存、复制、创建贴图、关闭等功能
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#ifndef SCREENSHOTEDITWINDOW_H
#define SCREENSHOTEDITWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>

// 前向声明
class StylePopover;

/**
 * @class ScreenshotEditWindow
 * @brief 截图编辑窗口类
 * 
 * 截图完成后显示的编辑窗口，提供以下功能：
 * - 显示截图预览
 * - 提供保存、复制、创建贴图、关闭四个操作按钮
 * - 支持窗口拖拽移动
 * - 支持键盘快捷键操作（Ctrl+S保存，Ctrl+C复制，ESC关闭）
 * - 无边框设计，始终置顶显示
 */
class ScreenshotEditWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param screenshot 要显示的截图
     * @param initialPos 初始位置
     * @param parent 父窗口指针
     */
    explicit ScreenshotEditWindow(const QPixmap &screenshot, const QPoint &initialPos = QPoint(), QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~ScreenshotEditWindow();

    /**
     * @brief 获取当前截图
     * @return 截图的QPixmap对象
     */
    QPixmap getScreenshot() const;

signals:
    /**
     * @brief 保存请求信号
     * @param screenshot 要保存的截图
     */
    void saveRequested(const QPixmap &screenshot);
    
    /**
     * @brief 复制请求信号
     * @param screenshot 要复制的截图
     */
    void copyRequested(const QPixmap &screenshot);
    
    /**
     * @brief 创建贴图请求信号
     * @param screenshot 要创建贴图的截图
     */
    void createStickyNoteRequested(const QPixmap &screenshot);
    
    /**
     * @brief 关闭请求信号
     */
    void closeRequested();

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
     * @brief 窗口移动事件处理
     * @param event 移动事件对象
     */
    void moveEvent(QMoveEvent *event) override;

private slots:
    /**
     * @brief 保存按钮点击处理
     */
    void onSaveClicked();

    /**
     * @brief 复制按钮点击处理
     */
    void onCopyClicked();

    /**
     * @brief 贴图按钮点击处理
     */
    void onStickyNoteClicked();

    /**
     * @brief 关闭按钮点击处理
     */
    void onCloseClicked();

    /**
     * @brief 工具按钮点击处理
     * @param id 工具ID
     */
    void onToolButtonClicked(int id);

    /**
     * @brief 颜色选择处理
     * @param color 选中的颜色
     */
    void onColorSelected(const QColor &color);

    /**
     * @brief 粗细选择处理
     * @param thickness 选中的粗细
     */
    void onThicknessSelected(int thickness);

    /**
     * @brief 文本编辑完成处理
     */
    void onTextEditFinished();
    
    /**
     * @brief 在指定位置开始文本输入
     * @param pos 输入位置
     */
    void startTextInputAt(const QPoint &pos);
    
    /**
     * @brief 应用马赛克效果
     * @param center 马赛克中心点
     */
    void applyMosaicAt(const QPoint &center);

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
     * @brief 显示样式选择弹窗
     * @param anchorBtn 锚点按钮
     */
    void showStylePopover(QPushButton *anchorBtn);

    /**
     * @brief 隐藏样式选择弹窗
     */
    void hideStylePopover();

    /**
     * @brief 开始绘制模式
     * @param toolType 工具类型
     */
    void startDrawingMode(int toolType);

    /**
     * @brief 结束绘制模式
     */
    void endDrawingMode();

    /**
     * @brief 绘制工具图形
     * @param painter 绘图器
     * @param startPos 起始位置
     * @param endPos 结束位置
     * @param toolType 工具类型
     */
    void drawTool(QPainter &painter, const QPoint &startPos, const QPoint &endPos, int toolType);

    /**
     * @brief 更新窗口大小以适应DPI变化
     */
    void updateWindowSizeForDPI();

    /**
     * @brief 重新定位按钮位置
     */
    void repositionButtons();

    /**
     * @brief 确保所有按钮可见（用于快速多屏移动时防止按钮消失）
     */
    void ensureButtonsVisible();

    /**
     * @brief 显示右键菜单
     * @param pos 菜单显示位置
     */
    void showContextMenu(const QPoint &pos);
    
    /**
     * @brief 确保窗口在屏幕可见范围内
     */
    void ensureWindowInScreen();

private:
    QPixmap m_screenshot;           ///< 当前截图
    QPixmap m_originalScreenshot;   ///< 原始截图备份
    QPixmap m_drawingLayer;         ///< 绘制层
    QSize m_originalScreenshotSize; ///< 原始截图显示尺寸（用于DPI自适应）
    QList<QPixmap> m_undoStack;     ///< 撤销历史栈

    // UI组件
    QLabel *m_imageLabel;           ///< 图片显示标签
    QPushButton *m_undoBtn;         ///< 撤销按钮
    QPushButton *m_saveBtn;         ///< 保存按钮
    QPushButton *m_copyBtn;         ///< 复制按钮
    QPushButton *m_stickyBtn;       ///< 贴图按钮
    QPushButton *m_closeBtn;        ///< 关闭按钮

    // 工具按钮
    QPushButton *m_penToolBtn;      ///< 画笔工具按钮
    QPushButton *m_lineToolBtn;     ///< 直线工具按钮
    QPushButton *m_arrowToolBtn;    ///< 箭头工具按钮
    QPushButton *m_circleToolBtn;   ///< 圆形工具按钮
    QPushButton *m_rectToolBtn;     ///< 矩形工具按钮
    QPushButton *m_mosaicToolBtn;   ///< 马赛克工具按钮
    QPushButton *m_textToolBtn;     ///< 文本工具按钮
    QButtonGroup *m_toolGroup;      ///< 工具按钮组

    // 样式选择
    StylePopover *m_stylePopover;   ///< 样式选择弹窗
    QColor m_currentColor;          ///< 当前颜色
    int m_currentThickness;         ///< 当前粗细

    // 绘制状态
    bool m_isDrawing;               ///< 是否正在绘制
    int m_currentTool;              ///< 当前工具
    QPoint m_drawStartPos;          ///< 绘制起始位置
    QPoint m_drawEndPos;            ///< 绘制结束位置
    QList<QPair<QPoint, QPoint>> m_drawHistory; ///< 绘制历史

    // 文本编辑
    QLineEdit *m_textEdit;          ///< 文本编辑框
    bool m_isTextEditing;           ///< 是否正在编辑文本

    // 拖拽状态
    bool m_isDragging;              ///< 是否正在拖拽
    QPoint m_dragOffset;            ///< 拖拽偏移量
    
    // DPI更新状态
    bool m_isUpdatingDPI;           ///< 是否正在更新DPI（防止重复更新）
    QScreen *m_lastScreen;          ///< 上次所在的屏幕
    QTimer *m_dpiUpdateTimer;       ///< DPI更新延迟定时器（避免快速移动时重复触发）
    
    // 对象状态
    bool m_isClosing;               ///< 是否正在关闭（防止重复处理）
};

#endif // SCREENSHOTEDITWINDOW_H
