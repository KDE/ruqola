/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QMap>
#include <QObject>
class LIBRUQOLACORE_EXPORT ReceiveTypingNotificationManager : public QObject
{
    Q_OBJECT
public:
    explicit ReceiveTypingNotificationManager(QObject *parent = nullptr);
    ~ReceiveTypingNotificationManager() override;

    void insertTypingNotification(const QString &roomId, const QString &userName, bool onTyping);

    Q_REQUIRED_RESULT QString typingNotification(const QString &roomId) const;

    void clearTypingNotification();
Q_SIGNALS:
    void notificationChanged(const QString &roomId, const QString &notificationStr);
    void clearNotification();

private:
    Q_DISABLE_COPY(ReceiveTypingNotificationManager)
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QString generateNotification(const QStringList &userNames) const;
    QMap<QString, QStringList> mMapTypingNotifications;
};
