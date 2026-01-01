/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

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

    void insertTypingNotification(const QByteArray &roomId, const QString &userName, bool onTyping);

    [[nodiscard]] QString typingNotification(const QByteArray &roomId) const;

    void clearTypingNotification();
Q_SIGNALS:
    void notificationChanged(const QByteArray &roomId, const QString &notificationStr);
    void clearNotification();

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateNotification(const QStringList &userNames) const;
    QMap<QByteArray, QStringList> mMapTypingNotifications;
};
