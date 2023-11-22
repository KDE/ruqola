/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelcompletermodel.h"
#include <QModelIndex>

ChannelCompleterModel::ChannelCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

ChannelCompleterModel::~ChannelCompleterModel() = default;

void ChannelCompleterModel::clear()
{
    if (!mChannels.isEmpty()) {
        beginResetModel();
        mChannels.clear();
        endResetModel();
    }
}

void ChannelCompleterModel::insertChannels(const QVector<ChannelUserCompleter> &channels)
{
    clear();
    if (!channels.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        mChannels = channels;
        endInsertRows();
    }
}

int ChannelCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mChannels.count();
}

QVariant ChannelCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mChannels.count()) {
        return {};
    }
    const ChannelUserCompleter channel = mChannels.at(index.row());
    switch (role) {
    // TODO    case Qt::DecorationRole:
    case Qt::DisplayRole:
    case RoomName:
        return channel.name();
    case ChannelId:
        return channel.identifier();
    }
    return {};
}

#include "moc_channelcompletermodel.cpp"
