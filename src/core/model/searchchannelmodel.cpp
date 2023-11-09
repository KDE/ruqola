/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchchannelmodel.h"
#include "ruqola_debug.h"

#include <QIcon>
#include <QJsonArray>
#include <QJsonObject>

SearchChannelModel::SearchChannelModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

SearchChannelModel::~SearchChannelModel() = default;

void SearchChannelModel::setChannels(const QVector<Channel> &channels)
{
    clear();
    if (!channels.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        mChannel = channels;
        endInsertRows();
    }
}

void SearchChannelModel::parseAllChannels(const QJsonObject &obj)
{
    QVector<Channel> channelList;
    const QJsonArray rooms = obj.value(QLatin1String("channels")).toArray();
    // qDebug() << " rooms " << rooms;
    const auto roomSize{rooms.size()};
    channelList.reserve(roomSize);
    for (int i = 0; i < roomSize; i++) {
        const QJsonObject o = rooms.at(i).toObject();
        // qDebug() << " o " << o;
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        channelList.append(std::move(channel));
    }
    setChannels(channelList);
}

void SearchChannelModel::parseChannels(const QJsonObject &obj)
{
    QVector<Channel> channelList;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
    const QJsonArray users = obj.value(QLatin1String("users")).toArray();
    const QJsonArray roomDirectory = obj.value(QLatin1String("result")).toArray();
    channelList.reserve(rooms.size() + users.size() + roomDirectory.size());
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    for (int i = 0; i < users.size(); i++) {
        const QJsonObject o = users.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::DirectChannel);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    for (int i = 0; i < roomDirectory.size(); i++) {
        const QJsonObject o = roomDirectory.at(i).toObject();
        // qDebug() << " o " << o;
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    setChannels(channelList);
}

void SearchChannelModel::clear()
{
    if (!mChannel.isEmpty()) {
        beginResetModel();
        mChannel.clear();
        endResetModel();
    }
}

int SearchChannelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mChannel.count();
}

QVariant SearchChannelModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mChannel.count()) {
        return {};
    }
    const Channel channel = mChannel.at(index.row());
    switch (role) {
    case SearchChannelModel::ChannelId:
        return channelId(channel);
    case SearchChannelModel::ChannelName:
        return channelName(channel);
    case SearchChannelModel::IconName:
        return channelIconName(channel);
    case SearchChannelModel::ChannelType:
        return channel.type();
    }

    return {};
}

QString SearchChannelModel::channelId(const Channel &channel) const
{
    // Specific channelId for opening room
    // For private channel we need to use username for channel we need roomId
    switch (channel.type()) {
    case Channel::ChannelType::DirectChannel:
        return channel.userName();
    case Channel::ChannelType::Room:
        return channel.roomId();
    case Channel::ChannelType::Unknown:
        qCWarning(RUQOLA_LOG) << "Unknown channel type!";
        return {};
    }
    return {};
}

QString SearchChannelModel::channelName(const Channel &channel) const
{
    switch (channel.type()) {
    case Channel::ChannelType::DirectChannel:
        return channel.userName();
    case Channel::ChannelType::Room:
        return channel.roomName();
    case Channel::ChannelType::Unknown:
        qCWarning(RUQOLA_LOG) << "Unknown channel type!";
        return {};
    }
    return {};
}

QIcon SearchChannelModel::channelIconName(const Channel &channel) const
{
    switch (channel.type()) {
    case Channel::ChannelType::DirectChannel:
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

#include "moc_searchchannelmodel.cpp"
