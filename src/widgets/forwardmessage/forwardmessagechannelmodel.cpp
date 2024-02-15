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
    // TODO
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
