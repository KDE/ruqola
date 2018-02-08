/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "searchchannelroommodel.h"
#include "ruqola_debug.h"

SearchChannelRoomModel::SearchChannelRoomModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

SearchChannelRoomModel::~SearchChannelRoomModel()
{

}

void SearchChannelRoomModel::setChannels(const QVector<Channel> &channels)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mChannel.count() - 1);
        mChannel.clear();
        endRemoveRows();
    }
    if (!channels.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        mChannel = channels;
        endInsertRows();
    }
}

int SearchChannelRoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mChannel.count();
}

QVariant SearchChannelRoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mChannel.count()) {
        return {};
    }
    const Channel channel = mChannel.at(index.row());
    switch (role) {
    //TODO
    default:
        qCWarning(RUQOLA_LOG) << "Unknown SearchChannelRoomModel roles: " << role;
    }

    return {};
}

QHash<int, QByteArray> SearchChannelRoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    //TODO
    return roles;
}
