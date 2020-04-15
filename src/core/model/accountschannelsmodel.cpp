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

#include "accountschannelsmodel.h"

#include "accountmanager.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "roomfilterproxymodel.h"
#include "rocketchataccountmodel.h"
#include "rocketchataccountfilterproxymodel.h"

AccountsChannelsModel::AccountsChannelsModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    const auto src = Ruqola::self()->accountManager()->rocketChatAccountModel();
    const auto acctsProxy = Ruqola::self()->accountManager()->rocketChatAccountProxyModel();

    auto roomsModel = [src, acctsProxy](int i) {
                          const auto acctIndex = acctsProxy->mapToSource(acctsProxy->index(i, 0)).row();
                          return src->account(acctIndex)->roomFilterProxyModel();
                      };

    auto mapRoomsModel = [roomsModel, acctsProxy, this](int roomsModelIndex) {
                             auto rooms = roomsModel(roomsModelIndex);
                             mapModelToIndex(rooms, [roomsModel, acctsProxy, rooms, this] {
            for (int i = 0, count = acctsProxy->rowCount(); i < count; ++i) {
                if (roomsModel(i) == rooms) {
                    return index(i, 0);
                }
            }
            return QModelIndex();
        });
                         };

    connect(acctsProxy, &QAbstractItemModel::rowsInserted, this,
            [mapRoomsModel](const QModelIndex &, int first, int last) {
        for (int i = first; i <= last; ++i) {
            mapRoomsModel(i);
        }
    });

    connect(acctsProxy, &QAbstractItemModel::rowsAboutToBeRemoved, this,
            [roomsModel, this](const QModelIndex &, int first, int last) {
        for (int i = first; i <= last; ++i) {
            unproxyModel(roomsModel(i));
        }
    });

    connect(acctsProxy, &QAbstractItemModel::modelReset, this,
            [mapRoomsModel, acctsProxy, this]() {
        while (!mProxied.isEmpty()) {
            unproxyModel(mProxied.begin()->model);
        }
        for (int i = 0, count = acctsProxy->rowCount(); i < count; ++i) {
            mapRoomsModel(i);
        }
    });

    mapModelToIndex(acctsProxy, [] {
        return QModelIndex();
    });
    for (int i = 0, count = acctsProxy->rowCount(); i < count; ++i) {
        mapRoomsModel(i);
    }
}

QModelIndex AccountsChannelsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (auto model = rootModel(parent)) {
        return createIndex(row, column, model);
    }
    return {};
}

QModelIndex AccountsChannelsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return {};
    }

    if (auto model = static_cast<QAbstractItemModel *>(child.internalPointer())) {
        return modelRoot(model);
    }

    return {};
}

int AccountsChannelsModel::rowCount(const QModelIndex &parent) const
{
    if (auto model = rootModel(parent)) {
        return model->rowCount();
    }
    return 0;
}

int AccountsChannelsModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant AccountsChannelsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    const auto model = static_cast<QAbstractItemModel *>(index.internalPointer());
    if (!model) {
        return {};
    }

    return model->index(index.row(), index.column()).data(role);
}

QModelIndex AccountsChannelsModel::modelRoot(QAbstractItemModel *model) const
{
    const auto find = [model](const ProxyIndex &i){
                          return i.model == model;
                      };
    const auto it = std::find_if(mProxied.begin(), mProxied.end(), find);
    return (it == mProxied.end()) ? QModelIndex() : it->root();
}

QAbstractItemModel *AccountsChannelsModel::rootModel(const QModelIndex &root) const
{
    const auto find = [&root](const ProxyIndex &i){
                          return i.root() == root;
                      };
    const auto it = std::find_if(mProxied.begin(), mProxied.end(), find);
    return (it == mProxied.end()) ? nullptr : it->model;
}

void AccountsChannelsModel::mapModelToIndex(QAbstractItemModel *model, const std::function<QModelIndex()> &root)
{
    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, this,
            [this, model](const QModelIndex &parent, int first, int last) {
        Q_ASSERT(!parent.isValid());
        beginInsertRows(modelRoot(model), first, last);
    });
    connect(model, &QAbstractItemModel::rowsInserted, this, &AccountsChannelsModel::endInsertRows);

    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, this,
            [this, model](const QModelIndex &parent, int first, int last) {
        Q_ASSERT(!parent.isValid());
        beginRemoveRows(modelRoot(model), first, last);
    });
    connect(model, &QAbstractItemModel::rowsRemoved, this, &AccountsChannelsModel::endRemoveRows);

    connect(model, &QAbstractItemModel::rowsAboutToBeMoved, this,
            [this, model](const QModelIndex &src, int sf, int sl, const QModelIndex &dst, int df) {
        Q_ASSERT(!src.isValid() && !dst.isValid());
        const auto idx = modelRoot(model);
        beginMoveRows(idx, sf, sl, idx, df);
    });
    connect(model, &QAbstractItemModel::rowsMoved, this, &AccountsChannelsModel::endMoveRows);

    connect(model, &QAbstractItemModel::modelAboutToBeReset, this, &AccountsChannelsModel::beginResetModel);
    connect(model, &QAbstractItemModel::modelReset, this, &AccountsChannelsModel::endResetModel);

    connect(model, &QAbstractItemModel::layoutAboutToBeChanged, this, &AccountsChannelsModel::layoutAboutToBeChanged);
    connect(model, &QAbstractItemModel::layoutChanged, this, &AccountsChannelsModel::layoutChanged);

    connect(model, &QAbstractItemModel::dataChanged, this,
            [this, model](const QModelIndex &tl, const QModelIndex &br) {
        const auto parent = modelRoot(model);
        Q_EMIT dataChanged(index(tl.row(), tl.column(), parent), index(br.row(), br.column(), parent));
    });

    mProxied.append({model, root});
}

void AccountsChannelsModel::unproxyModel(QAbstractItemModel *model)
{
    const auto find = [model](const ProxyIndex &i){
                          return i.model == model;
                      };
    const auto it = std::find_if(mProxied.begin(), mProxied.end(), find);
    if (it != mProxied.end()) {
        model->disconnect(this);
        mProxied.erase(it);
    }
}
