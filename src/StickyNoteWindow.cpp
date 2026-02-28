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
 * @file StickyNoteWindow.cpp
 * @brief 便签式贴图窗口类实现
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include "StickyNoteWindow.h"
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
#include <QFontMetrics>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QVariantAnimation>
#include <QEasingCurve>
#include <cmath>

StickyNoteWindow::StickyNoteWindow(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
    , m_pixmap(pixmap)
    , m_isDragging(false)
    , m_dragOffset()
    , m_isResizing(false)
    , m_resizeHandle(-1)
    , m_handleSize(12)
    , m_minSize(100, 100)
    , m_scaleFactor(1.0f)
    , m_originalSize(pixmap.size())
    , m_smoothScaling(true)
    , m_targetScaleFactor(1.0f)
    , m_scaleAnimation(nullptr)
    , m_contextMenu(nullptr)
    , m_animationInProgress(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // 设置初始大小为"显示尺寸"（按DPR换算），保证与编辑窗口一致
    {
        const qreal dpr = (m_pixmap.devicePixelRatio() > 0) ? m_pixmap.devicePixelRatio() : 1.0;
        m_originalSize = QSize(qRound(m_pixmap.width() / dpr), qRound(m_pixmap.height() / dpr));
        resize(m_originalSize);
    }
    
    // 创建平滑缩放动画
    setupScaleAnimation();
    
    // 创建右键菜单
    setupContextMenu();
    
    // 更新控制点
    updateHandles();
}

void StickyNoteWindow::setPixmap(const QPixmap &pixmap) {
    m_pixmap = pixmap;
    const qreal dpr = (m_pixmap.devicePixelRatio() > 0) ? m_pixmap.devicePixelRatio() : 1.0;
    m_originalSize = QSize(qRound(m_pixmap.width() / dpr), qRound(m_pixmap.height() / dpr));
    resize(m_originalSize);
    m_scaleFactor = 1.0f;
    update();
}

QPixmap StickyNoteWindow::getPixmap() const {
    return m_pixmap;
}

void StickyNoteWindow::setAlwaysOnTop(bool onTop) {
    if (onTop) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    } else {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    }
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    show();
}

void StickyNoteWindow::toggleAlwaysOnTop() {
    bool isOnTop = windowFlags() & Qt::WindowStaysOnTopHint;
    setAlwaysOnTop(!isOnTop);
}

void StickyNoteWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    
    if (!m_pixmap.isNull()) {
        int targetW = qMax(1, qRound(m_originalSize.width() * m_scaleFactor));
        int targetH = qMax(1, qRound(m_originalSize.height() * m_scaleFactor));
        int x = (width() - targetW) / 2;
        int y = (height() - targetH) / 2;
        QRect imgRect(x, y, targetW, targetH);
        
        // 关键优化：原始尺寸时直接绘制，避免缩放导致的模糊
        if (qAbs(m_scaleFactor - 1.0f) < 0.001f) {
            // 1:1 显示，直接绘制原始图片，保持完美清晰
            painter.drawPixmap(imgRect, m_pixmap);
        } else {
            // 需要缩放时才启用抗锯齿和平滑变换
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::SmoothPixmapTransform);
            QPixmap scaledPixmap = m_pixmap.scaled(QSize(targetW, targetH), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(imgRect, scaledPixmap);
        }

        // 仅显示用的灰色虚线边框（不影响复制/保存的像素）
        QPen dashPen(QColor(160, 160, 160, 220));
        dashPen.setStyle(Qt::DashLine);
        dashPen.setWidth(2);
        painter.setPen(dashPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(imgRect.adjusted(1, 1, -1, -1));
    }
    
    // 不绘制控制点，保持贴图完全干净
    // 控制点功能仍然可用，只是不显示视觉提示
    
    // 不绘制缩放比例信息，保持贴图完全干净
    // 缩放功能仍然可用，只是不显示视觉提示
}

void StickyNoteWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_startPoint = event->pos();
        
        // 检查是否点击了调整大小的控制点
        for (int i = 0; i < m_handles.size(); ++i) {
            if (isPointInHandle(event->pos(), m_handles[i])) {
                m_isResizing = true;
                m_resizeHandle = i;
                return;
            }
        }
        
        // 开始拖拽
        m_isDragging = true;
        m_dragOffset = event->pos();
    }
}

void StickyNoteWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPoint newPos = event->globalPosition().toPoint() - m_dragOffset;
        move(newPos);
    } else if (m_isResizing) {
        resizeWindow(event->pos());
    } else {
        updateCursor(event->pos());
    }
}

void StickyNoteWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        m_isResizing = false;
        m_resizeHandle = -1;
    }
}

void StickyNoteWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    // 双击可以调整大小
    if (size() == m_minSize) {
        resize(400, 300);
    } else {
        resize(m_minSize);
    }
    updateHandles();
}

void StickyNoteWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        emit closeRequested();
        break;
    case Qt::Key_S:
        if (event->modifiers() & Qt::ControlModifier) {
            emit saveRequested();
        }
        break;
    case Qt::Key_C:
        if (event->modifiers() & Qt::ControlModifier) {
            emit copyRequested();
        }
        break;
    case Qt::Key_0:
        if (event->modifiers() & Qt::ControlModifier) {
            onResetScale();
        }
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        if (event->modifiers() & Qt::ControlModifier) {
            startSmoothScale(m_scaleFactor * 1.30f);
        }
        break;
    case Qt::Key_Minus:
        if (event->modifiers() & Qt::ControlModifier) {
            startSmoothScale(m_scaleFactor / 1.30f);
        }
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void StickyNoteWindow::contextMenuEvent(QContextMenuEvent *event) {
    m_contextMenu->exec(event->globalPos());
}

void StickyNoteWindow::wheelEvent(QWheelEvent *event) {
    // 使用动画实现更平滑的缩放
    const int delta = event->angleDelta().y();
    const float steps = static_cast<float>(delta) / 120.0f;
    const float factorPerStep = 1.30f; // 10% 步进
    float target = m_scaleFactor * std::pow(factorPerStep, steps);
    startSmoothScale(target);
    event->accept();
}

void StickyNoteWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateHandles();
}

void StickyNoteWindow::onCloseClicked() {
    emit closeRequested();
}

void StickyNoteWindow::onScaleAnimationFinished() {
    m_animationInProgress = false;
    applyScaleFactor(m_targetScaleFactor, false);
    m_scaleFactor = m_targetScaleFactor;
}

void StickyNoteWindow::setupContextMenu() {
    m_contextMenu = new QMenu(this);
    
    QAction *saveAction = m_contextMenu->addAction("保存", this, &StickyNoteWindow::onSave);
    saveAction->setShortcut(QKeySequence::Save);
    
    QAction *copyAction = m_contextMenu->addAction("复制", this, &StickyNoteWindow::onCopy);
    copyAction->setShortcut(QKeySequence::Copy);
    
    m_contextMenu->addSeparator();
    
    m_contextMenu->addAction("最小化", this, &StickyNoteWindow::onMinimize);
    m_contextMenu->addAction("最大化", this, &StickyNoteWindow::onMaximize);
    
    m_contextMenu->addSeparator();
    
    QAction *alwaysOnTopAction = m_contextMenu->addAction("切换置顶", this, &StickyNoteWindow::onToggleAlwaysOnTop);
    alwaysOnTopAction->setCheckable(true);
    alwaysOnTopAction->setChecked(windowFlags() & Qt::WindowStaysOnTopHint);
    
    m_contextMenu->addSeparator();
    
    QAction *resetScaleAction = m_contextMenu->addAction("重置缩放", this, &StickyNoteWindow::onResetScale);
    resetScaleAction->setShortcut(QKeySequence("Ctrl+0"));
    
    m_contextMenu->addSeparator();
    
    QAction *closeAction = m_contextMenu->addAction("关闭", this, &StickyNoteWindow::onCloseClicked);
    closeAction->setShortcut(QKeySequence::Close);
}

void StickyNoteWindow::updateHandles() {
    m_handles.clear();
    
    // 添加四个角的控制点
    m_handles.append(rect().topLeft());
    m_handles.append(rect().topRight());
    m_handles.append(rect().bottomLeft());
    m_handles.append(rect().bottomRight());
    
    // 添加边中点控制点
    m_handles.append(QPoint(rect().center().x(), rect().top()));
    m_handles.append(QPoint(rect().center().x(), rect().bottom()));
    m_handles.append(QPoint(rect().left(), rect().center().y()));
    m_handles.append(QPoint(rect().right(), rect().center().y()));
}

void StickyNoteWindow::drawHandles(QPainter &painter) {
    // 使用半透明的控制点，只在鼠标悬停时显示
    painter.setPen(QPen(QColor(255, 255, 255, 150), 1));
    painter.setBrush(QColor(0, 0, 0, 100));
    
    for (const QPoint &handle : m_handles) {
        QRect handleRect(handle.x() - m_handleSize/2, handle.y() - m_handleSize/2, 
                        m_handleSize, m_handleSize);
        painter.drawEllipse(handleRect); // 使用圆形而不是方形
    }
}

bool StickyNoteWindow::isPointInHandle(const QPoint &point, const QPoint &handle) const {
    QRect handleRect(handle.x() - m_handleSize/2, handle.y() - m_handleSize/2, 
                    m_handleSize, m_handleSize);
    return handleRect.contains(point);
}

void StickyNoteWindow::updateCursor(const QPoint &pos) {
    for (int i = 0; i < m_handles.size(); ++i) {
        if (isPointInHandle(pos, m_handles[i])) {
            setCursor(Qt::SizeFDiagCursor);
            return;
        }
    }
    
    if (rect().contains(pos)) {
        setCursor(Qt::SizeAllCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void StickyNoteWindow::resizeWindow(const QPoint &pos) {
    if (m_resizeHandle < 0 || m_resizeHandle >= m_handles.size()) {
        return;
    }
    
    QPoint delta = pos - m_startPoint;
    QRect newRect = rect();
    
    switch (m_resizeHandle) {
    case 0: // 左上角
        newRect.setTopLeft(newRect.topLeft() + delta);
        break;
    case 1: // 右上角
        newRect.setTopRight(newRect.topRight() + delta);
        break;
    case 2: // 左下角
        newRect.setBottomLeft(newRect.bottomLeft() + delta);
        break;
    case 3: // 右下角
        newRect.setBottomRight(newRect.bottomRight() + delta);
        break;
    case 4: // 上边中点
        newRect.setTop(newRect.top() + delta.y());
        break;
    case 5: // 下边中点
        newRect.setBottom(newRect.bottom() + delta.y());
        break;
    case 6: // 左边中点
        newRect.setLeft(newRect.left() + delta.x());
        break;
    case 7: // 右边中点
        newRect.setRight(newRect.right() + delta.x());
        break;
    }
    
    // 确保最小尺寸
    if (newRect.width() >= m_minSize.width() && newRect.height() >= m_minSize.height()) {
        setGeometry(newRect);
        updateHandles();
        update();
    }
}

void StickyNoteWindow::setupScaleAnimation() {
    m_scaleAnimation = new QVariantAnimation(this);
    m_scaleAnimation->setDuration(140);
    m_scaleAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_scaleAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value){
        if (!m_animationInProgress) return;
        bool ok = false; float newScale = value.toFloat(&ok);
        if (ok) { applyScaleFactor(newScale, true); }
    });
    connect(m_scaleAnimation, &QVariantAnimation::finished, this, &StickyNoteWindow::onScaleAnimationFinished);
}

void StickyNoteWindow::startSmoothScale(float targetScale) {
    targetScale = qBound(0.1f, targetScale, 5.0f);
    if (qFuzzyCompare(targetScale, m_scaleFactor)) return;
    m_targetScaleFactor = targetScale;
    m_animationInProgress = true;
    if (m_scaleAnimation) {
        m_scaleAnimation->stop();
        m_scaleAnimation->setStartValue(m_scaleFactor);
        m_scaleAnimation->setEndValue(targetScale);
        m_scaleAnimation->start();
    } else {
        applyScaleFactor(targetScale, false);
        m_scaleFactor = targetScale;
    }
}

void StickyNoteWindow::setScaleFactor(float scale) {
    if (qFuzzyCompare(scale, m_scaleFactor)) return;
    m_scaleFactor = scale;
    updateWindowSize();
}

void StickyNoteWindow::updateWindowSize() {
    const int targetWidth = qMax(1, qRound(m_originalSize.width() * m_scaleFactor));
    const int targetHeight = qMax(1, qRound(m_originalSize.height() * m_scaleFactor));
    QPoint center = geometry().center();
    QRect newGeometry(0, 0, targetWidth, targetHeight);
    newGeometry.moveCenter(center);
    setGeometry(newGeometry);
    updateHandles();
    update();
}

void StickyNoteWindow::applyScaleFactor(float scale, bool duringAnimation) {
    if (duringAnimation && qFuzzyCompare(scale, m_scaleFactor)) return;
    QPoint centerPoint = geometry().center();
    float newWidthF = m_originalSize.width() * scale;
    float newHeightF = m_originalSize.height() * scale;
    int newWidth = qMax(1, qRound(newWidthF));
    int newHeight = qMax(1, qRound(newHeightF));
    QRect newGeometry(0, 0, newWidth, newHeight);
    newGeometry.moveCenter(centerPoint);
    if (duringAnimation) {
        setGeometry(newGeometry);
    } else {
        resize(newWidth, newHeight);
        move(newGeometry.topLeft());
    }
    m_scaleFactor = scale;
    updateHandles();
    update();
}

void StickyNoteWindow::onSave() {
    emit saveRequested();
}

void StickyNoteWindow::onCopy() {
    emit copyRequested();
}

void StickyNoteWindow::onMinimize() {
    showMinimized();
}

void StickyNoteWindow::onMaximize() {
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void StickyNoteWindow::onToggleAlwaysOnTop() {
    toggleAlwaysOnTop();
}

void StickyNoteWindow::onResetScale() {
    startSmoothScale(1.0f);
}
