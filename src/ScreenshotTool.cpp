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
 * @file ScreenshotTool.cpp
 * @brief 截图工具核心类实现
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include "ScreenshotTool.h"
#include "RegionSelector.h"
#include "ScreenshotEditWindow.h"
#include "StickyNoteWindow.h"
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QClipboard>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <climits>
#include <QStandardPaths>
#include <QDir>

ScreenshotTool::ScreenshotTool(QObject *parent)
    : QObject(parent)
    , m_regionSelector(nullptr)
    , m_lastEditPos()
    , m_lastCaptureTopLeft()
    , m_delayedCaptureTimer(nullptr)
{
    // 初始化延迟截图定时器
    m_delayedCaptureTimer = new QTimer(this);
    m_delayedCaptureTimer->setSingleShot(true);
    
    connect(m_delayedCaptureTimer, &QTimer::timeout, this, [this]() {
        qDebug() << "[DelayedCapture] Timer triggered, capturing full screen";
        
        // 延迟一小段时间，确保弹窗稳定显示
        QTimer::singleShot(100, this, [this]() {
            QPixmap screenshot = captureFullScreen();
            if (!screenshot.isNull()) {
                // 自动复制到剪贴板
                QApplication::clipboard()->setPixmap(screenshot);
                qDebug() << "[DelayedCapture] Screenshot captured and copied to clipboard";
                
                // 显示编辑窗口
                showScreenshotEditWindow(screenshot, QPoint(100, 100));
                emit screenshotCaptured(screenshot);
            }
        });
    });
}

ScreenshotTool::~ScreenshotTool()
{
    // 析构函数
}

QPixmap ScreenshotTool::captureRegion(const QRect &globalRect) {
    if (globalRect.isEmpty()) return QPixmap();

    // 调试信息：显示虚拟桌面信息（按照文档方案）
    const QList<QScreen*> screens = QGuiApplication::screens();
    qDebug() << "[VirtualDesktop] screens:" << screens.count();
    for (auto screen : screens) {
        qDebug() << "  -" << screen->name() << screen->geometry() << "DPI:" << screen->devicePixelRatio();
    }

    qDebug() << "[Selection] globalRect:" << globalRect;

    // 收集所有屏幕的截图信息
    QList<ScreenCaptureInfo> captures;
    
    // 1. 遍历所有屏幕，计算交集
    for (QScreen *screen : screens) {
        if (!screen) continue;
        
        QRect screenGeo = screen->geometry();
        QRect intersection = globalRect.intersected(screenGeo);
        
        if (!intersection.isEmpty()) {
            // 2. 坐标转换：全局 → 屏幕本地
            QRect localRect = intersection.translated(-screenGeo.topLeft());
            
            // 3. 抓取屏幕区域
            QPixmap segment = screen->grabWindow(0, 
                localRect.x(), localRect.y(),
                localRect.width(), localRect.height());
            
            if (!segment.isNull()) {
                captures.append({segment, intersection.topLeft()});
                qDebug() << "[Capture] screen=" << screen->name()
                         << " intersection=" << intersection
                         << " localRect=" << localRect
                         << " segmentSize=" << segment.size()
                         << " segmentDPR=" << segment.devicePixelRatio();
            }
        }
    }
    
    qDebug() << "[Capture] intersections:" << captures.count();
    
    // 4. 判断模式并处理
    if (captures.size() == 1) {
        return captures[0].pixmap;  // 单屏直接返回
    } else if (captures.size() > 1) {
        return mergeMultiScreenCaptures(captures, globalRect.size());
    }
    
    return QPixmap();
}

QPixmap ScreenshotTool::mergeMultiScreenCaptures(const QList<ScreenCaptureInfo> &captures, const QSize &logicalSize) {
    qDebug() << "[ImageMerge] Starting multi-screen merge with" << captures.size() << "captures, logicalSize:" << logicalSize;
    
    // 创建逻辑尺寸画布
    QImage canvas(logicalSize, QImage::Format_ARGB32_Premultiplied);
    canvas.fill(Qt::transparent);
    
    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    // 找到选区左上角作为基准点
    QPoint selectionTopLeft = QPoint(INT_MAX, INT_MAX);
    for (const auto &capture : captures) {
        selectionTopLeft.setX(qMin(selectionTopLeft.x(), capture.globalPos.x()));
        selectionTopLeft.setY(qMin(selectionTopLeft.y(), capture.globalPos.y()));
    }
    
    qDebug() << "[ImageMerge] selectionTopLeft:" << selectionTopLeft;
    
    for (int i = 0; i < captures.size(); ++i) {
        const auto &capture = captures[i];
        // 计算在画布中的位置（相对于选区左上角）
        QPoint position = capture.globalPos - selectionTopLeft;
        
        // 缩放到逻辑尺寸（处理不同DPI）
        QPixmap scaled = scaleToLogicalSize(capture.pixmap, capture.pixmap.size() / capture.pixmap.devicePixelRatio());
        
        qDebug() << "[ImageMerge] capture" << i 
                 << " globalPos:" << capture.globalPos
                 << " position:" << position
                 << " originalSize:" << capture.pixmap.size()
                 << " scaledSize:" << scaled.size();
        
        painter.drawPixmap(position, scaled);
    }
    
    qDebug() << "[ImageMerge] Final canvas size:" << canvas.size();
    return QPixmap::fromImage(canvas);
}

QPixmap ScreenshotTool::scaleToLogicalSize(const QPixmap &pixmap, const QSize &logicalSize) {
    if (pixmap.size() == logicalSize) {
        return pixmap;
    }
    return pixmap.scaled(logicalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void ScreenshotTool::startRegionCapture() {
    if (!m_regionSelector) {
        m_regionSelector = new RegionSelector();
        connect(m_regionSelector, &RegionSelector::regionSelected,
                this, &ScreenshotTool::onRegionSelected);
        connect(m_regionSelector, &RegionSelector::selectionCancelled,
                this, &ScreenshotTool::onSelectionCancelled);
    }
    
    m_regionSelector->startSelection();
}

void ScreenshotTool::startDelayedCapture(int delayMs) {
    qDebug() << "[DelayedCapture] Starting delayed capture with delay:" << delayMs << "ms";
    
    // 创建倒计时提示窗口
    QWidget *countdownWidget = new QWidget(nullptr, Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    countdownWidget->setAttribute(Qt::WA_TranslucentBackground);
    countdownWidget->setAttribute(Qt::WA_DeleteOnClose);
    
    QLabel *countdownLabel = new QLabel(countdownWidget);
    countdownLabel->setAlignment(Qt::AlignCenter);
    countdownLabel->setStyleSheet(
        "QLabel { "
        "background-color: rgba(0, 0, 0, 180); "
        "color: #00FF00; "
        "font-size: 48px; "
        "font-weight: bold; "
        "border-radius: 10px; "
        "padding: 20px 40px; "
        "}"
    );
    
    QVBoxLayout *layout = new QVBoxLayout(countdownWidget);
    layout->addWidget(countdownLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // 居中显示
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeo = screen->geometry();
    countdownWidget->setGeometry(
        screenGeo.center().x() - 150,
        screenGeo.center().y() - 100,
        300, 200
    );
    
    countdownWidget->show();
    
    // 倒计时更新
    int remainingSeconds = delayMs / 1000;
    countdownLabel->setText(QString("准备弹窗\n%1").arg(remainingSeconds));
    
    QTimer *countdownTimer = new QTimer(countdownWidget);
    countdownTimer->setInterval(1000);
    
    connect(countdownTimer, &QTimer::timeout, countdownWidget, [countdownLabel, &remainingSeconds]() {
        remainingSeconds--;
        if (remainingSeconds > 0) {
            countdownLabel->setText(QString("准备弹窗\n%1").arg(remainingSeconds));
        } else {
            countdownLabel->setText("截图中...");
        }
    });
    
    countdownTimer->start();
    
    // 延迟截图完成后关闭倒计时窗口
    QTimer::singleShot(delayMs + 500, countdownWidget, [countdownWidget, countdownTimer]() {
        countdownTimer->stop();
        countdownWidget->close();
    });
    
    m_delayedCaptureTimer->start(delayMs);
}

QPixmap ScreenshotTool::captureFullScreen() {
    qDebug() << "[FullScreenCapture] Starting full screen capture";
    
    // 获取所有屏幕
    const QList<QScreen*> screens = QGuiApplication::screens();
    if (screens.isEmpty()) {
        qWarning() << "[FullScreenCapture] No screens found";
        return QPixmap();
    }
    
    // 计算虚拟桌面总尺寸
    QRect virtualRect = QGuiApplication::primaryScreen()->virtualGeometry();
    qDebug() << "[FullScreenCapture] Virtual desktop geometry:" << virtualRect;
    
    // 使用更底层的方式截取屏幕，确保能截取到所有内容
    QPixmap fullScreenPixmap;
    
    // 尝试使用 QScreen::grabWindow 截取整个虚拟桌面
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen) {
        // 截取整个虚拟桌面
        fullScreenPixmap = primaryScreen->grabWindow(0, 
            virtualRect.x(), virtualRect.y(),
            virtualRect.width(), virtualRect.height());
        
        qDebug() << "[FullScreenCapture] Virtual desktop captured, size:" << fullScreenPixmap.size();
    }
    
    // 如果虚拟桌面截图失败，回退到多屏截图
    if (fullScreenPixmap.isNull()) {
        qDebug() << "[FullScreenCapture] Virtual desktop capture failed, falling back to multi-screen capture";
        
        fullScreenPixmap = QPixmap(virtualRect.size());
        fullScreenPixmap.fill(Qt::transparent);
        
        QPainter painter(&fullScreenPixmap);
        
        // 遍历所有屏幕进行截图
        for (QScreen *screen : screens) {
            if (!screen) continue;
            
            QRect screenGeo = screen->geometry();
            qDebug() << "[FullScreenCapture] Capturing screen:" << screen->name() << "geometry:" << screenGeo;
            
            // 截取屏幕
            QPixmap screenPixmap = screen->grabWindow(0);
            
            if (!screenPixmap.isNull()) {
                // 计算在虚拟桌面中的位置
                QPoint offset = screenGeo.topLeft() - virtualRect.topLeft();
                painter.drawPixmap(offset, screenPixmap);
                qDebug() << "[FullScreenCapture] Screen captured, offset:" << offset << "size:" << screenPixmap.size();
            }
        }
        
        painter.end();
    }
    
    qDebug() << "[FullScreenCapture] Full screen capture completed, size:" << fullScreenPixmap.size();
    return fullScreenPixmap;
}

bool ScreenshotTool::saveScreenshot(const QPixmap &pixmap, const QString &filePath) {
    return pixmap.save(filePath);
}

void ScreenshotTool::showScreenshotEditWindow(const QPixmap &pixmap, const QPoint &initialPos) {
    // 先创建窗口，不立刻决定位置
    ScreenshotEditWindow *editWindow = new ScreenshotEditWindow(pixmap, QPoint());
    
    // 连接信号
    connect(editWindow, &ScreenshotEditWindow::saveRequested, [this, editWindow](const QPixmap &screenshot) {
        QString fileName = QFileDialog::getSaveFileName(
            editWindow, "保存截图", 
            QString("screenshot_%1.png").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
            "PNG图片 (*.png);;JPEG图片 (*.jpg);;所有文件 (*.*)"
        );
        
        if (!fileName.isEmpty()) {
            if (saveScreenshot(screenshot, fileName)) {
                // 成功后不弹出提示框
            } else {
                QMessageBox::critical(editWindow, "错误", "保存截图失败！");
            }
        }
        m_lastEditPos = editWindow->pos();
    });
    
    connect(editWindow, &ScreenshotEditWindow::copyRequested, [editWindow](const QPixmap &screenshot) {
        QApplication::clipboard()->setPixmap(screenshot);
        // 复制后不弹出提示框
    });
    
    connect(editWindow, &ScreenshotEditWindow::createStickyNoteRequested, [this, editWindow](const QPixmap &screenshot) {
        // 修复：使用编辑窗口的当前位置，而不是原始截图位置
        // 这样用户移动编辑窗口后，贴图会在移动后的位置创建
        QPoint posToUse = editWindow->pos();
        qDebug() << "[StickyNote] Using edit window position:" << posToUse;
        createStickyNote(screenshot, posToUse);
        m_lastEditPos = editWindow->pos();
        editWindow->deleteLater();
        // 创建贴图后，通知主窗口重新显示
        emit editWindowClosed();
    });
    
    connect(editWindow, &ScreenshotEditWindow::closeRequested, [this, editWindow]() {
        m_lastEditPos = editWindow->pos();
        editWindow->deleteLater();
        // 关闭截图编辑窗口后，通知主窗口重新显示
        emit editWindowClosed();
    });
    
    // 计算期望位置：优先用选区左上角，否则沿用历史/活动窗口
    QPoint desired = initialPos;
    if (desired.isNull()) {
        if (!m_lastEditPos.isNull()) desired = m_lastEditPos;
        else if (QWidget *aw = QApplication::activeWindow()) desired = aw->pos();
        else desired = QPoint(100, 100); // 默认位置
    }
    
    qDebug() << "[EditWindow] desired pos:" << desired << " winSize:" << editWindow->size();
    
    // 将窗口限制在所在屏幕的可见区域内，避免被系统强制挪动
    QScreen *scr = QGuiApplication::screenAt(desired);
    if (!scr) scr = QGuiApplication::primaryScreen();
    QRect avail = scr ? scr->availableGeometry() : QRect(0,0,1920,1080);
    
    qDebug() << "[EditWindow] screen:" << (scr ? scr->name() : "null") 
             << " availableGeometry:" << avail;
    
    // 取窗口当前大小（已按图片尺寸设置）
    QSize winSize = editWindow->size();
    
    // 安全的位置计算：避免窗口比屏幕还大导致的负上界问题
    QPoint finalPos;
    if (winSize.width() > avail.width() || winSize.height() > avail.height()) {
        // 窗口比屏幕还大，贴靠屏幕左上角
        finalPos = avail.topLeft();
        qDebug() << "[EditWindow] window too large, using screen top-left:" << finalPos;
    } else {
        // 正常情况：在可用区域内裁剪
        int x = qBound(avail.left(), desired.x(), avail.right() - winSize.width() + 1);
        int y = qBound(avail.top(),  desired.y(), avail.bottom() - winSize.height() + 1);
        finalPos = QPoint(x, y);
    }
    
    qDebug() << "[EditWindow] finalPos:" << finalPos;
    editWindow->move(finalPos);

    // 显示编辑窗口
    editWindow->show();
    editWindow->raise();
    editWindow->activateWindow();
    m_lastEditPos = editWindow->pos();
}

void ScreenshotTool::createStickyNote(const QPixmap &pixmap, const QPoint &initialPos) {
    StickyNoteWindow *stickyNote = new StickyNoteWindow(pixmap);
    
    // 贴图默认不置顶，避免遮挡对话框
    // 注意：构造函数已经设置了正确的窗口属性，这里不需要重新设置
    
    // 连接信号
    connect(stickyNote, &StickyNoteWindow::closeRequested, [stickyNote]() {
        stickyNote->deleteLater();
    });
    
    connect(stickyNote, &StickyNoteWindow::saveRequested, [this, stickyNote]() {
        QString fileName = QFileDialog::getSaveFileName(
            stickyNote, "保存贴图", 
            QString("sticky_note_%1.png").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
            "PNG图片 (*.png);;JPEG图片 (*.jpg);;所有文件 (*.*)"
        );
        
        if (!fileName.isEmpty()) {
            if (saveScreenshot(stickyNote->getPixmap(), fileName)) {
                // 成功后不弹出提示框
            } else {
                QMessageBox::critical(stickyNote, "错误", "保存贴图失败！");
            }
        }
    });
    
    connect(stickyNote, &StickyNoteWindow::copyRequested, [stickyNote]() {
        QApplication::clipboard()->setPixmap(stickyNote->getPixmap());
        // 复制后不弹出提示框
    });
    
    // 显示贴图窗口
    stickyNote->show();
    stickyNote->raise();
    stickyNote->activateWindow();
    stickyNote->setWindowState(Qt::WindowActive);

    // 把贴图放到传入的位置（原始截图区域左上角），如果没传则保持当前默认
    if (!initialPos.isNull()) {
        stickyNote->move(initialPos);
    }
}

void ScreenshotTool::onRegionSelected(const QRect &rect) {
    // 确保区域有效
    if (rect.isEmpty() || rect.width() < 10 || rect.height() < 10) {
        return;
    }

    auto rectStr = [](const QRect &r){ return QString("(%1,%2,%3x%4)").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height()); };
    qDebug() << "[Capture] onRegionSelected rect=" << rectStr(rect);

    // 关键修复：从冻结的背景中截取选择的区域（单屏模式）
    QPixmap screenshot;
    if (m_regionSelector && !m_regionSelector->getFrozenBackground().isNull()) {
        // 使用冻结的背景画面
        QPixmap frozenBg = m_regionSelector->getFrozenBackground();
        qreal dpr = frozenBg.devicePixelRatio();
        
        qDebug() << "[Capture] Using frozen background (single screen), size:" << frozenBg.size() 
                 << "DPR:" << dpr
                 << "logical size:" << (frozenBg.size() / dpr);
        
        // 获取选区所在屏幕的几何信息（使用选区左上角确定屏幕）
        QScreen *currentScreen = QGuiApplication::screenAt(rect.topLeft());
        if (!currentScreen) {
            currentScreen = QGuiApplication::primaryScreen();
        }
        QRect screenGeometry = currentScreen->geometry();
        
        qDebug() << "[Capture] Screen:" << currentScreen->name()
                 << "geometry:" << screenGeometry;
        
        // 将全局坐标转换为屏幕本地坐标（逻辑像素）
        QRect captureRect = rect.translated(-screenGeometry.topLeft());
        
        qDebug() << "[Capture] Capture rect (logical):" << captureRect
                 << "screen geometry:" << screenGeometry
                 << "selected rect:" << rect;
        
        // 根据DPR转换为设备像素坐标
        QRect deviceCaptureRect;
        if (dpr > 1.0) {
            // 高DPI: 将逻辑像素坐标转换为设备像素坐标
            deviceCaptureRect = QRect(
                qRound(captureRect.x() * dpr),
                qRound(captureRect.y() * dpr),
                qRound(captureRect.width() * dpr),
                qRound(captureRect.height() * dpr)
            );
            qDebug() << "[Capture] Device capture rect:" << deviceCaptureRect;
        } else {
            // 普通DPI: 直接使用逻辑像素
            deviceCaptureRect = captureRect;
        }
        
        // 从冻结的背景中截取（使用设备像素坐标）
        screenshot = frozenBg.copy(deviceCaptureRect);
        
        // 保持原始DPR，确保高分辨率
        screenshot.setDevicePixelRatio(dpr);
        
        qDebug() << "[Capture] Screenshot size:" << screenshot.size() 
                 << "DPR:" << screenshot.devicePixelRatio()
                 << "logical size:" << (screenshot.size() / dpr);
    } else {
        // 回退：如果没有冻结背景，使用实时截图
        qDebug() << "[Capture] No frozen background, using live capture";
        screenshot = captureRegion(rect);
    }

    // 先隐藏选择器
    if (m_regionSelector) {
        m_regionSelector->releaseKeyboard();
        m_regionSelector->releaseMouse();
        m_regionSelector->hide();
    }

    if (!screenshot.isNull()) {
        // 记录最新选择的区域左上角，供贴图使用
        m_lastCaptureTopLeft = rect.topLeft();
        
        // 修复：自动复制截图到剪贴板，用户可以直接Ctrl+V粘贴
        QApplication::clipboard()->setPixmap(screenshot);
        qDebug() << "[AutoCopy] Screenshot automatically copied to clipboard";
        
        // 自动保存到历史文件夹
        saveToHistory(screenshot);
        
        // 编辑窗口位置：使用选区的全局坐标作为初始位置
        showScreenshotEditWindow(screenshot, rect.topLeft());
        // 同时通知外部（主窗口仅用于隐藏自身）
        emit screenshotCaptured(screenshot);
    }
}

void ScreenshotTool::onSelectionCancelled() {
    // 用户取消了区域选择
}

void ScreenshotTool::saveToHistory(const QPixmap &screenshot) {
    // 获取历史截图文件夹路径
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString historyPath = appData + "/ScreenshotHistory/images";
    
    // 确保目录存在
    QDir dir;
    if (!dir.exists(historyPath)) {
        if (!dir.mkpath(historyPath)) {
            qWarning() << "[History] Failed to create history folder:" << historyPath;
            return;
        }
    }
    
    // 生成文件名：年月日_时分秒.png
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".png";
    QString filePath = historyPath + "/" + fileName;
    
    // 保存截图
    if (screenshot.save(filePath)) {
        qDebug() << "[History] Screenshot saved to:" << filePath;
    } else {
        qWarning() << "[History] Failed to save screenshot to:" << filePath;
    }
}
