/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

        Q_REQUIRED_RESULT bool hasNotification() const
        {
            return (unreadMessage != 0) || hasAlert;
        }

        int unreadMessage = 0;
        bool hasAlert = false;
    };
    /**
     * @brief Creates tray icon consisting of actions
     */
    void createTrayIcon();
    void createToolTip();
    void updateUnityService(int unreadMessage);

    QMap<QString, TrayInfo> mListTrayIcon;
#ifdef UNITY_SUPPORT
    UnityServiceManager *mUnityServiceManager = nullptr;
#endif
};

