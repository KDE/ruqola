/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef USERSFORROOMFILTERPROXYMODEL_H
#define USERSFORROOMFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT UsersForRoomFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasFullList READ hasFullList NOTIFY hasFullListChanged)
public:
    explicit UsersForRoomFilterProxyModel(QObject *parent = nullptr);
    ~UsersForRoomFilterProxyModel() override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setFilterString(const QString &string);

    Q_REQUIRED_RESULT bool hasFullList() const;
Q_SIGNALS:
    void hasFullListChanged();

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // USERSFORROOMFILTERPROXYMODEL_H
