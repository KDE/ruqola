/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT UsersForRoomFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit UsersForRoomFilterProxyModel(QObject *parent = nullptr);
    ~UsersForRoomFilterProxyModel() override;
    void setFilterString(const QString &string);

    Q_REQUIRED_RESULT bool hasFullList() const;
    Q_REQUIRED_RESULT bool loadMoreUsersInProgress() const;

    Q_REQUIRED_RESULT int total() const;
    Q_REQUIRED_RESULT int numberOfUsers() const;

    void setStatusType(const QString &statusType);

    void clearFilter();
Q_SIGNALS:
    void hasFullListChanged();
    void loadingInProgressChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    QString mStatusType;
};
