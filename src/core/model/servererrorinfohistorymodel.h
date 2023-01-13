/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "libruqolacore_export.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT ServerErrorInfoHistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ServerErrorInfoHistoryModel(QObject *parent = nullptr);
    ~ServerErrorInfoHistoryModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    //    void insertNotifications(const QVector<NotificationInfo> &infos);

    //    void addNotification(const NotificationInfo &info);

private:
    //    Q_REQUIRED_RESULT QString generateMessage(const NotificationInfo &info) const;
    //    QVector<NotificationInfo> mNotificationInfo;
};
