/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include <KStatusNotifierItem>
#include <QMap>

class UnityServiceManager;
class LIBRUQOLACORE_EXPORT Notification : public KStatusNotifierItem
{
    Q_OBJECT
public:
    explicit Notification(QObject *parent = nullptr);
    ~Notification() override;

    void roomNeedAttention();
    void updateNotification(bool hasAlert, int unreadNumber, const QString &account);
    void clearNotification(const QString &account);
#ifdef UNITY_SUPPORT
    UnityServiceManager *unityServiceManager();
#endif

Q_SIGNALS:
    void alert();

private:
    Q_DISABLE_COPY(Notification)
    struct TrayInfo {
        TrayInfo(int unread, bool alert)
            : unreadMessage(unread)
            , hasAlert(alert)
        {
        }

        [[nodiscard]] bool hasNotification() const
        {
            return (unreadMessage != 0) || hasAlert;
        }

        int unreadMessage = 0;
        bool hasAlert = false;
    };
    /**
     * @brief Creates tray icon consisting of actions
     */
    LIBRUQOLACORE_NO_EXPORT void createTrayIcon();
    LIBRUQOLACORE_NO_EXPORT void createToolTip();
    LIBRUQOLACORE_NO_EXPORT void updateUnityService(int unreadMessage);

    QMap<QString, TrayInfo> mListTrayIcon;
#ifdef UNITY_SUPPORT
    UnityServiceManager *mUnityServiceManager = nullptr;
#endif
};
