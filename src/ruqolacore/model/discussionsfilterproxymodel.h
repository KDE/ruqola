/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#ifndef DISCUSSIONSFILTERPROXYMODEL_H
#define DISCUSSIONSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "libruqola_private_export.h"
class DiscussionsModel;
class LIBRUQOLACORE_TESTS_EXPORT DiscussionsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasFullList READ hasFullList NOTIFY hasFullListChanged)
public:
    explicit DiscussionsFilterProxyModel(DiscussionsModel *model = nullptr, QObject *parent = nullptr);
    ~DiscussionsFilterProxyModel() override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void setFilterString(const QString &string);
    Q_INVOKABLE void clearFilter();
    Q_INVOKABLE Q_REQUIRED_RESULT int total() const;

    Q_REQUIRED_RESULT bool hasFullList() const;
Q_SIGNALS:
    void hasFullListChanged();
private:
    DiscussionsModel *mDiscussionsModel = nullptr;
};

#endif // DISCUSSIONSFILTERPROXYMODEL_H
