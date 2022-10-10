/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "ownuser/ownuserpreferences.h"
#include <QSortFilterProxyModel>

/**
 * This proxy model sits on top of RoomModel and implements
 * - filtering (on the RoomOpen role)
 * - sorting (by RoomOrder and by RoomName)
 */
class LIBRUQOLACORE_EXPORT RoomFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RoomFilterProxyModel(QObject *parent = nullptr);
    ~RoomFilterProxyModel() override;

    void setFilterString(const QString &string);

    void setSortOrder(OwnUserPreferences::RoomListSortOrder sortOrder);

protected:
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    Q_REQUIRED_RESULT bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Q_DISABLE_COPY(RoomFilterProxyModel)
    QString mFilterString;
    OwnUserPreferences::RoomListSortOrder mSortOrder = OwnUserPreferences::RoomListSortOrder::ByLastMessage;
};
