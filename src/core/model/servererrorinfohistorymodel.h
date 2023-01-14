/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "libruqolacore_export.h"
#include "servererrorinfo.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT ServerErrorInfoHistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ServerErrorInfoHistoryRoles {
        AccountName = Qt::UserRole + 1,
        MessageStr,
    };
    Q_ENUM(ServerErrorInfoHistoryRoles)
    explicit ServerErrorInfoHistoryModel(QObject *parent = nullptr);
    ~ServerErrorInfoHistoryModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    void insertServerErrorInfos(const QVector<ServerErrorInfo> &infos);

    void addServerErrorInfo(const ServerErrorInfo &info);

private:
    QVector<ServerErrorInfo> mServerErrorInfo;
};
