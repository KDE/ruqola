/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "joinedchannelmodel.h"

JoinedChannelModel::JoinedChannelModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

JoinedChannelModel::~JoinedChannelModel() = default;

int JoinedChannelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRooms.count();
}

QVariant JoinedChannelModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRooms.count()) {
        return {};
    }
    const ChannelUserCompleter &channel = mRooms.at(index.row());
    switch (role) {
    case Name:
        return channel.name();
    case ChannelId:
        return channel.identifier();
    case AvatarInfo:
        return QVariant::fromValue(channel.avatarInfo());
    }
    return {};
}

void JoinedChannelModel::setRooms(const QList<ChannelUserCompleter> &rooms)
{
    clear();
    if (!rooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, rooms.count() - 1);
        mRooms = rooms;
        endInsertRows();
    }
}

void JoinedChannelModel::clear()
{
    if (!mRooms.isEmpty()) {
        beginResetModel();
        mRooms.clear();
        endResetModel();
    }
}

#include "moc_joinedchannelmodel.cpp"
