/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef ADMINROOMSFILTERPROXYMODEL_H
#define ADMINROOMSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "libruqolacore_export.h"
class AdminRoomsModel;
class LIBRUQOLACORE_EXPORT AdminRoomsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum class FilterRoom {
        None = 1,
        DirectRooms = 2,
        PublicRooms = 4,
        PrivateRooms = 8,
        DiscussionRooms = 16,
    };
    Q_FLAGS(FilterRoom FilterRooms)
    Q_DECLARE_FLAGS(FilterRooms, FilterRoom)

    explicit AdminRoomsFilterProxyModel(AdminRoomsModel *model = nullptr, QObject *parent = nullptr);
    ~AdminRoomsFilterProxyModel() override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;
    void setFilterString(const QString &string);
    void clearFilter();

    void setFilterRooms(AdminRoomsFilterProxyModel::FilterRooms filters);

Q_SIGNALS:
    void hasFullListChanged();
    void loadingInProgressChanged();
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
private:
    AdminRoomsFilterProxyModel::FilterRooms mFilters = FilterRoom::None;
    AdminRoomsModel *const mAdminRoomsModel;
};

#endif // ADMINROOMSFILTERPROXYMODEL_H
