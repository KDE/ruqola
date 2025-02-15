/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT UsersForRoomFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum class FilterUserType : uint8_t {
        All,
        Online,
        Offline,
        Away,
        Busy,
        Owners,
    };
    Q_ENUM(FilterUserType);

    explicit UsersForRoomFilterProxyModel(QObject *parent = nullptr);
    ~UsersForRoomFilterProxyModel() override;
    void setFilterString(const QString &string);

    [[nodiscard]] bool hasFullList() const;
    [[nodiscard]] bool loadMoreUsersInProgress() const;

    [[nodiscard]] int total() const;
    [[nodiscard]] int numberOfUsers() const;

    void setStatusType(UsersForRoomFilterProxyModel::FilterUserType statusType);

    void clearFilter();
Q_SIGNALS:
    void hasFullListChanged();
    void loadingInProgressChanged();

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    UsersForRoomFilterProxyModel::FilterUserType mStatusType = UsersForRoomFilterProxyModel::FilterUserType::All;
};
