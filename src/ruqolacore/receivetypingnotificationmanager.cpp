/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "receivetypingnotificationmanager.h"
#include <KLocalizedString>

ReceiveTypingNotificationManager::ReceiveTypingNotificationManager(QObject *parent)
    : QObject(parent)
{
}

ReceiveTypingNotificationManager::~ReceiveTypingNotificationManager()
{
}

void ReceiveTypingNotificationManager::clearTypingNotification()
{
    Q_EMIT clearNotification();
}

void ReceiveTypingNotificationManager::insertTypingNotification(const QString &roomId, const QString &userName, bool onTyping)
{
    if (mMapTypingNotifications.contains(roomId)) {
        QStringList lst = mMapTypingNotifications.value(roomId);
        if (onTyping) {
            if (!lst.contains(userName)) {
                lst.append(userName);
                mMapTypingNotifications[roomId] = lst;
                Q_EMIT notificationChanged(roomId, generateNotification(lst));
            }
        } else {
            const int removedUserCount = lst.removeAll(userName);
            if (removedUserCount > 0) {
                if (lst.isEmpty()) {
                    //remove roomId
                    mMapTypingNotifications.remove(roomId);
                } else {
                    mMapTypingNotifications[roomId] = lst;
                }
                Q_EMIT notificationChanged(roomId, generateNotification(lst));
            }
        }
    } else {
        if (onTyping) {
            mMapTypingNotifications.insert(roomId, {userName});
            Q_EMIT notificationChanged(roomId, generateNotification({userName}));
        }
    }
}

QString ReceiveTypingNotificationManager::generateNotification(const QStringList &userNames) const
{
    if (userNames.isEmpty()) {
        return QString();
    } else if (userNames.count() == 1) {
        return i18n("%1 is typing...", userNames[0]);
    } else {
        QString notificationStr;
        for (int i = 0; i < userNames.count(); ++i) {
            const QString user = userNames.at(i);
            if (i == 0) {
                notificationStr = user;
            } else if (i < userNames.count() - 1) {
                notificationStr = i18n("%1, %2", notificationStr, user);
            } else {
                notificationStr = i18n("%1 and %2", notificationStr, user);
            }
        }
        return i18n("%1 are typing...", notificationStr);
    }
}

QString ReceiveTypingNotificationManager::typingNotification(const QString &roomId) const
{
    return generateNotification(mMapTypingNotifications.value(roomId));
}
