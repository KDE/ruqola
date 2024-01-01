/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "notificationinfo.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT NotifierJob : public QObject
{
    Q_OBJECT
public:
    explicit NotifierJob(QObject *parent = nullptr);
    ~NotifierJob() override;

    void start();

    [[nodiscard]] NotificationInfo info() const;
    void setInfo(const NotificationInfo &info);

Q_SIGNALS:
    void switchToAccountAndRoomName(const QString &accountName, const QString &roomName, const QString &channelType);
    void sendReply(const QString &message, const QString &roomId, const QString &tmId);

private:
    NotificationInfo mInfo;
};
