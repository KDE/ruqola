/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractItemModel>

#include "libruqolacore_export.h"
#include "notificationinfo.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT NotificationHistoryModel : public QAbstractItemModel
{
public:
    explicit NotificationHistoryModel(QObject *parent = nullptr);
    ~NotificationHistoryModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

private:
    QVector<NotificationInfo> mNotificationInfo;
};
