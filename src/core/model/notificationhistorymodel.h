/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "libruqolacore_export.h"
#include "notificationinfo.h"
#include <QList>

class LIBRUQOLACORE_EXPORT NotificationHistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationHistoryRoles {
        AccountName = Qt::UserRole + 1,
        Time,
        Date,
        MessageStr,
        RoomId,
        RoomName,
        ChannelType,
        Pixmap,
        SenderName,
        SenderUserName,
        MessageId,
    };
    Q_ENUM(NotificationHistoryRoles)

    explicit NotificationHistoryModel(QObject *parent = nullptr);
    ~NotificationHistoryModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    void insertNotifications(const QList<NotificationInfo> &infos);

    void addNotification(const NotificationInfo &info);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateMessage(const NotificationInfo &info) const;
    QList<NotificationInfo> mNotificationInfo;
};
