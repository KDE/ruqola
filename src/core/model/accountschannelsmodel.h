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

#pragma once

#include <QAbstractItemModel>

#include "libruqolacore_export.h"

class RocketChatAccount;

class LIBRUQOLACORE_EXPORT AccountsChannelsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AccountsChannelsModel(QObject *parent = nullptr);
    ~AccountsChannelsModel() override;

    void setFilterString(const QString &filter);
    bool isFiltered() const;

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex findRoomById(const QString &roomId, const RocketChatAccount *acct) const;
    QModelIndex findRoomByName(const QString &roomName, const RocketChatAccount *acct) const;
    QModelIndex findRoomByRole(int role, const QVariant &value, const RocketChatAccount *acct) const;

    QString accountForIndex(const QModelIndex &index) const;

Q_SIGNALS:
    void modelChanged();

private:
    Q_REQUIRED_RESULT QModelIndex modelRoot(QAbstractItemModel *model) const;
    QAbstractItemModel *rootModel(const QModelIndex &root) const;
    void mapModelToIndex(QAbstractItemModel *model, const std::function<QModelIndex()> &root);
    void unproxyModel(QAbstractItemModel *model);

    struct ProxyIndex {
        QAbstractItemModel *model;
        std::function<QModelIndex()> root;
    };
    QVector<ProxyIndex> mProxied;
    bool mFiltered = false;

    enum Visit
    {
        Continue,
        Abort
    };
    template<class T, class Fn>
    void visitProxied(const Fn &fn) const
    {
        for (const auto &proxied: mProxied)
        {
            if (auto model = qobject_cast<T*>(proxied.model))
                if (fn(model, proxied.root()) == Abort)
                    return;
        }
    }
};

