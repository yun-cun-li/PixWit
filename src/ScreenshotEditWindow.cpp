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
 * @file ScreenshotEditWindow.cpp
 * @brief 截图编辑窗口类实现
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include "ScreenshotEditWindow.h"
#include "StylePopover.h"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QMoveEvent>
#include <QFontMetrics>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QMenu>
#include <cmath>

ScreenshotEditWindow::ScreenshotEditWindow(const QPixmap &screenshot, const QPoint &initialPos, QWidget *parent)
    : QWidget(parent)
    , m_screenshot(screenshot)
    , m_originalScreenshot(screenshot)
    , m_imageLabel(nullptr)
    , m_undoBtn(nullptr)
    , m_saveBtn(nullptr)
    , m_copyBtn(nullptr)
    , m_stickyBtn(nullptr)
    , m_closeBtn(nullptr)
    , m_penToolBtn(nullptr)
    , m_lineToolBtn(nullptr)
    , m_arrowToolBtn(nullptr)
    , m_circleToolBtn(nullptr)
    , m_rectToolBtn(nullptr)
    , m_mosaicToolBtn(nullptr)
    , m_textToolBtn(nullptr)
    , m_toolGroup(nullptr)
    , m_stylePopover(nullptr)
    , m_currentColor(QColor(255, 0, 0))
    , m_currentThickness(3)
    , m_isDrawing(false)
    , m_currentTool(0)
    , m_drawStartPos()
    , m_drawEndPos()
    , m_drawHistory()
    , m_textEdit(nullptr)
    , m_isTextEditing(false)
    , m_isDragging(false)
    , m_dragOffset()
    , m_isUpdatingDPI(false)
    , m_lastScreen(nullptr)
    , m_dpiUpdateTimer(nullptr)
    , m_undoStack()
    , m_isClosing(false)
{
    // 设置窗口属性：无边框、置顶、工具窗口
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);  // 透明背景
    setFocusPolicy(Qt::StrongFocus);             // 强焦点策略，支持键盘事件
    
    // 先计算并存储原始截图尺寸，确保setupUI可以使用
    QSize displaySize = m_screenshot.size() / m_screenshot.devicePixelRatio();
    m_originalScreenshotSize = displaySize;
    qDebug() << "[ScreenshotEditWindow] Original screenshot size:" << m_originalScreenshotSize 
             << "DPR:" << m_screenshot.devicePixelRatio();
    
    setupUI();          // 初始化用户界面
    setupConnections(); // 连接信号槽
    
    // 初始化DPI更新定时器（单次触发，避免快速移动时重复更新）
    m_dpiUpdateTimer = new QTimer(this);
    m_dpiUpdateTimer->setSingleShot(true);
    m_dpiUpdateTimer->setInterval(50); // 50ms延迟，快速响应
    connect(m_dpiUpdateTimer, &QTimer::timeout, this, &ScreenshotEditWindow::updateWindowSizeForDPI);
    
    // 设置窗口大小：图片显示尺寸 + 按钮区域（考虑设备像素比）
    QSize windowSize(displaySize.width(), displaySize.height() + 50); // 为按钮预留50像素高度
    
    // 清除最大尺寸限制，允许窗口自由放大
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    
    // 清除布局的尺寸限制（布局管理器也会影响窗口最小尺寸）
    QLayout *mainLayout = layout();
    if (mainLayout) {
        mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    }
    
    // 计算合理的最小窗口尺寸：
    // 使用图标按钮后尺寸大幅减小：12×28px + 11×4px(间距) + 20px(边距) ≈ 400px
    // 设置420px确保有足够空间，图片居中显示
    int minWindowWidth = qMax(420, displaySize.width());
    int minWindowHeight = displaySize.height() + 50;
    setMinimumSize(minWindowWidth, minWindowHeight);
    
    resize(windowSize);
    
    qDebug() << "[ScreenshotEditWindow] Set window size to:" << windowSize;
    
    // 如果提供了初始位置，则使用；否则不主动移动，避免位置被重置
    if (!initialPos.isNull()) {
        move(initialPos);
    }
    
    // 确保窗口在屏幕可见范围内
    ensureWindowInScreen();
    
    // 初始化当前屏幕记录
    m_lastScreen = QGuiApplication::screenAt(pos());
    if (!m_lastScreen) {
        m_lastScreen = QGuiApplication::primaryScreen();
    }
    qDebug() << "[ScreenshotEditWindow] Initial screen:" << m_lastScreen->name();
}

ScreenshotEditWindow::~ScreenshotEditWindow() {
    // 设置关闭标志，防止在析构过程中处理事件
    m_isClosing = true;
    
    // 清理文本编辑框
    if (m_textEdit) {
        m_textEdit->deleteLater();
        m_textEdit = nullptr;
    }
    
    // 清理样式弹窗
    if (m_stylePopover) {
        m_stylePopover->deleteLater();
        m_stylePopover = nullptr;
    }
    
    qDebug() << "[ScreenshotEditWindow] Destructor called";
}

QPixmap ScreenshotEditWindow::getScreenshot() const {
    return m_screenshot;
}

void ScreenshotEditWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // 高质量缩放
    
    // 设置颜色管理，保持原始颜色不变
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    
    // 绘制截图内容，只在图片区域内绘制
    if (!m_screenshot.isNull()) {
        // 计算实际显示尺寸（考虑设备像素比）
        QSize displaySize = m_screenshot.size() / m_screenshot.devicePixelRatio();
        QRect imageRect(0, 0, displaySize.width(), displaySize.height());
        
        // 限制绘制区域为图片区域，不覆盖按钮区域
        painter.setClipRect(imageRect);
        painter.drawPixmap(imageRect, m_screenshot);
        painter.setClipping(false); // 取消裁剪限制
        
        // 仅用于界面显示的虚线边框（不影响保存/复制/贴图的图片内容）
        QPen dashPen(QColor(160, 160, 160, 220));
        dashPen.setStyle(Qt::DashLine);
        dashPen.setWidth(2);
        painter.setPen(dashPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(imageRect.adjusted(1, 1, -1, -1));

        // 绘制预览中的图形（仅显示不落盘）
        if (m_isDrawing && m_currentTool != 0) {
            QPen previewPen(m_currentColor);
            previewPen.setWidth(m_currentThickness);
            previewPen.setCapStyle(Qt::RoundCap);
            previewPen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(previewPen);
            
            if (m_currentTool == 1 && m_drawHistory.size() > 1) { // 画笔工具
                for (int i = 1; i < m_drawHistory.size(); ++i) {
                    painter.drawLine(m_drawHistory[i-1].first, m_drawHistory[i].first);
                }
            } else if (m_currentTool == 2) { // 直线工具
                painter.drawLine(m_drawStartPos, m_drawEndPos);
            } else if (m_currentTool == 3) { // 箭头工具
                painter.drawLine(m_drawStartPos, m_drawEndPos);
                // 预览箭头
                auto drawHead = [&](const QPoint &from, const QPoint &to){
                    QLineF line(from, to);
                    double angle = std::atan2(-line.dy(), line.dx());
                    double len = 12.0;
                    QPointF p1 = QPointF(to) + QPointF(-len * std::cos(angle + 3.14159265358979323846/6.0), len * std::sin(angle + 3.14159265358979323846/6.0));
                    QPointF p2 = QPointF(to) + QPointF(-len * std::cos(angle - 3.14159265358979323846/6.0), len * std::sin(angle - 3.14159265358979323846/6.0));
                    painter.drawLine(to, p1.toPoint());
                    painter.drawLine(to, p2.toPoint());
                };
                drawHead(m_drawStartPos, m_drawEndPos);
            } else if (m_currentTool == 4) { // 圆形工具
                QRect rect = QRect(m_drawStartPos, m_drawEndPos).normalized();
                painter.drawEllipse(rect);
            } else if (m_currentTool == 5) { // 矩形工具
                QRect rect = QRect(m_drawStartPos, m_drawEndPos).normalized();
                painter.drawRect(rect);
            }
        }
        // 按钮区域不绘制任何背景，保持完全透明
    }
}

void ScreenshotEditWindow::mousePressEvent(QMouseEvent *event) {
    // 处理右键点击 - 显示快捷菜单
    if (event->button() == Qt::RightButton) {
        showContextMenu(event->pos());
        return;
    }
    
    if (event->button() != Qt::LeftButton) return;
    QSize displaySize = m_screenshot.size() / m_screenshot.devicePixelRatio();
    QRect imageRect(0, 0, displaySize.width(), displaySize.height());
    bool inImage = imageRect.contains(event->pos());
    
    if (inImage && m_currentTool != 0) {
        // 文本工具：点击即在图上原位出现内嵌输入框
        if (m_currentTool == 7) {
            m_drawStartPos = event->pos();  // 保存点击位置
            startDrawingMode(7);
            return;
        }
        // 在开始一次绘制前保存当前截图到撤销栈
        m_undoStack.append(m_screenshot.copy());
        // 限制撤销栈大小，避免内存占用过大
        if (m_undoStack.size() > 20) {
            m_undoStack.removeFirst();
        }
        qDebug() << "[Undo] Saved state, stack size:" << m_undoStack.size();
        
        m_isDrawing = true;
        m_drawStartPos = event->pos();
        m_drawEndPos = event->pos();
        m_drawHistory.clear();
        m_drawHistory.append(qMakePair(event->pos(), event->pos()));
    } else {
        m_dragOffset = event->pos();
        m_isDragging = true;
    }
}

void ScreenshotEditWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDrawing && (event->buttons() & Qt::LeftButton)) {
        m_drawEndPos = event->pos();
        if (m_currentTool == 1) { // 画笔工具
            m_drawHistory.append(qMakePair(event->pos(), event->pos()));
        } else if (m_currentTool == 6) { // 马赛克工具
            // 马赛克工具：拖动过程中即时生效
            applyMosaicAt(event->pos());
        }
        update();
        return;
    }
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        QPoint newPos = event->globalPosition().toPoint() - m_dragOffset;  // 计算新位置
        move(newPos);  // 移动窗口
    }
}

void ScreenshotEditWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (m_isDrawing && event->button() == Qt::LeftButton) {
        // 将绘制内容落盘到 m_screenshot
        QPainter p(&m_screenshot);
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen(m_currentColor);
        pen.setWidth(m_currentThickness);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);
        
        if (m_currentTool == 1 && m_drawHistory.size() > 1) { // 画笔工具
            for (int i = 1; i < m_drawHistory.size(); ++i) {
                p.drawLine(m_drawHistory[i-1].first, m_drawHistory[i].first);
            }
        } else if (m_currentTool == 2) { // 直线工具
            p.drawLine(m_drawStartPos, m_drawEndPos);
        } else if (m_currentTool == 3) { // 箭头工具
            QPoint s = m_drawStartPos;
            QPoint e = m_drawEndPos;
            p.drawLine(s, e);
            QLineF line(s, e);
            double angle = std::atan2(-line.dy(), line.dx());
            double len = 18.0;
            QPointF p1 = QPointF(e) + QPointF(-len * std::cos(angle + 3.14159265358979323846/6.0), len * std::sin(angle + 3.14159265358979323846/6.0));
            QPointF p2 = QPointF(e) + QPointF(-len * std::cos(angle - 3.14159265358979323846/6.0), len * std::sin(angle - 3.14159265358979323846/6.0));
            p.drawLine(e, p1.toPoint());
            p.drawLine(e, p2.toPoint());
        } else if (m_currentTool == 4) { // 圆形工具
            QRect r = QRect(m_drawStartPos, m_drawEndPos).normalized();
            p.drawEllipse(r);
        } else if (m_currentTool == 5) { // 矩形工具
            QRect r = QRect(m_drawStartPos, m_drawEndPos).normalized();
            p.drawRect(r);
        }
        m_isDrawing = false;
        m_drawHistory.clear();
        update();
        return;
    }
    
    // 拖拽结束后，确保窗口在屏幕可见范围内
    if (m_isDragging) {
        ensureWindowInScreen();
    }
    
    Q_UNUSED(event)
    m_isDragging = false;  // 清除拖拽标志
}

void ScreenshotEditWindow::keyPressEvent(QKeyEvent *event) {
    // 如果正在关闭，忽略所有键盘事件
    if (m_isClosing) {
        event->ignore();
        return;
    }
    
    switch (event->key()) {
    case Qt::Key_Escape:
        // ESC键：优先取消工具选择，再关闭窗口
        if (m_currentTool != 0) {
            // 如果有工具被选中，取消工具选择
            qDebug() << "[KeyPress] ESC - Canceling tool selection";
            if (m_toolGroup) {
                QAbstractButton *checkedBtn = m_toolGroup->checkedButton();
                if (checkedBtn) {
                    checkedBtn->setChecked(false);
                }
            }
            m_currentTool = 0;
            m_isDrawing = false;
            m_isTextEditing = false;
            // 隐藏样式选择弹窗
            hideStylePopover();
            update();
        } else {
            // 没有工具选中，关闭窗口
            qDebug() << "[KeyPress] ESC - Closing window";
            m_isClosing = true;
            emit closeRequested();
        }
        break;
    case Qt::Key_S:
        if (event->modifiers() & Qt::ControlModifier) {
            emit saveRequested(m_screenshot);  // Ctrl+S保存截图
        }
        break;
    case Qt::Key_C:
        if (event->modifiers() & Qt::ControlModifier) {
            emit copyRequested(m_screenshot);  // Ctrl+C复制截图
        }
        break;
    case Qt::Key_W:
        if (event->modifiers() & Qt::ControlModifier) {
            // Ctrl+W 关闭窗口
            qDebug() << "[KeyPress] Ctrl+W - Closing window";
            m_isClosing = true;
            emit closeRequested();
        }
        break;
    case Qt::Key_D:
        if (event->modifiers() & Qt::ControlModifier) {
            // Ctrl+D 取消工具选择（类似ESC）
            qDebug() << "[KeyPress] Ctrl+D - Deselect tool";
            if (m_toolGroup) {
                QAbstractButton *checkedBtn = m_toolGroup->checkedButton();
                if (checkedBtn) {
                    checkedBtn->setChecked(false);
                }
            }
            m_currentTool = 0;
            m_isDrawing = false;
            m_isTextEditing = false;
            // 隐藏样式选择弹窗
            hideStylePopover();
            update();
        }
        break;
    case Qt::Key_Space:
        // 空格键：临时切换到移动模式（按住空格时可以拖动）
        if (!event->isAutoRepeat() && m_currentTool != 0) {
            qDebug() << "[KeyPress] Space - Temporary move mode";
            // 暂时记录当前工具，切换到移动模式
            event->accept();
        }
        break;
    default:
        QWidget::keyPressEvent(event);  // 其他按键交给父类处理
        break;
    }
}

void ScreenshotEditWindow::onSaveClicked() {
    emit saveRequested(m_screenshot);
}

void ScreenshotEditWindow::onCopyClicked() {
    emit copyRequested(m_screenshot);
}

void ScreenshotEditWindow::onStickyNoteClicked() {
    emit createStickyNoteRequested(m_screenshot);
}

void ScreenshotEditWindow::onCloseClicked() {
    if (!m_isClosing) {
        m_isClosing = true;
        emit closeRequested();
    }
}

void ScreenshotEditWindow::onToolButtonClicked(int id) {
    m_currentTool = id;
    if (id >= 1 && id <= 6) {
        QPushButton *anchor = nullptr;
        switch (id) {
        case 1: anchor = m_penToolBtn; break;
        case 2: anchor = m_lineToolBtn; break;
        case 3: anchor = m_arrowToolBtn; break;
        case 4: anchor = m_circleToolBtn; break;
        case 5: anchor = m_rectToolBtn; break;
        case 6: anchor = m_mosaicToolBtn; break;
        }
        if (anchor) {
            showStylePopover(anchor);
        }
    }
}

void ScreenshotEditWindow::onColorSelected(const QColor &color) {
    m_currentColor = color;
}

void ScreenshotEditWindow::onThicknessSelected(int thickness) {
    m_currentThickness = thickness;
}

void ScreenshotEditWindow::onTextEditFinished() {
    if (!m_textEdit) return;
    QString text = m_textEdit->text().trimmed();
    if (!text.isEmpty()) {
        // 保存当前状态到撤销栈
        m_undoStack.append(m_screenshot.copy());
        if (m_undoStack.size() > 20) {
            m_undoStack.removeFirst();
        }
        qDebug() << "[Undo] Saved state (text), stack size:" << m_undoStack.size();
        
        QPainter p(&m_screenshot);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(m_currentColor);
        QFont font; font.setPointSize(16); p.setFont(font);
        QFontMetrics fm(font);
        QPoint drawPoint = QPoint(m_drawStartPos.x(), m_drawStartPos.y() + fm.ascent());
        p.drawText(drawPoint, text);
    }
    m_textEdit->deleteLater();
    m_textEdit = nullptr;
    m_isTextEditing = false;
    update();
}

void ScreenshotEditWindow::setupUI() {
    // 工具按钮 - 使用SVG矢量图标，适配多DPI环境
    m_penToolBtn = new QPushButton(this);
    m_lineToolBtn = new QPushButton(this);
    m_arrowToolBtn = new QPushButton(this);
    m_circleToolBtn = new QPushButton(this);
    m_rectToolBtn = new QPushButton(this);
    m_mosaicToolBtn = new QPushButton(this);
    m_textToolBtn = new QPushButton(this);
    
    // 设置SVG图标 - 矢量图标在任何DPI下都清晰 (使用英文文件名避免编码问题)
    m_penToolBtn->setIcon(QIcon(":/icons/resources/icons/pen.svg"));
    m_lineToolBtn->setIcon(QIcon(":/icons/resources/icons/line.svg"));
    m_arrowToolBtn->setIcon(QIcon(":/icons/resources/icons/arrow.svg"));
    m_circleToolBtn->setIcon(QIcon(":/icons/resources/icons/circle.svg"));
    m_rectToolBtn->setIcon(QIcon(":/icons/resources/icons/rect.svg"));
    m_mosaicToolBtn->setIcon(QIcon(":/icons/resources/icons/mosaic.svg"));
    m_textToolBtn->setIcon(QIcon(":/icons/resources/icons/text.svg"));
    
    // 设置图标大小（SVG会自动缩放到此尺寸）
    QSize iconSize(20, 20);
    m_penToolBtn->setIconSize(iconSize);
    m_lineToolBtn->setIconSize(iconSize);
    m_arrowToolBtn->setIconSize(iconSize);
    m_circleToolBtn->setIconSize(iconSize);
    m_rectToolBtn->setIconSize(iconSize);
    m_mosaicToolBtn->setIconSize(iconSize);
    m_textToolBtn->setIconSize(iconSize);
    
    // 设置tooltip提示
    m_penToolBtn->setToolTip("画笔");
    m_lineToolBtn->setToolTip("直线");
    m_arrowToolBtn->setToolTip("箭头");
    m_circleToolBtn->setToolTip("圆形");
    m_rectToolBtn->setToolTip("矩形");
    m_mosaicToolBtn->setToolTip("马赛克");
    m_textToolBtn->setToolTip("文字");
    
    m_penToolBtn->setCheckable(true);
    m_lineToolBtn->setCheckable(true);
    m_arrowToolBtn->setCheckable(true);
    m_circleToolBtn->setCheckable(true);
    m_rectToolBtn->setCheckable(true);
    m_mosaicToolBtn->setCheckable(true);
    m_textToolBtn->setCheckable(true);
    
    m_toolGroup = new QButtonGroup(this);
    m_toolGroup->setExclusive(true);
    m_toolGroup->addButton(m_penToolBtn, 1);
    m_toolGroup->addButton(m_lineToolBtn, 2);
    m_toolGroup->addButton(m_arrowToolBtn, 3);
    m_toolGroup->addButton(m_circleToolBtn, 4);
    m_toolGroup->addButton(m_rectToolBtn, 5);
    m_toolGroup->addButton(m_mosaicToolBtn, 6);
    m_toolGroup->addButton(m_textToolBtn, 7);

    // 底部一行：左侧绘图工具 + 右侧操作按钮
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(10, 8, 10, 8);
    bottomLayout->setSpacing(4); // 减小间距以适应小窗口
    
    // 创建操作按钮 - 使用SVG矢量图标
    m_undoBtn = new QPushButton(this);
    m_saveBtn = new QPushButton(this);
    m_copyBtn = new QPushButton(this);
    m_stickyBtn = new QPushButton(this);
    m_closeBtn = new QPushButton(this);
    
    // 设置SVG图标 (使用英文文件名避免编码问题)
    m_undoBtn->setIcon(QIcon(":/icons/resources/icons/undo.svg"));
    m_saveBtn->setIcon(QIcon(":/icons/resources/icons/save.svg"));
    m_copyBtn->setIcon(QIcon(":/icons/resources/icons/copy.svg"));
    m_stickyBtn->setIcon(QIcon(":/icons/resources/icons/pin.svg"));
    m_closeBtn->setIcon(QIcon(":/icons/resources/icons/close.svg"));
    
    // 设置图标大小
    m_undoBtn->setIconSize(iconSize);
    m_saveBtn->setIconSize(iconSize);
    m_copyBtn->setIconSize(iconSize);
    m_stickyBtn->setIconSize(iconSize);
    m_closeBtn->setIconSize(iconSize);
    
    // 设置tooltip提示
    m_undoBtn->setToolTip("撤销 (Ctrl+Z)");
    m_saveBtn->setToolTip("保存");
    m_copyBtn->setToolTip("复制到剪贴板");
    m_stickyBtn->setToolTip("贴图到桌面");
    m_closeBtn->setToolTip("关闭 (ESC)");
    
    // 设置按钮样式 - 统一使用亮色背景，更清晰易识别
    QString actionBtnStyle = "QPushButton { "
                         "background-color: rgba(240, 240, 245, 230); "
                         "color: #333; "
                         "border: 1px solid rgba(0, 0, 0, 80); "
                         "padding: 6px 12px; "
                         "border-radius: 6px; "
                         "font-weight: 500; "
                         "font-size: 12px; "
                         "} "
                         "QPushButton:hover { "
                         "background-color: rgba(230, 230, 240, 250); "
                         "border: 1px solid rgba(0, 0, 0, 120); "
                         "} "
                         "QPushButton:pressed { "
                         "background-color: rgba(220, 220, 230, 240); "
                         "}";
    QString toolBtnStyle = "QPushButton { "
                         "background-color: rgba(255, 255, 255, 230); "
                         "color: #333; "
                         "border: 1px solid rgba(0, 0, 0, 80); "
                         "padding: 6px 10px; "
                         "border-radius: 6px; "
                         "font-size: 12px; "
                         "} "
                         "QPushButton:checked { "
                         "background-color: rgba(64, 158, 255, 240); "
                         "color: white; "
                         "border: 1px solid rgba(0,0,0,100); "
                         "} "
                         "QPushButton:hover { "
                         "background-color: rgba(255, 255, 255, 250); "
                         "border: 1px solid rgba(0, 0, 0, 120); "
                         "}";
    
    m_undoBtn->setStyleSheet(actionBtnStyle);
    m_saveBtn->setStyleSheet(actionBtnStyle);
    m_copyBtn->setStyleSheet(actionBtnStyle);
    m_stickyBtn->setStyleSheet(actionBtnStyle);
    m_closeBtn->setStyleSheet(actionBtnStyle);
    m_penToolBtn->setStyleSheet(toolBtnStyle);
    m_lineToolBtn->setStyleSheet(toolBtnStyle);
    m_arrowToolBtn->setStyleSheet(toolBtnStyle);
    m_circleToolBtn->setStyleSheet(toolBtnStyle);
    m_rectToolBtn->setStyleSheet(toolBtnStyle);
    m_mosaicToolBtn->setStyleSheet(toolBtnStyle);
    m_textToolBtn->setStyleSheet(toolBtnStyle);
    
    // 关键修复：设置按钮最小尺寸（SVG图标模式，矢量图标自适应DPI）
    QSize btnMinSize(28, 28);   // 统一按钮最小尺寸（SVG矢量图标）
    
    // 设置按钮最小尺寸
    m_penToolBtn->setMinimumSize(btnMinSize);
    m_lineToolBtn->setMinimumSize(btnMinSize);
    m_arrowToolBtn->setMinimumSize(btnMinSize);
    m_circleToolBtn->setMinimumSize(btnMinSize);
    m_rectToolBtn->setMinimumSize(btnMinSize);
    m_mosaicToolBtn->setMinimumSize(btnMinSize);
    m_textToolBtn->setMinimumSize(btnMinSize);
    
    m_undoBtn->setMinimumSize(btnMinSize);
    m_saveBtn->setMinimumSize(btnMinSize);
    m_copyBtn->setMinimumSize(btnMinSize);
    m_stickyBtn->setMinimumSize(btnMinSize);
    m_closeBtn->setMinimumSize(btnMinSize);
    
    // 添加到一行：左侧工具，右侧操作按钮
    bottomLayout->addWidget(m_penToolBtn);
    bottomLayout->addWidget(m_lineToolBtn);
    bottomLayout->addWidget(m_arrowToolBtn);
    bottomLayout->addWidget(m_circleToolBtn);
    bottomLayout->addWidget(m_rectToolBtn);
    bottomLayout->addWidget(m_mosaicToolBtn);
    bottomLayout->addWidget(m_textToolBtn);

    // 不再在底栏展示颜色/粗细，改为弹窗
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_undoBtn);
    bottomLayout->addWidget(m_saveBtn);
    bottomLayout->addWidget(m_copyBtn);
    bottomLayout->addWidget(m_stickyBtn);
    bottomLayout->addWidget(m_closeBtn);
    
    // 使用正确的布局：上方是图片区域（固定空间），下方是按钮
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 关键修复：禁用布局的尺寸约束，允许窗口自由缩放
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    
    // 创建一个固定高度的透明Widget作为图片显示区域
    QWidget *imageArea = new QWidget(this);
    qDebug() << "[setupUI] Setting imageArea fixed size to:" << m_originalScreenshotSize;
    imageArea->setFixedSize(m_originalScreenshotSize.width(), m_originalScreenshotSize.height());
    imageArea->setStyleSheet("background: transparent;");
    imageArea->setAttribute(Qt::WA_TransparentForMouseEvents, false); // 确保可以接收鼠标事件
    qDebug() << "[setupUI] imageArea actual size:" << imageArea->size();
    
    // 将图片区域添加到布局（居中对齐，避免小图片偏左的问题）
    mainLayout->addWidget(imageArea, 0, Qt::AlignTop | Qt::AlignHCenter);
    
    // 添加按钮布局到底部
    mainLayout->addLayout(bottomLayout);
}

void ScreenshotEditWindow::setupConnections() {
    connect(m_saveBtn, &QPushButton::clicked, this, &ScreenshotEditWindow::onSaveClicked);
    connect(m_copyBtn, &QPushButton::clicked, this, &ScreenshotEditWindow::onCopyClicked);
    connect(m_stickyBtn, &QPushButton::clicked, this, &ScreenshotEditWindow::onStickyNoteClicked);
    connect(m_closeBtn, &QPushButton::clicked, this, &ScreenshotEditWindow::onCloseClicked);
    connect(m_undoBtn, &QPushButton::clicked, this, [this]() {
        // 撤销功能实现：从撤销栈恢复上一个状态
        if (!m_undoStack.isEmpty()) {
            m_screenshot = m_undoStack.takeLast();
            qDebug() << "[Undo] Restored previous state, stack size:" << m_undoStack.size();
            update();
        } else {
            qDebug() << "[Undo] No more undo history";
        }
    });
    
    connect(m_toolGroup, &QButtonGroup::idClicked, this, &ScreenshotEditWindow::onToolButtonClicked);
}

void ScreenshotEditWindow::showStylePopover(QPushButton *anchorBtn) {
    if (!m_stylePopover) {
        m_stylePopover = new StylePopover(this);
        connect(m_stylePopover, &StylePopover::colorSelected, this, &ScreenshotEditWindow::onColorSelected);
        connect(m_stylePopover, &StylePopover::thicknessSelected, this, &ScreenshotEditWindow::onThicknessSelected);
    }
    // 对齐到触发按钮的正下方（带 8px 间距），并做屏幕边界修正
    QPoint anchorCenterGlobal = anchorBtn->mapToGlobal(QPoint(anchorBtn->width()/2, anchorBtn->height() + 8));
    m_stylePopover->setCurrent(m_currentColor, m_currentThickness);
    QSize sz = m_stylePopover->sizeHint();
    
    // 修复：使用当前编辑窗口所在的屏幕，而不是主屏幕
    QScreen *currentScreen = QGuiApplication::screenAt(anchorCenterGlobal);
    if (!currentScreen) {
        currentScreen = QGuiApplication::screenAt(this->pos());
    }
    if (!currentScreen) {
        currentScreen = QGuiApplication::primaryScreen();
    }
    QRect screenRect = currentScreen ? currentScreen->geometry() : QRect(0,0,1920,1080);
    
    qDebug() << "[ColorPicker] anchorCenterGlobal:" << anchorCenterGlobal
             << " currentScreen:" << (currentScreen ? currentScreen->name() : "null")
             << " screenRect:" << screenRect;
    
    int targetX = anchorCenterGlobal.x() - sz.width()/2;
    int targetY = anchorCenterGlobal.y();
    // 左右边界
    if (targetX < screenRect.left()) targetX = screenRect.left() + 6;
    if (targetX + sz.width() > screenRect.right()) targetX = screenRect.right() - sz.width() - 6;
    // 上下边界
    if (targetY + sz.height() > screenRect.bottom()) {
        // 如果下方空间不够，显示在按钮上方
        targetY = anchorCenterGlobal.y() - sz.height() - 8;
    }
    // 箭头相对于弹窗的X
    int arrowX = anchorCenterGlobal.x() - targetX;
    arrowX = qBound(14, arrowX, sz.width()-14);
    m_stylePopover->setArrowCenterX(arrowX);
    m_stylePopover->move(targetX, targetY);
    m_stylePopover->show();
    m_stylePopover->raise();
    
    qDebug() << "[ColorPicker] Final position:" << QPoint(targetX, targetY);
}

void ScreenshotEditWindow::hideStylePopover() {
    if (m_stylePopover) {
        m_stylePopover->hide();
    }
}

void ScreenshotEditWindow::startDrawingMode(int toolType) {
    m_currentTool = toolType;
    if (toolType == 7) { // 文本工具
        startTextInputAt(m_drawStartPos);
    }
}

void ScreenshotEditWindow::endDrawingMode() {
    m_isDrawing = false;
    m_currentTool = 0;
}

void ScreenshotEditWindow::drawTool(QPainter &painter, const QPoint &startPos, const QPoint &endPos, int toolType) {
    QPen pen(m_currentColor);
    pen.setWidth(m_currentThickness);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    
    switch (toolType) {
    case 1: // 画笔工具
        if (m_drawHistory.size() > 1) {
            for (int i = 1; i < m_drawHistory.size(); ++i) {
                painter.drawLine(m_drawHistory[i-1].first, m_drawHistory[i].first);
            }
        }
        break;
    case 2: // 直线工具
        painter.drawLine(startPos, endPos);
        break;
    case 3: // 箭头工具
        {
            painter.drawLine(startPos, endPos);
            // 绘制箭头头部
            QLineF line(startPos, endPos);
            double angle = std::atan2(-line.dy(), line.dx());
            double len = 18.0;
            QPointF p1 = QPointF(endPos) + QPointF(-len * std::cos(angle + 3.14159265358979323846/6.0), len * std::sin(angle + 3.14159265358979323846/6.0));
            QPointF p2 = QPointF(endPos) + QPointF(-len * std::cos(angle - 3.14159265358979323846/6.0), len * std::sin(angle - 3.14159265358979323846/6.0));
            painter.drawLine(endPos, p1.toPoint());
            painter.drawLine(endPos, p2.toPoint());
        }
        break;
    case 4: // 圆形工具
        {
            QRect rect = QRect(startPos, endPos).normalized();
            painter.drawEllipse(rect);
        }
        break;
    case 5: // 矩形工具
        {
            QRect rect = QRect(startPos, endPos).normalized();
            painter.drawRect(rect);
        }
        break;
    }
}

void ScreenshotEditWindow::startTextInputAt(const QPoint &pos) {
    if (m_textEdit) {
        m_textEdit->deleteLater();
        m_textEdit = nullptr;
    }
    m_drawStartPos = pos;
    m_textEdit = new QLineEdit(this);
    m_textEdit->setPlaceholderText("");
    m_textEdit->setFrame(false);
    m_textEdit->setStyleSheet("QLineEdit{background: rgba(255,255,255,210); border:1px solid rgba(0,0,0,80); padding:3px; border-radius:4px; font-size:14px;}");
    m_textEdit->move(pos);
    m_textEdit->setFixedWidth(220);
    m_textEdit->show();
    m_textEdit->setFocus();
    m_isTextEditing = true;
    connect(m_textEdit, &QLineEdit::returnPressed, this, &ScreenshotEditWindow::onTextEditFinished);
    connect(m_textEdit, &QLineEdit::editingFinished, this, &ScreenshotEditWindow::onTextEditFinished);
}

void ScreenshotEditWindow::applyMosaicAt(const QPoint &center) {
    if (m_screenshot.isNull()) return;
    // 马赛克工具即时生效，但撤销栈已在 mousePressEvent 中保存
    // 基于粗细计算块大小
    int block = qMax(4, m_currentThickness * 6);
    int half = block / 2;
    QRect patchRect(center.x() - half, center.y() - half, block, block);
    QSize displaySize = m_screenshot.size() / m_screenshot.devicePixelRatio();
    QRect imageRect(0, 0, displaySize.width(), displaySize.height());
    patchRect = patchRect.intersected(imageRect);
    if (patchRect.isEmpty()) return;
    QImage patch = m_screenshot.copy(patchRect).toImage();
    QSize smallSize(qMax(1, patch.width() / 6), qMax(1, patch.height() / 6));
    QImage reduced = patch.scaled(smallSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QImage pixelated = reduced.scaled(patch.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QPainter p(&m_screenshot);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.drawImage(patchRect.topLeft(), pixelated);
    update();
}

void ScreenshotEditWindow::updateWindowSizeForDPI() {
    // 防止重复更新
    if (m_isUpdatingDPI) {
        qDebug() << "[DPI] Already updating, skip...";
        return;
    }
    
    m_isUpdatingDPI = true;
    
    // 获取当前屏幕的DPI缩放比例
    QScreen *currentScreen = QGuiApplication::screenAt(pos());
    if (!currentScreen) {
        currentScreen = QGuiApplication::primaryScreen();
    }
    
    qreal dpiRatio = currentScreen->devicePixelRatio();
    qDebug() << "[DPI] Current screen:" << currentScreen->name() 
             << "DPI ratio:" << dpiRatio;
    
    // 图片显示尺寸始终是原始尺寸
    QSize imageDisplaySize = m_originalScreenshotSize;
    
    // 获取布局
    QLayout *mainLayout = layout();
    
    // 更新布局中的图片区域大小
    if (mainLayout && mainLayout->count() > 0) {
        QLayoutItem *item = mainLayout->itemAt(0);
        if (item && item->widget()) {
            QWidget *imageArea = item->widget();
            imageArea->setFixedSize(imageDisplaySize);
            qDebug() << "[DPI] Set imageArea size to:" << imageDisplaySize;
        }
    }
    
    // 强制刷新布局
    if (mainLayout) {
        mainLayout->invalidate();
        mainLayout->activate();
    }
    
    // 简单计算窗口大小：图片 + 按钮区域（使用固定值更可靠）
    QSize targetWindowSize(imageDisplaySize.width(), imageDisplaySize.height() + 50);
    
    qDebug() << "[DPI] Resizing window from" << this->size() << "to" << targetWindowSize;
    
    // 清除最大尺寸限制
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    
    // 清除布局的尺寸限制
    if (mainLayout) {
        mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    }
    
    // 设置合理的最小窗口尺寸：使用图标按钮后只需420px
    // 图标按钮比文字按钮更紧凑
    int minWindowWidth = qMax(420, imageDisplaySize.width());
    int minWindowHeight = imageDisplaySize.height() + 50;
    setMinimumSize(minWindowWidth, minWindowHeight);
    
    resize(targetWindowSize);
    
    // 确保所有按钮可见并提升层级
    ensureButtonsVisible();
    
    // 强制重绘
    update();
    
    qDebug() << "[DPI] Update complete. Final window size:" << this->size();
    
    // 重置更新标志
    m_isUpdatingDPI = false;
}

void ScreenshotEditWindow::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
    
    // 检测是否移动到不同屏幕
    QScreen *oldScreen = QGuiApplication::screenAt(event->oldPos());
    QScreen *newScreen = QGuiApplication::screenAt(event->pos());
    
    if (oldScreen && newScreen && oldScreen != newScreen) {
        qDebug() << "[DPI] Window moved from screen" << oldScreen->name() 
                 << "to screen" << newScreen->name();
        qDebug() << "[DPI] Old DPI:" << oldScreen->devicePixelRatio() 
                 << "New DPI:" << newScreen->devicePixelRatio();
        
        // 立即确保按钮可见（防止快速移动时按钮消失）
        ensureButtonsVisible();
        
        // 使用QTimer延迟更新，确保窗口移动完成后再更新布局
        // 这样可以避免在移动过程中重复触发更新
        QTimer::singleShot(100, this, [this]() {
            if (!m_isUpdatingDPI) {
                updateWindowSizeForDPI();
            }
        });
    }
}

void ScreenshotEditWindow::repositionButtons() {
    // 使用布局管理器，不需要手动重新定位
    // 布局管理器会自动处理
    // 这个方法保留用于将来的扩展需求
}

void ScreenshotEditWindow::ensureButtonsVisible() {
    // 快速确保所有按钮可见并提升到顶层
    // 这个方法在快速多屏移动时被调用，避免按钮消失问题
    QList<QPushButton*> allButtons = {
        m_penToolBtn, m_lineToolBtn, m_arrowToolBtn, m_circleToolBtn,
        m_rectToolBtn, m_mosaicToolBtn, m_textToolBtn,
        m_undoBtn, m_saveBtn, m_copyBtn, m_stickyBtn, m_closeBtn
    };
    
    for (QPushButton* btn : allButtons) {
        if (btn) {
            btn->setVisible(true);
            btn->raise();
            btn->update(); // 强制按钮重绘
        }
    }
    
    // 强制刷新布局
    QLayout *mainLayout = layout();
    if (mainLayout) {
        mainLayout->invalidate();
        mainLayout->activate();
    }
    
    // 强制窗口重绘
    update();
    repaint();
    
    qDebug() << "[ButtonVisibility] Ensured all buttons are visible";
}

void ScreenshotEditWindow::showContextMenu(const QPoint &pos) {
    QMenu contextMenu(this);
    
    // 如果有工具被选中，添加"取消工具"选项
    if (m_currentTool != 0) {
        QAction *cancelToolAction = contextMenu.addAction("取消工具 (ESC)");
        connect(cancelToolAction, &QAction::triggered, this, [this]() {
            qDebug() << "[ContextMenu] Cancel tool selected";
            if (m_toolGroup) {
                QAbstractButton *checkedBtn = m_toolGroup->checkedButton();
                if (checkedBtn) {
                    checkedBtn->setChecked(false);
                }
            }
            m_currentTool = 0;
            m_isDrawing = false;
            m_isTextEditing = false;
            // 隐藏样式选择弹窗
            hideStylePopover();
            update();
        });
        contextMenu.addSeparator();
    }
    
    // 添加常用操作
    QAction *saveAction = contextMenu.addAction("保存 (Ctrl+S)");
    connect(saveAction, &QAction::triggered, this, [this]() {
        emit saveRequested(m_screenshot);
    });
    
    QAction *copyAction = contextMenu.addAction("复制 (Ctrl+C)");
    connect(copyAction, &QAction::triggered, this, [this]() {
        emit copyRequested(m_screenshot);
    });
    
    QAction *stickyAction = contextMenu.addAction("贴图");
    connect(stickyAction, &QAction::triggered, this, [this]() {
        emit createStickyNoteRequested(m_screenshot);
    });
    
    contextMenu.addSeparator();
    
    QAction *closeAction = contextMenu.addAction("关闭 (Ctrl+W)");
    connect(closeAction, &QAction::triggered, this, [this]() {
        if (!m_isClosing) {
            m_isClosing = true;
            emit closeRequested();
        }
    });
    
    // 在鼠标位置显示菜单
    contextMenu.exec(mapToGlobal(pos));
}

void ScreenshotEditWindow::ensureWindowInScreen() {
    QRect windowGeometry = geometry();
    
    // 找到与窗口重叠面积最大的屏幕（更可靠的方法）
    QScreen *currentScreen = nullptr;
    int maxOverlapArea = 0;
    
    QList<QScreen*> screens = QGuiApplication::screens();
    for (QScreen *screen : screens) {
        QRect screenGeom = screen->availableGeometry();
        QRect intersection = windowGeometry.intersected(screenGeom);
        int overlapArea = intersection.width() * intersection.height();
        
        if (overlapArea > maxOverlapArea) {
            maxOverlapArea = overlapArea;
            currentScreen = screen;
        }
    }
    
    // 如果没有找到重叠的屏幕，使用窗口中心点所在的屏幕
    if (!currentScreen || maxOverlapArea == 0) {
        QPoint windowCenter = windowGeometry.center();
        currentScreen = QGuiApplication::screenAt(windowCenter);
    }
    
    // 如果还是没有，使用左上角位置
    if (!currentScreen) {
        currentScreen = QGuiApplication::screenAt(pos());
    }
    
    // 最后的退路：使用主屏幕
    if (!currentScreen) {
        currentScreen = QGuiApplication::primaryScreen();
    }
    
    QRect screenGeometry = currentScreen->availableGeometry();
    
    qDebug() << "[WindowBounds] Position:" << pos() 
             << "Size:" << size() 
             << "Screen:" << currentScreen->name()
             << "Screen geometry:" << screenGeometry
             << "Overlap area:" << maxOverlapArea;
    
    // 调整位置确保窗口完全在屏幕内
    int newX = windowGeometry.x();
    int newY = windowGeometry.y();
    
    // 检查右边界
    if (windowGeometry.right() > screenGeometry.right()) {
        newX = screenGeometry.right() - windowGeometry.width();
        qDebug() << "[WindowBounds] Adjusting X from" << windowGeometry.x() << "to" << newX << "(right edge)";
    }
    
    // 检查左边界
    if (newX < screenGeometry.left()) {
        newX = screenGeometry.left();
        qDebug() << "[WindowBounds] Adjusting X to left edge:" << newX;
    }
    
    // 检查底边界
    if (windowGeometry.bottom() > screenGeometry.bottom()) {
        newY = screenGeometry.bottom() - windowGeometry.height();
        qDebug() << "[WindowBounds] Adjusting Y from" << windowGeometry.y() << "to" << newY << "(bottom edge)";
    }
    
    // 检查顶边界
    if (newY < screenGeometry.top()) {
        newY = screenGeometry.top();
        qDebug() << "[WindowBounds] Adjusting Y to top edge:" << newY;
    }
    
    // 如果位置需要调整，移动窗口
    if (newX != windowGeometry.x() || newY != windowGeometry.y()) {
        qDebug() << "[WindowBounds] Moving window from" << pos() << "to" << QPoint(newX, newY);
        move(newX, newY);
    } else {
        qDebug() << "[WindowBounds] Window is within screen bounds, no adjustment needed";
    }
}
