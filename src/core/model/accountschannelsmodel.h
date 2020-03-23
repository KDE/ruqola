/*
   Copyright (c) 2020 Olivier de Gaalon <olivier.jg@gmail.com>

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

#ifndef ACCOUNTSCHANNELSMODEL_H
#define ACCOUNTSCHANNELSMODEL_H

#include <QAbstractItemModel>
#include <QPointer>

#include "libruqolacore_export.h"

class RocketChatAccount;
class RoomFilterProxyModel;

class LIBRUQOLACORE_EXPORT AccountsChannelsModel : public QAbstractItemModel
{
public:
    explicit AccountsChannelsModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QModelIndex modelRoot(QAbstractItemModel *model) const;
    QAbstractItemModel *rootModel(const QModelIndex &root) const;
    void mapModelToIndex(QAbstractItemModel *model, const std::function<QModelIndex()> &root);
    void unproxyModel(QAbstractItemModel *model);

    struct ProxyIndex
    {
        QAbstractItemModel *model;
        std::function<QModelIndex()> root;
    };
    QVector<ProxyIndex> mProxied;
};

#endif // ACCOUNTSCHANNELSMODEL_H
