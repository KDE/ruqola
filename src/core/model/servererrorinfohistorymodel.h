/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "libruqolacore_export.h"
#include "servererrorinfo.h"
#include <QList>

class LIBRUQOLACORE_EXPORT ServerErrorInfoHistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ServerErrorInfoHistoryRoles {
        AccountName = Qt::UserRole + 1,
        MessageStr,
        Identifier,
        DateTime,
        DateTimeStr,
    };
    Q_ENUM(ServerErrorInfoHistoryRoles)
    explicit ServerErrorInfoHistoryModel(QObject *parent = nullptr);
    ~ServerErrorInfoHistoryModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    void insertServerErrorInfos(const QList<ServerErrorInfo> &infos);

    void addServerErrorInfo(const ServerErrorInfo &info);

private:
    QList<ServerErrorInfo> mServerErrorInfo;
};
