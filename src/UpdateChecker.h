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
 * @file UpdateChecker.h
 * @brief 自动更新检查器类
 * 
 * 提供自动检查软件更新的功能
 * 
 * @author CapStep Team
 * @version 1.0.0
 * @date 2025-10-01
 */

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QMap>

/**
 * @struct DownloadOption
 * @brief 下载选项结构
 */
struct DownloadOption {
    QString url;        ///< 下载地址
    QString name;       ///< 显示名称
    QString description; ///< 描述
    QString size;       ///< 文件大小
    QString type;       ///< 类型 (installer/portable)
};

/**
 * @class UpdateChecker
 * @brief 自动更新检查器类
 * 
 * 提供以下功能：
 * - 定期检查软件更新（默认每天一次）
 * - 从远程服务器获取最新版本信息
 * - 版本号比较
 * - 通知用户有新版本可用
 * - 支持多种下载选项（安装版/便携版）
 */
class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    explicit UpdateChecker(QObject *parent = nullptr);
    ~UpdateChecker();

    /**
     * @brief 设置当前版本号
     * @param version 当前版本号 (例如: "1.0.1")
     */
    void setCurrentVersion(const QString &version);

    /**
     * @brief 获取当前版本号
     * @return 当前版本号
     */
    QString currentVersion() const { return m_currentVersion; }

    /**
     * @brief 设置更新检查URL
     * @param url 更新信息的URL
     */
    void setUpdateUrl(const QString &url);

    /**
     * @brief 启动自动更新检查
     * @param checkIntervalHours 检查间隔（小时），默认24小时
     */
    void startAutoCheck(int checkIntervalHours = 24);

    /**
     * @brief 停止自动更新检查
     */
    void stopAutoCheck();

    /**
     * @brief 手动检查更新
     */
    void checkForUpdates();

    /**
     * @brief 是否启用自动检查
     * @param enabled true启用，false禁用
     */
    void setAutoCheckEnabled(bool enabled);

    /**
     * @brief 获取是否启用自动检查
     * @return true启用，false禁用
     */
    bool isAutoCheckEnabled() const { return m_autoCheckEnabled; }

signals:
    /**
     * @brief 发现新版本信号
     * @param version 新版本号
     * @param downloadOptions 下载选项列表
     * @param releaseNotes 更新说明
     */
    void updateAvailable(const QString &version, const QMap<QString, DownloadOption> &downloadOptions, const QString &releaseNotes);

    /**
     * @brief 已是最新版本信号
     */
    void noUpdateAvailable();

    /**
     * @brief 检查更新失败信号
     * @param errorMessage 错误信息
     */
    void checkFailed(const QString &errorMessage);

    /**
     * @brief 检查开始信号
     */
    void checkStarted();

    /**
     * @brief 检查完成信号
     */
    void checkFinished();

private slots:
    /**
     * @brief 处理网络回复
     * @param reply 网络回复对象
     */
    void onNetworkReply(QNetworkReply *reply);

    /**
     * @brief 定时器触发时检查更新
     */
    void onTimerTriggered();

private:
    /**
     * @brief 比较版本号
     * @param version1 版本1
     * @param version2 版本2
     * @return >0 version1更新, =0 相同, <0 version2更新
     */
    int compareVersions(const QString &version1, const QString &version2);

    /**
     * @brief 解析版本信息JSON
     * @param jsonData JSON数据
     * @return true成功，false失败
     */
    bool parseVersionInfo(const QByteArray &jsonData);

    /**
     * @brief 保存最后检查时间
     */
    void saveLastCheckTime();

    /**
     * @brief 加载最后检查时间
     * @return 最后检查时间
     */
    QDateTime loadLastCheckTime();

    /**
     * @brief 是否需要检查更新
     * @return true需要，false不需要
     */
    bool shouldCheckUpdate();

    /**
     * @brief 显示更新对话框
     */
    void showUpdateDialog();

private:
    QString m_currentVersion;                   ///< 当前版本号
    QString m_updateUrl;                        ///< 更新检查URL
    QNetworkAccessManager *m_networkManager;    ///< 网络管理器
    QTimer *m_checkTimer;                       ///< 定时检查定时器
    QSettings *m_settings;                      ///< 设置存储
    bool m_autoCheckEnabled;                    ///< 是否启用自动检查
    int m_checkIntervalHours;                   ///< 检查间隔（小时）
    bool m_isChecking;                          ///< 是否正在检查

    // 临时存储解析的更新信息
    QString m_latestVersion;                    ///< 最新版本号
    QMap<QString, DownloadOption> m_downloadOptions; ///< 下载选项列表
    QString m_releaseNotes;                     ///< 更新说明
};

#endif // UPDATECHECKER_H

