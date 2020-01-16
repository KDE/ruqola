/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef RECEIVETYPINGNOTIFICATIONMANAGER_H
#define RECEIVETYPINGNOTIFICATIONMANAGER_H

#include <QObject>
#include "libruqola_private_export.h"
#include <QMap>
class LIBRUQOLACORE_TESTS_EXPORT ReceiveTypingNotificationManager : public QObject
{
    Q_OBJECT
public:
    explicit ReceiveTypingNotificationManager(QObject *parent = nullptr);
    ~ReceiveTypingNotificationManager();

    void insertTypingNotification(const QString &roomId, const QString &userName, bool onTyping);

    Q_REQUIRED_RESULT QString typingNotification(const QString &roomId) const;

    void clearTypingNotification();
Q_SIGNALS:
    void notificationChanged(const QString &roomId, const QString &notificationStr);
    void clearNotification();

private:
    Q_DISABLE_COPY(ReceiveTypingNotificationManager)
    QString generateNotification(const QStringList &userNames) const;
    QMap<QString, QStringList> mMapTypingNotifications;
};

#endif // RECEIVETYPINGNOTIFICATIONMANAGER_H
