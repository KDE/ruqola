/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagechannelmodel.h"

ForwardMessageChannelModel::ForwardMessageChannelModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ForwardMessageChannelModel::~ForwardMessageChannelModel() = default;

int ForwardMessageChannelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRooms.count();
}

QVariant ForwardMessageChannelModel::data(const QModelIndex &index, int role) const
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

void ForwardMessageChannelModel::setRooms(const QVector<ChannelUserCompleter> &rooms)
{
    clear();
    if (!rooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, rooms.count() - 1);
        mRooms = rooms;
        endInsertRows();
    }
}

void ForwardMessageChannelModel::clear()
{
    if (!mRooms.isEmpty()) {
        beginResetModel();
        mRooms.clear();
        endResetModel();
    }
}

#include "moc_forwardmessagechannelmodel.cpp"
