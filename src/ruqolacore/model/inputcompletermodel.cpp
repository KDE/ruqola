/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "inputcompletermodel.h"
#include "ruqola_debug.h"

#include <QJsonArray>

#include <QIcon>
#include <QJsonObject>
#include <QModelIndex>

InputCompleterModel::InputCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

InputCompleterModel::~InputCompleterModel()
{
}

void InputCompleterModel::setChannels(const QVector<Channel> &channels)
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

void InputCompleterModel::parseChannels(const QJsonObject &obj)
{
    QVector<Channel> channelList;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        //Verify that it's valid
        channelList.append(channel);
    }
    const QJsonArray users = obj.value(QLatin1String("users")).toArray();
    for (int i = 0; i < users.size(); i++) {
        const QJsonObject o = users.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::PrivateChannel);
        //Verify that it's valid
        channelList.append(channel);
    }
    setChannels(channelList);
}

void InputCompleterModel::clear()
{
    if (!mChannel.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, mChannel.count() - 1);
        mChannel.clear();
        endRemoveRows();
    }
}

int InputCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mChannel.count();
}

QVariant InputCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mChannel.count()) {
        return {};
    }
    const Channel channel = mChannel.at(index.row());
    switch (role) {
    case InputCompleterModel::DisplayName:
        return channelName(channel);
    case InputCompleterModel::CompleterName:
        return completerName(channel);
    case InputCompleterModel::IconName:
        return channelIconName(channel);
    case InputCompleterModel::ChannelType:
        return channel.type();
    }
    return {};
}

QString InputCompleterModel::completerName(const Channel &channel) const
{
    //Specific channelId for opening room
    //For private channel we need to use username for channel we need roomId
    switch (channel.type()) {
    case Channel::ChannelType::PrivateChannel:
        return channel.userName();
    case Channel::ChannelType::Room:
        return channel.roomName();
    case Channel::ChannelType::Unknown:
        qCWarning(RUQOLA_LOG) << "Unknown channel type!";
        return {};
    }

    return {};
}

QString InputCompleterModel::channelName(const Channel &channel) const
{
    switch (channel.type()) {
    case Channel::ChannelType::PrivateChannel:
    {
        QString text = channel.userName();
        const QString name = channel.name();
        if (!name.isEmpty()) {
            text += QLatin1String(" (") + channel.name() + QLatin1Char(')');
        }
        return text;
    }
    case Channel::ChannelType::Room:
        return channel.roomName();
    case Channel::ChannelType::Unknown:
        qCWarning(RUQOLA_LOG) << "Unknown channel type!";
        return {};
    }
    return {};
}

QIcon InputCompleterModel::channelIconName(const Channel &channel) const
{
    switch (channel.type()) {
    case Channel::ChannelType::PrivateChannel:
        return QIcon::fromTheme(channel.iconFromStatus());
    case Channel::ChannelType::Room:
        if (channel.roomType() == QLatin1Char('c')) {
            return QIcon::fromTheme(QStringLiteral("irc-channel-inactive"));
        } else if (channel.roomType() == QLatin1Char('p')) {
            return QIcon::fromTheme(QStringLiteral("lock"));
        }
        qCWarning(RUQOLA_LOG) << "Unknown room type!" << channel.roomType();
        return {};
    case Channel::ChannelType::Unknown:
        qCWarning(RUQOLA_LOG) << "Unknown channel type!";
        return {};
    }
    return {};
}

QHash<int, QByteArray> InputCompleterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[InputCompleterModel::DisplayName] = QByteArrayLiteral("displayname");
    roles[InputCompleterModel::CompleterName] = QByteArrayLiteral("completername");
    roles[InputCompleterModel::IconName] = QByteArrayLiteral("iconname");
    roles[InputCompleterModel::ChannelType] = QByteArrayLiteral("channeltype");
    return roles;
}
