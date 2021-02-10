/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#endif // USERSFORROOMFILTERPROXYMODEL_H
