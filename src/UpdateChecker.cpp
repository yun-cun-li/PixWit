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
 * @file UpdateChecker.cpp
 * @brief 自动更新检查器实现
 * 
 * @author CapStep Team
 * @version 1.0.0
 * @date 2025-10-01
 */

#include "UpdateChecker.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject(parent)
    , m_currentVersion("0.0.5")
    , m_updateUrl("https://gitee.com/yun-meng-song/cap-step-releases/raw/master/latest.json")
    , m_networkManager(nullptr)
    , m_checkTimer(nullptr)
    , m_settings(nullptr)
    , m_autoCheckEnabled(true)
    , m_checkIntervalHours(24)
    , m_isChecking(false)
{
    // 创建网络管理器
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &UpdateChecker::onNetworkReply);

    // 创建定时器
    m_checkTimer = new QTimer(this);
    connect(m_checkTimer, &QTimer::timeout,
            this, &UpdateChecker::onTimerTriggered);

    // 创建设置存储
    m_settings = new QSettings("CapStep", "UpdateChecker", this);

    qDebug() << "[UpdateChecker] Initialized";
}

UpdateChecker::~UpdateChecker()
{
    stopAutoCheck();
    qDebug() << "[UpdateChecker] Destroyed";
}

void UpdateChecker::setCurrentVersion(const QString &version)
{
    m_currentVersion = version;
    qDebug() << "[UpdateChecker] Current version set to:" << version;
}

void UpdateChecker::setUpdateUrl(const QString &url)
{
    m_updateUrl = url;
    qDebug() << "[UpdateChecker] Update URL set to:" << url;
}

void UpdateChecker::startAutoCheck(int checkIntervalHours)
{
    if (!m_autoCheckEnabled) {
        qDebug() << "[UpdateChecker] Auto-check is disabled";
        return;
    }

    m_checkIntervalHours = checkIntervalHours;
    
    // 检查是否需要立即检查
    if (shouldCheckUpdate()) {
        qDebug() << "[UpdateChecker] Starting immediate check";
        QTimer::singleShot(5000, this, &UpdateChecker::checkForUpdates); // 启动5秒后检查
    }

    // 启动定时器（转换为毫秒）
    int intervalMs = checkIntervalHours * 60 * 60 * 1000;
    m_checkTimer->start(intervalMs);
    
    qDebug() << "[UpdateChecker] Auto-check started, interval:" << checkIntervalHours << "hours";
}

void UpdateChecker::stopAutoCheck()
{
    if (m_checkTimer->isActive()) {
        m_checkTimer->stop();
        qDebug() << "[UpdateChecker] Auto-check stopped";
    }
}

void UpdateChecker::checkForUpdates()
{
    if (m_isChecking) {
        qDebug() << "[UpdateChecker] Already checking for updates";
        return;
    }

    if (m_updateUrl.isEmpty()) {
        qDebug() << "[UpdateChecker] Update URL is empty";
        emit checkFailed("Update URL not configured");
        return;
    }

    m_isChecking = true;
    emit checkStarted();

    qDebug() << "[UpdateChecker] Checking for updates from:" << m_updateUrl;
    
    // 创建网络请求
    QUrl url(m_updateUrl);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "CapStep-UpdateChecker/1.0");
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    
    m_networkManager->get(request);
}

void UpdateChecker::setAutoCheckEnabled(bool enabled)
{
    m_autoCheckEnabled = enabled;
    m_settings->setValue("autoCheckEnabled", enabled);
    
    if (enabled) {
        qDebug() << "[UpdateChecker] Auto-check enabled";
    } else {
        qDebug() << "[UpdateChecker] Auto-check disabled";
        stopAutoCheck();
    }
}

void UpdateChecker::onNetworkReply(QNetworkReply *reply)
{
    m_isChecking = false;
    emit checkFinished();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = reply->errorString();
        qWarning() << "[UpdateChecker] Network error:" << errorMsg;
        emit checkFailed(errorMsg);
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    qDebug() << "[UpdateChecker] Received update info:" << data.size() << "bytes";

    if (parseVersionInfo(data)) {
        // 保存检查时间
        saveLastCheckTime();

        // 比较版本
        int comparison = compareVersions(m_latestVersion, m_currentVersion);
        
        if (comparison > 0) {
            qDebug() << "[UpdateChecker] New version available:" << m_latestVersion;
            emit updateAvailable(m_latestVersion, m_downloadOptions, m_releaseNotes);
            
            // 显示更新通知对话框
            showUpdateDialog();
        } else {
            qDebug() << "[UpdateChecker] Already up to date";
            emit noUpdateAvailable();
        }
    } else {
        qWarning() << "[UpdateChecker] Failed to parse version info";
        emit checkFailed("Failed to parse update information");
    }
}

void UpdateChecker::onTimerTriggered()
{
    qDebug() << "[UpdateChecker] Timer triggered, checking for updates";
    checkForUpdates();
}

int UpdateChecker::compareVersions(const QString &version1, const QString &version2)
{
    // 分割版本号
    QStringList parts1 = version1.split('.');
    QStringList parts2 = version2.split('.');
    
    int maxLength = qMax(parts1.length(), parts2.length());
    
    for (int i = 0; i < maxLength; ++i) {
        int num1 = (i < parts1.length()) ? parts1[i].toInt() : 0;
        int num2 = (i < parts2.length()) ? parts2[i].toInt() : 0;
        
        if (num1 > num2) {
            return 1;
        } else if (num1 < num2) {
            return -1;
        }
    }
    
    return 0; // 版本相同
}

bool UpdateChecker::parseVersionInfo(const QByteArray &jsonData)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "[UpdateChecker] JSON parse error:" << parseError.errorString();
        return false;
    }
    
    if (!doc.isObject()) {
        qWarning() << "[UpdateChecker] Invalid JSON format";
        return false;
    }
    
    QJsonObject obj = doc.object();
    
    // 解析版本信息
    m_latestVersion = obj.value("version").toString();
    m_releaseNotes = obj.value("releaseNotes").toString();
    
    // 清空下载选项
    m_downloadOptions.clear();
    
    // 解析下载选项 - 支持新格式
    QString portableUrl = obj.value("downloadUrl").toString();
    QString setupUrl = obj.value("setupUrl").toString();
    
    if (!portableUrl.isEmpty()) {
        DownloadOption portable;
        portable.url = portableUrl;
        portable.name = "便携版 (Portable.zip)";
        portable.description = "绿色版本，无需安装，解压即用";
        portable.type = "portable";
        m_downloadOptions["portable"] = portable;
    }
    
    if (!setupUrl.isEmpty()) {
        DownloadOption setup;
        setup.url = setupUrl;
        setup.name = "安装版 (Setup.exe)";
        setup.description = "推荐使用，自动安装到系统";
        setup.type = "installer";
        m_downloadOptions["setup"] = setup;
    }
    
    // 如果没有找到任何下载选项，使用默认的 downloadUrl
    if (m_downloadOptions.isEmpty() && !portableUrl.isEmpty()) {
        DownloadOption defaultOption;
        defaultOption.url = portableUrl;
        defaultOption.name = "下载更新";
        defaultOption.description = "点击下载最新版本";
        defaultOption.type = "default";
        m_downloadOptions["default"] = defaultOption;
    }
    
    // 验证必需字段
    if (m_latestVersion.isEmpty() || m_downloadOptions.isEmpty()) {
        qWarning() << "[UpdateChecker] Missing required fields in JSON";
        return false;
    }
    
    qDebug() << "[UpdateChecker] Parsed version info:";
    qDebug() << "  - Version:" << m_latestVersion;
    qDebug() << "  - Download options:" << m_downloadOptions.size();
    qDebug() << "  - Release Notes:" << m_releaseNotes;
    
    return true;
}

void UpdateChecker::showUpdateDialog()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("发现新版本");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(QString("发现新版本 %1").arg(m_latestVersion));
    
    // 构建更新内容文本
    QString updateText = QString("当前版本：%1\n\n更新内容：\n%2\n\n请选择下载方式：")
                        .arg(m_currentVersion)
                        .arg(m_releaseNotes);
    
    msgBox.setInformativeText(updateText);
    
    // 根据下载选项数量设置按钮
    if (m_downloadOptions.size() == 1) {
        // 只有一个选项，直接下载
        auto it = m_downloadOptions.begin();
        QString buttonText = QString("下载 %1").arg(it.value().name);
        QPushButton* downloadBtn = msgBox.addButton(buttonText, QMessageBox::AcceptRole);
        msgBox.addButton("取消", QMessageBox::RejectRole);
        msgBox.setDefaultButton(downloadBtn);
        
        if (msgBox.exec() == QMessageBox::AcceptRole) {
            QDesktopServices::openUrl(QUrl(it.value().url));
        }
    } else if (m_downloadOptions.size() > 1) {
        // 多个选项，显示选择对话框
        QPushButton* cancelBtn = msgBox.addButton("取消", QMessageBox::RejectRole);
        
        // 为每个下载选项添加按钮
        QMap<QString, QPushButton*> downloadButtons;
        for (auto it = m_downloadOptions.begin(); it != m_downloadOptions.end(); ++it) {
            QString buttonText = QString("下载 %1").arg(it.value().name);
            QPushButton* button = msgBox.addButton(buttonText, QMessageBox::ActionRole);
            downloadButtons[it.key()] = button;
        }
        
        msgBox.exec();
        
        // 检查哪个按钮被点击
        QAbstractButton* clickedButton = msgBox.clickedButton();
        if (clickedButton && clickedButton != cancelBtn) {
            for (auto it = downloadButtons.begin(); it != downloadButtons.end(); ++it) {
                if (it.value() == clickedButton) {
                    QString selectedKey = it.key();
                    if (m_downloadOptions.contains(selectedKey)) {
                        QDesktopServices::openUrl(QUrl(m_downloadOptions[selectedKey].url));
                    }
                    break;
                }
            }
        }
    }
}

void UpdateChecker::saveLastCheckTime()
{
    QDateTime now = QDateTime::currentDateTime();
    m_settings->setValue("lastCheckTime", now);
    qDebug() << "[UpdateChecker] Saved last check time:" << now.toString();
}

QDateTime UpdateChecker::loadLastCheckTime()
{
    QDateTime lastCheck = m_settings->value("lastCheckTime").toDateTime();
    qDebug() << "[UpdateChecker] Loaded last check time:" << lastCheck.toString();
    return lastCheck;
}

bool UpdateChecker::shouldCheckUpdate()
{
    QDateTime lastCheck = loadLastCheckTime();
    
    if (!lastCheck.isValid()) {
        qDebug() << "[UpdateChecker] No previous check time, should check";
        return true;
    }
    
    QDateTime now = QDateTime::currentDateTime();
    qint64 hoursSinceLastCheck = lastCheck.secsTo(now) / 3600;
    
    bool shouldCheck = hoursSinceLastCheck >= m_checkIntervalHours;
    
    qDebug() << "[UpdateChecker] Hours since last check:" << hoursSinceLastCheck
             << "/ Required:" << m_checkIntervalHours
             << "/ Should check:" << shouldCheck;
    
    return shouldCheck;
}

