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
 * @file RegionSelector.cpp
 * @brief 区域选择器类实现
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include "RegionSelector.h"
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QFont>
#include <QDebug>
#include <QTimer>
#include <QCursor>

RegionSelector::RegionSelector(QWidget *parent)
    : QWidget(parent)
    , m_state(StateIdle)
    , m_isSelecting(false)
    , m_startPoint()
    , m_endPoint()
    , m_selectionRect()
    , m_startGlobal()
    , m_endGlobal()
    , m_globalSelection()
    , m_resizeHandle(0)
    , m_resizeStartPos()
    , m_resizeStartRect()
    , m_currentScreen(nullptr)
{
    // 设置窗口属性：无边框、工具窗口（移除置顶，避免覆盖弹窗）
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);  // 透明背景
    setAttribute(Qt::WA_DeleteOnClose, false);   // 不要在关闭时删除
    setAttribute(Qt::WA_NoSystemBackground);     // 无系统背景
    setAttribute(Qt::WA_TransparentForMouseEvents, false);  // 确保接收鼠标事件
    setMouseTracking(true);                      // 启用鼠标跟踪
    setFocusPolicy(Qt::StrongFocus);             // 强焦点策略，支持键盘事件
}

void RegionSelector::startSelection() {
    m_isSelecting = false;
    m_selectionRect = QRect();
    m_startPoint = QPoint();
    m_endPoint = QPoint();
    m_globalSelection = QRect();

    // 获取当前鼠标所在的屏幕
    QPoint cursorPos = QCursor::pos();
    m_currentScreen = QGuiApplication::screenAt(cursorPos);
    if (!m_currentScreen) {
        m_currentScreen = QGuiApplication::primaryScreen();
    }
    
    QRect screenRect = getCurrentScreenRect();
    if (!screenRect.isNull()) {
        // 先截取当前屏幕画面（冻结画面）
        qDebug() << "[SelectorStart] Freezing screen:" << m_currentScreen->name() 
                 << "at cursor pos:" << cursorPos
                 << "screen geometry:" << screenRect;
        captureScreenBackground();
        
        // 设置窗口几何，覆盖当前屏幕
        setGeometry(screenRect);
        qDebug() << "[SelectorStart] Screen rect=" << screenRect << " setGeometry to:" << screenRect;

        show();
        raise();
        activateWindow();
        
        // 再次确保窗口几何正确
        QTimer::singleShot(50, [this, screenRect]() {
            if (isVisible()) {
                // 强制重新设置几何，防止系统调整
                setGeometry(screenRect);
                qDebug() << "[SelectorStart] After show, window geometry:" << geometry();
                setFocus();
                setCursor(Qt::CrossCursor);
                grabKeyboard();
                update();
            }
        });
    }
}

QRect RegionSelector::getSelectionRect() const {
    return m_globalSelection;
}

QRect RegionSelector::getCurrentScreenRect() const {
    // 获取当前屏幕的几何信息（单屏模式）
    if (!m_currentScreen) {
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            return screen->geometry();
        }
        return QRect();
    }
    
    QRect screenRect = m_currentScreen->geometry();
    qDebug() << "[CurrentScreen] Screen:" << m_currentScreen->name() 
             << "geometry:" << screenRect
             << "DPI:" << m_currentScreen->devicePixelRatio();
    
    return screenRect;
}

QRect RegionSelector::globalToLocalRect(const QRect &globalRect) const {
    // 获取窗口在全局坐标系中的位置
    QRect windowGeometry = this->geometry();
    
    // 方法1：直接坐标平移（适用于虚拟桌面覆盖整个多屏环境的情况）
    QRect localRect = globalRect.translated(-windowGeometry.topLeft());
    
    // 调试信息：记录坐标转换过程
    qDebug() << "[CoordConvert] globalRect:" << globalRect
             << " windowGeometry:" << windowGeometry
             << " localRect:" << localRect;
    
    // 验证转换结果的合理性
    if (localRect.x() < -1000 || localRect.y() < -1000 || 
        localRect.x() > windowGeometry.width() + 1000 || 
        localRect.y() > windowGeometry.height() + 1000) {
        qDebug() << "[CoordConvert] WARNING: Unusual coordinate conversion detected!";
        qDebug() << "[CoordConvert] This might indicate a DPI scaling issue.";
    }
    
    return localRect;
}

/**
 * @brief 修复混合DPI环境的坐标转换
 */
QRect RegionSelector::globalToLocalRectFixed(const QRect &globalRect) const {
    // 关键修复：虚拟桌面坐标系是统一的逻辑坐标
    // 区域选择器窗口覆盖整个虚拟桌面，所以可以直接平移
    QRect windowGeometry = this->geometry();
    
    // 简单直接的坐标平移
    QRect localRect(globalRect.x() - windowGeometry.x(),
                   globalRect.y() - windowGeometry.y(),
                   globalRect.width(),
                   globalRect.height());
    
    // 坐标转换完成
    
    return localRect;
}

void RegionSelector::drawSelectionCorners(QPainter &painter, const QRect &rect) {
    int cornerSize = 10;
    QPen cornerPen(QColor(160, 160, 160), 3);
    painter.setPen(cornerPen);
    
    // 左上角
    painter.drawLine(rect.topLeft(), rect.topLeft() + QPoint(cornerSize, 0));
    painter.drawLine(rect.topLeft(), rect.topLeft() + QPoint(0, cornerSize));
    
    // 右上角
    painter.drawLine(rect.topRight(), rect.topRight() + QPoint(-cornerSize, 0));
    painter.drawLine(rect.topRight(), rect.topRight() + QPoint(0, cornerSize));
    
    // 左下角
    painter.drawLine(rect.bottomLeft(), rect.bottomLeft() + QPoint(cornerSize, 0));
    painter.drawLine(rect.bottomLeft(), rect.bottomLeft() + QPoint(0, -cornerSize));
    
    // 右下角
    painter.drawLine(rect.bottomRight(), rect.bottomRight() + QPoint(-cornerSize, 0));
    painter.drawLine(rect.bottomRight(), rect.bottomRight() + QPoint(0, -cornerSize));
}

void RegionSelector::drawResizeHandles(QPainter &painter, const QRect &rect) {
    const int handleSize = 8; // 手柄大小
    const QColor handleColor(64, 158, 255); // 蓝色手柄
    const QColor handleBorder(255, 255, 255); // 白色边框
    
    painter.setPen(QPen(handleBorder, 2));
    painter.setBrush(handleColor);
    
    // 8个调整手柄位置
    QPoint handles[8] = {
        rect.topLeft(),                                    // 1-左上
        QPoint(rect.center().x(), rect.top()),            // 2-上
        rect.topRight(),                                   // 3-右上
        QPoint(rect.right(), rect.center().y()),          // 4-右
        rect.bottomRight(),                                // 5-右下
        QPoint(rect.center().x(), rect.bottom()),         // 6-下
        rect.bottomLeft(),                                 // 7-左下
        QPoint(rect.left(), rect.center().y())            // 8-左
    };
    
    // 绘制每个手柄
    for (const QPoint &handle : handles) {
        painter.drawRect(handle.x() - handleSize/2, handle.y() - handleSize/2, 
                        handleSize, handleSize);
    }
}

void RegionSelector::drawSizeInfo(QPainter &painter, const QRect &rect) {
    QString sizeText = QString("%1 x %2")
        .arg(m_globalSelection.width())
        .arg(m_globalSelection.height());
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);
    
    // 使用带背景的文字，确保在任何背景下都可见
    QRect textRect = QRect(rect.topLeft() + QPoint(5, -25), QSize(100, 20));
    painter.fillRect(textRect, QColor(0, 0, 0, 180));
    painter.setPen(Qt::white);
    painter.drawText(textRect, Qt::AlignCenter, sizeText);
}

void RegionSelector::drawCrosshair(QPainter &painter) {
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    
    // 绘制小十字架
    painter.setPen(QPen(QColor(255, 0, 0), 2, Qt::SolidLine));
    
    // 十字架大小
    int crossSize = 20;
    int halfSize = crossSize / 2;
    
    // 水平线
    painter.drawLine(mousePos.x() - halfSize, mousePos.y(), 
                   mousePos.x() + halfSize, mousePos.y());
    // 垂直线
    painter.drawLine(mousePos.x(), mousePos.y() - halfSize, 
                   mousePos.x(), mousePos.y() + halfSize);
    
    // 绘制中心点
    painter.setPen(QPen(QColor(255, 0, 0), 3, Qt::SolidLine));
    painter.drawPoint(mousePos);
}

void RegionSelector::drawTipInfo(QPainter &painter) {
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);
    
    // 绘制提示背景
    QRect tipRect(10, 10, 380, 35);
    painter.fillRect(tipRect, QColor(0, 0, 0, 200));
    
    // 绘制边框
    painter.setPen(QPen(QColor(255, 255, 255, 150), 1));
    painter.drawRect(tipRect.adjusted(0, 0, -1, -1));
    
    // 绘制提示文字
    painter.setPen(Qt::white);
    QString tipText;
    if (m_state == StateSelecting) {
        tipText = "拖拽截图，取消按ESC";
    } else if (m_state == StateSelected) {
        tipText = "拖拽边框调整，右键或Enter确认，ESC取消";
    } else {
        tipText = "点击左键开始截图，取消按ESC";
    }
    painter.drawText(tipRect, Qt::AlignCenter, tipText);
}

void RegionSelector::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true); // 启用平滑变换以获得更好的显示效果
    
    // 绘制冻结的背景画面
    if (!m_frozenBackground.isNull()) {
        // 如果冻结图像的devicePixelRatio不为1，需要调整绘制区域
        qreal dpr = m_frozenBackground.devicePixelRatio();
        
        qDebug() << "[Paint] Frozen background DPR:" << dpr 
                 << "size:" << m_frozenBackground.size()
                 << "window rect:" << rect();
        
        // 根据DPR调整绘制（从窗口本地坐标0,0开始绘制）
        if (dpr > 1.0) {
            // 高DPI图像，需要缩放显示
            QSize logicalSize = m_frozenBackground.size() / dpr;
            painter.drawPixmap(0, 0, logicalSize.width(), logicalSize.height(), m_frozenBackground);
        } else {
            // 普通DPI，直接绘制
            painter.drawPixmap(0, 0, m_frozenBackground);
        }
    } else {
        // 如果没有冻结画面，绘制半透明背景
        painter.fillRect(rect(), QColor(0, 0, 0, 10));
    }
    
    // 绘制选择框（正在选择、已选择或正在调整时）
    if (!m_globalSelection.isEmpty() && 
        (m_state == StateSelecting || m_state == StateSelected || m_state == StateResizing)) {
        // 转换为本地坐标
        QPoint localTopLeft = mapFromGlobal(m_globalSelection.topLeft());
        QPoint localBottomRight = mapFromGlobal(m_globalSelection.bottomRight());
        QRect localSelection = QRect(localTopLeft, localBottomRight).normalized();
        
        if (localSelection.isValid() && rect().intersects(localSelection)) {
            // 绘制遮罩（半透明黑色，选择区域除外）
            QPainterPath path;
            path.addRect(rect());
            path.addRect(localSelection);
            painter.fillPath(path, QColor(0, 0, 0, 100));
            
            // 绘制选择框
            if (m_state == StateSelected || m_state == StateResizing) {
                // 已选择状态：实线边框
                painter.setPen(QPen(QColor(64, 158, 255), 2, Qt::SolidLine));
            } else {
                // 正在选择：虚线边框
                painter.setPen(QPen(QColor(160, 160, 160), 2, Qt::DashLine));
            }
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(localSelection);
            
            if (m_state == StateSelected || m_state == StateResizing) {
                // 绘制调整手柄
                drawResizeHandles(painter, localSelection);
            } else {
                // 绘制角标
                drawSelectionCorners(painter, localSelection);
            }
            
            // 绘制尺寸信息
            drawSizeInfo(painter, localSelection);
        }
    } else if (m_state == StateIdle) {
        // 显示小十字架跟随鼠标（仅在空闲状态时）
        drawCrosshair(painter);
    }
    
    // 始终在左上角显示提示信息
    drawTipInfo(painter);
}

void RegionSelector::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 保护：确保窗口已完全显示后再接受按下事件
        if (!isVisible() || !isActiveWindow()) {
            qDebug() << "[SelectBegin] window not ready, ignoring press";
            return;
        }
        
        QPoint globalPos = event->globalPosition().toPoint();
        
        // 检查是否在已选择的区域上点击（调整模式）
        if (m_state == StateSelected && !m_globalSelection.isEmpty()) {
            int hit = hitTest(globalPos);
            if (hit > 0) {
                // 开始调整大小或移动
                m_state = StateResizing;
                m_resizeHandle = hit;
                m_resizeStartPos = globalPos;
                m_resizeStartRect = m_globalSelection;
                updateCursor(hit);
                qDebug() << "[ResizeBegin] hit:" << hit << " pos:" << globalPos;
                return;
            }
        }
        
        // 否则开始新的选择
        m_startGlobal = globalPos;
        m_endGlobal = m_startGlobal;
        m_globalSelection = QRect();
        m_isSelecting = true;
        m_state = StateSelecting;
        
        // 调试：记录选择起点和屏幕信息
        QScreen *scr = QGuiApplication::screenAt(m_startGlobal);
        QRect scrGeom = scr ? scr->geometry() : QRect();
        qDebug() << "[SelectBegin] startGlobal:" << m_startGlobal
                 << " localPos:" << event->pos()
                 << " windowGeometry:" << this->geometry()
                 << " screen:" << (scr ? scr->name() : QString("null"))
                 << " screenGeom:" << scrGeom;
        
        // 确保鼠标跟踪启用
        if (!hasMouseTracking()) {
            setMouseTracking(true);
        }
        
        update(); // 强制重绘
    }
}

void RegionSelector::mouseMoveEvent(QMouseEvent *event) {
    QPoint globalPos = event->globalPosition().toPoint();
    
    if (m_state == StateSelecting) {
        // 正在选择区域
        m_endGlobal = globalPos;
        m_globalSelection = QRect(m_startGlobal, m_endGlobal).normalized();
        update();
    } else if (m_state == StateResizing) {
        // 正在调整大小或移动
        QPoint delta = globalPos - m_resizeStartPos;
        QRect newRect = m_resizeStartRect;
        
        // 根据不同的handle调整矩形
        switch (m_resizeHandle) {
            case 1: // 左上
                newRect.setTopLeft(m_resizeStartRect.topLeft() + delta);
                break;
            case 2: // 上
                newRect.setTop(m_resizeStartRect.top() + delta.y());
                break;
            case 3: // 右上
                newRect.setTopRight(m_resizeStartRect.topRight() + delta);
                break;
            case 4: // 右
                newRect.setRight(m_resizeStartRect.right() + delta.x());
                break;
            case 5: // 右下
                newRect.setBottomRight(m_resizeStartRect.bottomRight() + delta);
                break;
            case 6: // 下
                newRect.setBottom(m_resizeStartRect.bottom() + delta.y());
                break;
            case 7: // 左下
                newRect.setBottomLeft(m_resizeStartRect.bottomLeft() + delta);
                break;
            case 8: // 左
                newRect.setLeft(m_resizeStartRect.left() + delta.x());
                break;
            case 9: // 移动整个区域
                newRect.translate(delta);
                break;
        }
        
        // 确保最小尺寸
        newRect = newRect.normalized();
        if (newRect.width() >= 10 && newRect.height() >= 10) {
            m_globalSelection = newRect;
        }
        
        update();
    } else if (m_state == StateSelected && !m_globalSelection.isEmpty()) {
        // 更新光标样式
        int hit = hitTest(globalPos);
        updateCursor(hit);
    }
}

void RegionSelector::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (m_state == StateSelecting) {
            // 选择完成
            m_isSelecting = false;
            m_globalSelection = QRect(m_startGlobal, m_endGlobal).normalized();
            
            if (!m_globalSelection.isEmpty() &&
                m_globalSelection.width() >= 10 &&
                m_globalSelection.height() >= 10) {
                // 进入已选择状态，允许调整
                m_state = StateSelected;
                qDebug() << "[SelectComplete] selection:" << m_globalSelection;
                update();
            } else {
                cleanupAndClose();
            }
        } else if (m_state == StateResizing) {
            // 调整完成，回到已选择状态
            m_state = StateSelected;
            m_resizeHandle = 0;
            qDebug() << "[ResizeComplete] selection:" << m_globalSelection;
            update();
        }
    } else if (event->button() == Qt::RightButton) {
        // 右键确认选择
        if (m_state == StateSelected && !m_globalSelection.isEmpty()) {
            qDebug() << "[Confirm] Right-click confirm selection:" << m_globalSelection;
            emit regionSelected(m_globalSelection);
            
            // 立即关闭，避免视觉跳动
            cleanupAndClose();
        }
    }
}

void RegionSelector::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        m_isSelecting = false;
        m_globalSelection = QRect();
        m_state = StateIdle;
        emit selectionCancelled();
        
        QTimer::singleShot(50, [this]() {
            cleanupAndClose();
        });
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space) {
        // Enter/Space确认选择
        if (m_state == StateSelected && !m_globalSelection.isEmpty()) {
            qDebug() << "[Confirm] Keyboard confirm selection:" << m_globalSelection;
            emit regionSelected(m_globalSelection);
            
            // 立即关闭，避免视觉跳动
            cleanupAndClose();
        }
    }
}

void RegionSelector::cleanupAndClose() {
    releaseKeyboard();
    releaseMouse();
    hide();
    close();
}

int RegionSelector::hitTest(const QPoint &pos) const {
    if (m_globalSelection.isEmpty()) {
        return 0;
    }
    
    const int gripSize = 8; // 抓取区域大小
    QRect rect = m_globalSelection;
    
    // 检测8个角和边
    // 1-左上, 2-上, 3-右上, 4-右, 5-右下, 6-下, 7-左下, 8-左
    
    bool nearLeft = qAbs(pos.x() - rect.left()) <= gripSize;
    bool nearRight = qAbs(pos.x() - rect.right()) <= gripSize;
    bool nearTop = qAbs(pos.y() - rect.top()) <= gripSize;
    bool nearBottom = qAbs(pos.y() - rect.bottom()) <= gripSize;
    bool inHorizontal = pos.x() >= rect.left() - gripSize && pos.x() <= rect.right() + gripSize;
    bool inVertical = pos.y() >= rect.top() - gripSize && pos.y() <= rect.bottom() + gripSize;
    
    // 角点优先
    if (nearLeft && nearTop && inHorizontal && inVertical) return 1; // 左上
    if (nearRight && nearTop && inHorizontal && inVertical) return 3; // 右上
    if (nearRight && nearBottom && inHorizontal && inVertical) return 5; // 右下
    if (nearLeft && nearBottom && inHorizontal && inVertical) return 7; // 左下
    
    // 边
    if (nearTop && inHorizontal) return 2; // 上
    if (nearRight && inVertical) return 4; // 右
    if (nearBottom && inHorizontal) return 6; // 下
    if (nearLeft && inVertical) return 8; // 左
    
    // 内部（可移动）
    if (rect.contains(pos)) return 9;
    
    return 0; // 外部
}

void RegionSelector::updateCursor(int hit) {
    switch (hit) {
        case 1:
        case 5:
            setCursor(Qt::SizeFDiagCursor); // 左上-右下
            break;
        case 3:
        case 7:
            setCursor(Qt::SizeBDiagCursor); // 右上-左下
            break;
        case 2:
        case 6:
            setCursor(Qt::SizeVerCursor); // 上下
            break;
        case 4:
        case 8:
            setCursor(Qt::SizeHorCursor); // 左右
            break;
        case 9:
            setCursor(Qt::SizeAllCursor); // 移动
            break;
        default:
            setCursor(Qt::CrossCursor); // 十字
            break;
    }
}

void RegionSelector::captureScreenBackground() {
    qDebug() << "[ScreenFreeze] Capturing current screen background";
    
    if (!m_currentScreen) {
        qWarning() << "[ScreenFreeze] No current screen set";
        return;
    }
    
    // 获取当前屏幕的几何信息
    QRect screenGeometry = m_currentScreen->geometry();
    qDebug() << "[ScreenFreeze] Screen:" << m_currentScreen->name() 
             << "geometry:" << screenGeometry
             << "DPI:" << m_currentScreen->devicePixelRatio();
    
    // 截取当前屏幕（使用屏幕本地坐标，从(0,0)开始）
    m_frozenBackground = m_currentScreen->grabWindow(0);
    
    qDebug() << "[ScreenFreeze] Screen frozen, size:" << m_frozenBackground.size()
             << "devicePixelRatio:" << m_frozenBackground.devicePixelRatio()
             << "logical size:" << (m_frozenBackground.size() / m_frozenBackground.devicePixelRatio());
}
