/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#ifndef ROOMFILTERPROXYMODEL_H
#define ROOMFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT RoomFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RoomFilterProxyModel(QObject *parent = nullptr);
    ~RoomFilterProxyModel() override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setFilterString(const QString &string);
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
private:
    Q_DISABLE_COPY(RoomFilterProxyModel)
};

#endif // ROOMFILTERPROXYMODEL_H
