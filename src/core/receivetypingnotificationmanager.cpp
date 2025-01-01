/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "receivetypingnotificationmanager.h"
#include <KLocalizedString>

ReceiveTypingNotificationManager::ReceiveTypingNotificationManager(QObject *parent)
    : QObject(parent)
{
}

ReceiveTypingNotificationManager::~ReceiveTypingNotificationManager() = default;

void ReceiveTypingNotificationManager::clearTypingNotification()
{
    Q_EMIT clearNotification();
}

void ReceiveTypingNotificationManager::insertTypingNotification(const QByteArray &roomId, const QString &userName, bool onTyping)
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
                    // remove roomId
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
        return {};
    }
    if (userNames.count() == 1) {
        return i18nc("WARNING!!!!!!: PLEASE We need a space between after </strong>", "<strong>%1</strong> is typing...", userNames[0]);
    }

    QString notificationStr;
    for (int i = 0; i < userNames.count(); ++i) {
        const QString &user = userNames.at(i);
        if (i == 0) {
            notificationStr = user;
        } else if (i < userNames.count() - 1) {
            notificationStr = i18n("%1, %2", notificationStr, user);
        } else {
            notificationStr = i18n("%1 and %2", notificationStr, user);
        }
    }
    return i18nc("WARNING!!!!!!: PLEASE We need a space between after </strong>", "<strong>%1</strong> are typing...", notificationStr);
}

QString ReceiveTypingNotificationManager::typingNotification(const QByteArray &roomId) const
{
    return generateNotification(mMapTypingNotifications.value(roomId));
}

#include "moc_receivetypingnotificationmanager.cpp"
