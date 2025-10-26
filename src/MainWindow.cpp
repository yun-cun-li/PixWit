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
 * @file MainWindow.cpp
 * @brief 主窗口类实现
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include "MainWindow.h"
#include "ScreenshotTool.h"
#include "GlobalHotkey.h"
#include "UpdateChecker.h"
#include <QApplication>
#include <QSettings>
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
#include <QSystemTrayIcon>
#include <QTimer>
#include <QIcon>
#include <QLocalServer>
#include <QLocalSocket>
#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_screenshotTool(nullptr)
    , m_currentScreenshot()
    , m_isDragging(false)
    , m_dragOffset()
    , m_lastPosition()
    , m_tray(nullptr)
    , m_hotkey(nullptr)
    , m_toggleShowAction(nullptr)
    , m_updateChecker(nullptr)
    , m_localServer(nullptr)
{
    setupUI();
    setupConnections();
    moveToRightEdge(); // 移动到桌面右边缘中间位置
    
    // macOS风格柔和阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(30);  // 更大的模糊半径
    shadowEffect->setColor(QColor(0, 0, 0, 40));  // 更浅的阴影
    shadowEffect->setOffset(0, 6);  // 稍微向下偏移
    setGraphicsEffect(shadowEffect);

    // 托盘与热键
    setupTray();
    m_hotkey = new GlobalHotkey(this);
    connect(m_hotkey, &GlobalHotkey::hotkeyPressed, this, &MainWindow::onRegionCapture);

    // 设置自动更新检查
    setupUpdateChecker();
    
    // 设置本地服务器，用于单实例通信
    m_localServer = new QLocalServer(this);
    connect(m_localServer, &QLocalServer::newConnection, this, &MainWindow::onNewConnection);
    // 移除可能存在的旧服务器
    QLocalServer::removeServer("CapStepInstance");
    if (!m_localServer->listen("CapStepInstance")) {
        qDebug() << "[LocalServer] Failed to start:" << m_localServer->errorString();
    } else {
        qDebug() << "[LocalServer] Started successfully";
    }

    // 方案A：启动后显示主界面（不自动隐藏到托盘）
    // 用户可以看到程序已启动，了解快捷键等信息
}

MainWindow::~MainWindow()
{
    // 析构函数
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // macOS风格毛玻璃背景
    QRect bgRect = rect();
    
    // 绘制半透明白色背景（毛玻璃效果）
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(255, 255, 255, 240));
    gradient.setColorAt(1, QColor(245, 245, 250, 235));
    
    QPainterPath path;
    path.addRoundedRect(bgRect, 12, 12);  // 更大的圆角
    painter.fillPath(path, gradient);
    
    // 添加微妙的边框
    painter.setPen(QPen(QColor(0, 0, 0, 25), 1));
    painter.drawPath(path);
    
    // 内侧高光效果（顶部）
    QPen highlightPen(QColor(255, 255, 255, 80), 1);
    painter.setPen(highlightPen);
    QPainterPath highlightPath;
    highlightPath.addRoundedRect(bgRect.adjusted(1, 1, -1, -height()/2), 11, 11);
    painter.drawPath(highlightPath);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragOffset = event->pos();  // 记录拖拽起始点
        m_isDragging = true;          // 设置拖拽标志
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        QPoint newPos = event->globalPosition().toPoint() - m_dragOffset;  // 计算新位置
        move(newPos);  // 移动窗口
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    m_isDragging = false;  // 清除拖拽标志
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    Q_UNUSED(event)
    // 键盘事件处理
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_tray && m_tray->isVisible()) {
        hide();
        event->ignore();
    } else {
        event->accept();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // 只处理鼠标事件
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseMove ||
        event->type() == QEvent::MouseButtonRelease) {
        
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        
        // 将子控件的鼠标事件转换为相对于主窗口的坐标
        QWidget *widget = qobject_cast<QWidget*>(obj);
        if (widget) {
            QPoint globalPos = widget->mapToGlobal(mouseEvent->pos());
            QPoint localPos = mapFromGlobal(globalPos);
            
            // 创建新的鼠标事件，坐标相对于主窗口
            QMouseEvent newEvent(mouseEvent->type(),
                               localPos,
                               globalPos,
                               mouseEvent->button(),
                               mouseEvent->buttons(),
                               mouseEvent->modifiers());
            
            // 根据事件类型调用相应的处理方法
            switch (event->type()) {
            case QEvent::MouseButtonPress:
                mousePressEvent(&newEvent);
                // 对于按钮，如果是拖拽意图（移动距离小），优先处理拖拽
                if (qobject_cast<QPushButton*>(obj)) {
                    return false; // 让按钮也能响应点击
                }
                return false;
            case QEvent::MouseMove:
                mouseMoveEvent(&newEvent);
                // 如果正在拖拽，阻止按钮的悬停效果
                if (m_isDragging) {
                    return true;
                }
                return false;
            case QEvent::MouseButtonRelease:
                mouseReleaseEvent(&newEvent);
                return false;
            default:
                break;
            }
        }
    }
    
    // 其他事件交给父类处理
    return QWidget::eventFilter(obj, event);
}

void MainWindow::onFullScreenCapture() {
    // 全屏截图功能已移除，只保留区域截图
    onRegionCapture();
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        show();
        raise();
        activateWindow();
        break;
    case QSystemTrayIcon::Trigger:
        // 单击托盘图标显示主界面
        show();
        raise();
        activateWindow();
        break;
    default:
        break;
    }
}

void MainWindow::onQuit() {
    QApplication::quit();
}

void MainWindow::onShow() {
    show();
    raise();
    activateWindow();
}

void MainWindow::onNewConnection() {
    // 有新的实例尝试启动，显示并置顶主窗口
    QLocalSocket *socket = m_localServer->nextPendingConnection();
    if (socket) {
        qDebug() << "[LocalServer] Received connection from another instance";
        
        // 读取消息（如果有）
        socket->waitForReadyRead(100);
        QByteArray data = socket->readAll();
        qDebug() << "[LocalServer] Received message:" << data;
        
        // 显示主窗口（如果已显示则置顶）
        if (isVisible()) {
            qDebug() << "[LocalServer] Window already visible, raising to top";
        } else {
            qDebug() << "[LocalServer] Showing window";
        }
        show();          // 如果隐藏则显示
        raise();         // 置顶
        activateWindow(); // 激活获取焦点
        
        // 关闭连接
        socket->disconnectFromServer();
        socket->deleteLater();
    }
}

void MainWindow::onRegionCapture() {
    // 检查是否有模态窗口正在显示
    if (QApplication::activeModalWidget() != nullptr) {
        qDebug() << "[MainWindow] Modal dialog is active, ignoring hotkey";
        return;
    }
    
    // 开始截图，主界面保持当前显示状态
    m_lastPosition = pos();
    m_screenshotTool->startRegionCapture();
}

void MainWindow::onDelayedCapture() {
    // 检查是否有模态窗口正在显示
    if (QApplication::activeModalWidget() != nullptr) {
        qDebug() << "[MainWindow] Modal dialog is active, ignoring hotkey";
        return;
    }
    
    // F1 键触发画面冻结+区域截图
    qDebug() << "[MainWindow] F1 pressed, freezing screen and starting region capture";
    
    // 立即截取全屏画面（冻结当前画面）
    m_screenshotTool->startRegionCapture();
}

void MainWindow::onScreenshotCaptured(const QPixmap &screenshot) {
    m_currentScreenshot = screenshot;
    // 编辑窗口已在工具类内部弹出，主窗口保持当前显示状态
    m_lastPosition = pos();
}

void MainWindow::onEditWindowClosed() {
    // 编辑窗口关闭后，不自动弹出主界面，仅恢复位置，保持在托盘
    QTimer::singleShot(100, [this]() {
        if (!m_lastPosition.isNull()) {
            move(m_lastPosition);
        }
    });
}

void MainWindow::setupUI() {
    // 设置窗口属性
    setWindowTitle("CapStep - 截图工具");
    
    // 设置窗口图标（任务栏显示）
    setWindowIcon(QIcon(":/icons/resources/icons/app.ico"));
    
    setFixedWidth(60); // 固定宽度，使用布局自动计算高度，消除垂直间隙
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground); // 启用透明背景
    setAttribute(Qt::WA_NoSystemBackground);    // 避免系统背景
    setAttribute(Qt::WA_OpaquePaintEvent, false); // 非不透明绘制事件
    
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 5, 0); // 去除上下间距
    mainLayout->setSpacing(0); // 去除控件之间的垂直间距
    mainLayout->setSizeConstraint(QLayout::SetFixedSize); // 高度自适应内容
    
    // 创建关闭按钮
    QPushButton *minimizeBtn = new QPushButton("×");
    minimizeBtn->setFixedSize(50, 22); // 紧凑但保证易点
    
    // 主功能按钮
    QPushButton *regionBtn = new QPushButton();
    
    // 设置按钮图标（不显示文字）
    regionBtn->setIcon(QIcon(":/icons/resources/icons/area_screenshot.svg"));
    regionBtn->setIconSize(QSize(32, 32));  // 更大的图标
    regionBtn->setText(""); // 不显示文字
    regionBtn->setToolTip("区域截图 (F1)"); // 添加工具提示
    
    // 设置按钮大小 - macOS风格
    QSize buttonSize(50, 50); // 正方形按钮
    regionBtn->setFixedSize(buttonSize);
    
    // 按钮布局 - 极度紧凑
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0); // 无间距
    buttonLayout->setAlignment(Qt::AlignCenter);
    
    buttonLayout->addWidget(regionBtn);
    
    // 创建专门的拖拽区域（顶部）- 更明显的拖拽指示符
    QLabel *dragArea = new QLabel();
    dragArea->setFixedHeight(18);
    dragArea->setAlignment(Qt::AlignCenter);
    dragArea->setText("⋮⋮⋮"); // 拖拽指示符
    dragArea->setStyleSheet("QLabel { "
                           "color: rgba(80, 80, 80, 0.9); "
                           "font-size: 14px; "
                           "font-weight: bold; "
                           "background: rgba(255, 255, 255, 0.1); "
                           "border-radius: 8px; "
                           "padding: 1px; "
                           "}");
    dragArea->setCursor(Qt::SizeAllCursor); // 设置拖拽光标
    dragArea->installEventFilter(this);
    
    // 添加到主布局
    mainLayout->addWidget(dragArea); // 拖拽区域在顶部
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(minimizeBtn); // 关闭按钮在底部
    
    // 设置样式
    setupStyles(regionBtn, minimizeBtn);
    // 去除按钮之间的圆角外间隔造成的视觉缝隙
    regionBtn->setStyleSheet(regionBtn->styleSheet() + "\nmargin: 0px;\n");
    minimizeBtn->setStyleSheet(minimizeBtn->styleSheet() + "\nmargin: 0px;\n");
    
    // 连接信号
    connect(regionBtn, &QPushButton::clicked, this, &MainWindow::onRegionCapture);
    connect(minimizeBtn, &QPushButton::clicked, this, &MainWindow::hide);
    
    // 安装事件过滤器，让按钮区域也能拖拽
    regionBtn->installEventFilter(this);
}

void MainWindow::setupStyles(QPushButton *regionBtn, QPushButton *minimizeBtn) {
    // 主窗口样式 - 透明背景
    setStyleSheet(R"(
        MainWindow {
            background: transparent;
        }
        QLabel {
            color: rgba(108, 117, 125, 0.8);
            font-size: 12px;
            background: transparent;
        }
    )");
    
    // macOS风格按钮样式
    QString buttonStyle = R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(255, 255, 255, 0.95),
                stop:1 rgba(248, 249, 250, 0.90));
            border: 1px solid rgba(0, 0, 0, 0.08);
            border-radius: 12px;
            padding: 0px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(245, 248, 255, 1.0),
                stop:1 rgba(235, 242, 255, 0.95));
            border: 1px solid rgba(0, 123, 255, 0.25);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(230, 235, 245, 0.95),
                stop:1 rgba(240, 243, 248, 0.90));
            border: 1px solid rgba(0, 123, 255, 0.4);
        }
    )";
    
    regionBtn->setStyleSheet(buttonStyle);
    
    // macOS风格关闭按钮 - 柔和红色
    minimizeBtn->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(255, 95, 87, 0.90),
                stop:1 rgba(255, 82, 74, 0.85));
            border: 1px solid rgba(255, 255, 255, 0.3);
            border-radius: 9px;
            color: white;
            font-size: 12px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(255, 105, 97, 0.95),
                stop:1 rgba(255, 92, 84, 0.90));
            border: 1px solid rgba(255, 255, 255, 0.5);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(245, 75, 67, 0.95),
                stop:1 rgba(235, 65, 57, 0.90));
        }
    )");
}

void MainWindow::setupConnections() {
    m_screenshotTool = new ScreenshotTool(this);
    connect(m_screenshotTool, &ScreenshotTool::screenshotCaptured,
            this, &MainWindow::onScreenshotCaptured);
    connect(m_screenshotTool, &ScreenshotTool::editWindowClosed,
            this, &MainWindow::onEditWindowClosed);
}

void MainWindow::setupTray() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }
    // 全局单例托盘，避免出现多个图标
    static QSystemTrayIcon *s_traySingleton = nullptr;
    if (s_traySingleton) {
        m_tray = s_traySingleton;
        return;
    }
    if (m_tray) return;
    m_tray = new QSystemTrayIcon(this);
    s_traySingleton = m_tray;
    // 使用应用程序主图标作为托盘图标
    QIcon appIcon(":/icons/resources/icons/app.ico");
    if (appIcon.isNull()) {
        // 如果主图标加载失败，使用region.png作为备选
        appIcon = QIcon(":/icons/resources/icons/region.png");
    }
    m_tray->setIcon(appIcon);
    m_tray->setToolTip("CapStep");
    QMenu *menu = new QMenu(this);
    QAction *actHistory = menu->addAction("历史截图");
    QMenu *hotkeyMenu = menu->addMenu("快捷键");
    QAction *hkF1 = hotkeyMenu->addAction("F1");
    QAction *hkF2 = hotkeyMenu->addAction("F2");
    QAction *hkF3 = hotkeyMenu->addAction("F3");
    hkF1->setCheckable(true);
    hkF2->setCheckable(true);
    hkF3->setCheckable(true);
    hkF1->setChecked(true);
    
    // 显示/隐藏主界面 - 使用checkable状态
    m_toggleShowAction = menu->addAction("显示主界面");
    m_toggleShowAction->setCheckable(true);
    m_toggleShowAction->setChecked(false);  // 默认隐藏
    
    menu->addSeparator();
    
    // 开机自动启动选项
    QAction *actAutoStart = menu->addAction("开机自动启动");
    actAutoStart->setCheckable(true);
    actAutoStart->setChecked(isAutoStartEnabled());
    
    QAction *actCheckUpdate = menu->addAction("检查更新");
    
    QAction *actQuit = menu->addAction("退出");
    
    connect(actHistory, &QAction::triggered, this, &MainWindow::onOpenHistoryFolder);
    
    // 切换主界面显示/隐藏
    connect(m_toggleShowAction, &QAction::triggered, this, [this]() {
        if (isVisible()) {
            // 当前可见，点击后隐藏
            qDebug() << "[Tray] Hiding main window";
            hide();
        } else {
            // 当前隐藏，点击后显示
            qDebug() << "[Tray] Showing main window";
            show();
            raise();
            activateWindow();
        }
    });
    
    // 开机自动启动
    connect(actAutoStart, &QAction::triggered, this, [this, actAutoStart](bool checked) {
        setAutoStartEnabled(checked);
        qDebug() << "[Tray] Auto-start" << (checked ? "enabled" : "disabled");
    });
    
    // 检查更新
    connect(actCheckUpdate, &QAction::triggered, this, [this]() {
        qDebug() << "[Tray] Check update menu clicked";
        qDebug() << "[Tray] m_updateChecker is:" << (m_updateChecker != nullptr ? "valid" : "null");
        
        if (m_updateChecker) {
            qDebug() << "[Tray] Calling checkForUpdates()...";
            m_updateChecker->checkForUpdates();
            qDebug() << "[Tray] checkForUpdates() called successfully";
        } else {
            qDebug() << "[Tray] ERROR: m_updateChecker is null!";
        }
    });
    
    connect(actQuit, &QAction::triggered, this, &MainWindow::onQuit);
    
    auto checkOnly = [hkF1, hkF2, hkF3](QAction *sel){ hkF1->setChecked(false); hkF2->setChecked(false); hkF3->setChecked(false); sel->setChecked(true); };
    connect(hkF1, &QAction::triggered, this, [this, checkOnly, hkF1]() { if (m_hotkey) m_hotkey->setKeyF1(); checkOnly(hkF1); });
    connect(hkF2, &QAction::triggered, this, [this, checkOnly, hkF2]() { if (m_hotkey) m_hotkey->setKeyF2(); checkOnly(hkF2); });
    connect(hkF3, &QAction::triggered, this, [this, checkOnly, hkF3]() { if (m_hotkey) m_hotkey->setKeyF3(); checkOnly(hkF3); });
    
    m_tray->setContextMenu(menu);
    m_tray->show();
}

void MainWindow::moveToRightEdge() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        
        // 计算右边缘位置：屏幕宽度 - 窗口宽度 - 边距
        int rightEdgeX = screenGeometry.width() - width() - 20; // 20像素边距
        
        // 计算垂直中间位置
        int centerY = (screenGeometry.height() - height()) / 2;
        
        // 移动窗口到右边缘中间位置
        move(rightEdgeX, centerY);
    }
}

void MainWindow::showCloseMenu(const QPoint &pos) {
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    
    QAction *closeAction = menu->addAction("关闭应用");
    closeAction->setIcon(QIcon(":/icons/resources/icons/close.png")); // 如果有关闭图标
    
    connect(closeAction, &QAction::triggered, this, &QWidget::close);
    
    menu->exec(pos);
}

void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    // 当窗口显示时，更新托盘菜单勾选状态
    if (m_toggleShowAction) {
        m_toggleShowAction->setChecked(true);
        qDebug() << "[Tray] Menu updated: window shown";
    }
}

void MainWindow::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    // 当窗口隐藏时，更新托盘菜单勾选状态
    if (m_toggleShowAction) {
        m_toggleShowAction->setChecked(false);
        qDebug() << "[Tray] Menu updated: window hidden";
    }
}

void MainWindow::setupUpdateChecker() {
    qDebug() << "[MainWindow] Setting up update checker...";
    
    // 创建更新检查器
    m_updateChecker = new UpdateChecker(this);
    qDebug() << "[MainWindow] UpdateChecker created:" << (m_updateChecker != nullptr);
    
    // 设置当前版本
    QString currentVersion = QApplication::applicationVersion();
    m_updateChecker->setCurrentVersion(currentVersion);
    qDebug() << "[MainWindow] Current version set to:" << currentVersion;
    
    // 设置更新URL（使用 Gitee 仓库）
    QString updateUrl = "https://gitee.com/yun-meng-song/cap-step-releases/raw/master/latest.json";
    m_updateChecker->setUpdateUrl(updateUrl);
    qDebug() << "[MainWindow] Update URL set to:" << updateUrl;
    
    // 启动自动检查（每24小时检查一次）
    m_updateChecker->startAutoCheck(1);
    qDebug() << "[MainWindow] Auto-check started";
    
    qDebug() << "[MainWindow] Update checker setup completed";
}

bool MainWindow::isAutoStartEnabled() {
    // 检查注册表中是否存在开机启动项
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
                      QSettings::NativeFormat);
    return settings.contains("CapStep");
}

void MainWindow::setAutoStartEnabled(bool enabled) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
                      QSettings::NativeFormat);
    
    if (enabled) {
        // 启用开机启动
        QString appPath = QApplication::applicationFilePath();
        // 使用规范路径格式
        appPath.replace('/', '\\');
        settings.setValue("CapStep", QString("\"%1\"").arg(appPath));
        qDebug() << "[AutoStart] Enabled:" << appPath;
    } else {
        // 禁用开机启动
        settings.remove("CapStep");
        qDebug() << "[AutoStart] Disabled";
    }
}

QString MainWindow::getHistoryFolderPath() {
    // 获取 %APPDATA%\CapStep\ScreenshotHistory\images\ 路径
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString historyPath = appData + "/ScreenshotHistory/images";
    
    // 确保目录存在
    QDir dir;
    if (!dir.exists(historyPath)) {
        dir.mkpath(historyPath);
        qDebug() << "[History] Created history folder:" << historyPath;
    }
    
    return historyPath;
}

void MainWindow::onOpenHistoryFolder() {
    QString historyPath = getHistoryFolderPath();
    
    // 使用系统文件管理器打开文件夹
    bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(historyPath));
    
    if (success) {
        qDebug() << "[History] Opened history folder:" << historyPath;
    } else {
        qDebug() << "[History] Failed to open history folder:" << historyPath;
    }
}