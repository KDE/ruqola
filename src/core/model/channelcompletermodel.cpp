/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "channelcompletermodel.h"
#include "ruqola_debug.h"
#include <QIcon>
#include <QModelIndex>

ChannelCompleterModel::ChannelCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

ChannelCompleterModel::~ChannelCompleterModel()
{
}

void ChannelCompleterModel::clear()
{
    if (!mChannels.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        mChannels.clear();
        endRemoveRows();
    }
}

void ChannelCompleterModel::insertChannels(const QVector<Channel> &channels)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mChannels.count() - 1);
        mChannels.clear();
        endRemoveRows();
    }
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
        return QVariant();
    }
#if 0
    const User user = mChannels.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return displayUserName(user);
    case UserName:
        return user.userName();
    case UserId:
        return user.userId();
    case Qt::DecorationRole:
        return QIcon::fromTheme(user.iconFromStatus());
    case UserIconStatus:
        return user.iconFromStatus();
    }
#endif
    return {};
}
