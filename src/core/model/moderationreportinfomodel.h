/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "libruqolacore_export.h"
#include "notificationinfo.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT ModerationReportInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationHistoryRoles {
        AccountName = Qt::UserRole + 1,
        DateTime,
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

    explicit ModerationReportInfoModel(QObject *parent = nullptr);
    ~ModerationReportInfoModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    void insertNotifications(const QVector<NotificationInfo> &infos);

    void addNotification(const NotificationInfo &info);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateMessage(const NotificationInfo &info) const;
    QVector<NotificationInfo> mNotificationInfo;
};
