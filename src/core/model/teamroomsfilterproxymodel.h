/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class TeamRoomsModel;
class LIBRUQOLACORE_EXPORT TeamRoomsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TeamRoomsFilterProxyModel(TeamRoomsModel *teamModel = nullptr, QObject *parent = nullptr);
    ~TeamRoomsFilterProxyModel() override;

    void setFilterString(const QString &string);

    [[nodiscard]] bool sortByAutoJoin() const;
    void setSortByAutoJoin(bool sortByAutoJoin);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    TeamRoomsModel *const mTeamRoomsModel;
    bool mSortByAutoJoin = false;
};
