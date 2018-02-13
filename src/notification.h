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

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "libruqola_private_export.h"
#include <KStatusNotifierItem>
#include <QMap>

class LIBRUQOLACORE_TESTS_EXPORT Notification : public KStatusNotifierItem
{
    Q_OBJECT
public:
    explicit Notification(QObject *parent = nullptr);
    ~Notification() = default;

    void updateNotification(bool hasAlert, int unreadNumber, const QString &account);
    void clearNotification(const QString &account);
private:
    struct TrayInfo {
        TrayInfo(int unread, bool alert)
            : unreadMessage(unread)
            , hasAlert(alert)
        {
        }

        int unreadMessage = 0;
        bool hasAlert = false;
        bool hasNotification() const
        {
            return (unreadMessage != 0) || hasAlert;
        }
    };
    /**
    * @brief Creates tray icon consisting of actions
    */
    void createTrayIcon();
    void createToolTip();

    QMap<QString, TrayInfo> mListTrayIcon;
};

#endif // NOTIFICATION_H
