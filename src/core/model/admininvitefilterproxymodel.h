/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class AdminInviteModel;
class LIBRUQOLACORE_EXPORT AdminInviteFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AdminInviteFilterProxyModel(AdminInviteModel *model = nullptr, QObject *parent = nullptr);
    ~AdminInviteFilterProxyModel() override;

    void setFilterString(const QString &string);
    void clearFilter();
Q_SIGNALS:
    void hasFullListChanged();
    void loadingInProgressChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    AdminInviteModel *const mAdminInviteModel;
};
