/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inputcompletermodel.h"
#include "rocketchataccount.h"
#include <KLocalizedString>

#include <QJsonArray>

#include <QIcon>
#include <QModelIndex>

InputCompleterModel::InputCompleterModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
{
}

InputCompleterModel::~InputCompleterModel() = default;

QString InputCompleterModel::here()
{
    return QStringLiteral("here");
}

QString InputCompleterModel::all()
{
    return QStringLiteral("all");
}

ChannelUserCompleter InputCompleterModel::createHereChannel()
{
    ChannelUserCompleter here;
    here.setName(InputCompleterModel::here());
    here.setDescription(i18n("Notify all in this room"));
    here.setType(ChannelUserCompleter::ChannelUserCompleterType::Notification);
    return here;
}

ChannelUserCompleter InputCompleterModel::noFoundChannelUser()
{
    ChannelUserCompleter noFound;
    noFound.setDescription(i18n("No result found."));
    noFound.setType(ChannelUserCompleter::ChannelUserCompleterType::Unknown);
    return noFound;
}

ChannelUserCompleter InputCompleterModel::createAllChannel()
{
    ChannelUserCompleter all;
    all.setName(InputCompleterModel::all());
    all.setDescription(i18n("Notify active users in this room"));
    all.setType(ChannelUserCompleter::ChannelUserCompleterType::Notification);
    return all;
}

void InputCompleterModel::setDefaultUserCompletion()
{
    // Show here/all when we only use "@"
    QList<ChannelUserCompleter> customCompletion;

    customCompletion.append(createHereChannel());
    customCompletion.append(createAllChannel());

    setChannels(customCompletion);
}

void InputCompleterModel::setChannels(const QList<ChannelUserCompleter> &channels)
{
    clear();
    if (!channels.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        mChannelUserCompleters = channels;
        endInsertRows();
    }
}

QList<ChannelUserCompleter> InputCompleterModel::searchOpenedRooms()
{
    QList<ChannelUserCompleter> channels;
    if (mRocketChatAccount) {
        if (!mSearchInfo.searchString.isEmpty()) {
            const QList<Room *> rooms = mRocketChatAccount->roomModel()->findRoomNameConstains(mSearchInfo.searchString);
            for (const Room *room : rooms) {
                if (room->channelType() == Room::RoomType::Channel) { // Only direct channel.
                    ChannelUserCompleter channel;
                    channel.setType(ChannelUserCompleter::ChannelUserCompleterType::Room);
                    channel.setName(room->displayFName());
                    channel.setIdentifier(room->roomId());
                    channel.setChannelIcon();
                    channel.setAvatarInfo(room->avatarInfo());
                    channels.append(std::move(channel));
                }
            }
        }
    }
    return channels;
}

void InputCompleterModel::setSearchInfo(const SearchInfo &newSearchInfo)
{
    mSearchInfo = newSearchInfo;
}

void InputCompleterModel::parseSearchChannels(const QJsonObject &obj)
{
    QList<ChannelUserCompleter> channelList;
    const QJsonArray rooms = obj.value(QLatin1String("items")).toArray();
    const auto roomsSize(rooms.size());
    channelList.reserve(roomsSize);
    for (auto i = 0; i < roomsSize; i++) {
        const QJsonObject o = rooms.at(i).toObject();
        ChannelUserCompleter channel;
        channel.parseChannel(o, ChannelUserCompleter::ChannelUserCompleterType::Room);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    if (channelList.isEmpty()) {
        channelList.append(noFoundChannelUser());
    }
    setChannels(channelList);
}

void InputCompleterModel::parseChannels(const QJsonObject &obj)
{
    QList<ChannelUserCompleter> channelList;
    if (mSearchInfo.searchType == SearchInfo::Channels || mSearchInfo.searchType == SearchInfo::ChannelsAndUsers) {
        const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
        channelList.reserve(rooms.size());
        for (int i = 0; i < rooms.size(); i++) {
            const QJsonObject o = rooms.at(i).toObject();
            ChannelUserCompleter channel;
            channel.parseChannel(o, ChannelUserCompleter::ChannelUserCompleterType::Room);
            // Verify that it's valid
            channelList.append(std::move(channel));
        }
        channelList.append(searchOpenedRooms());
    }
    if (mSearchInfo.searchType == SearchInfo::Users || mSearchInfo.searchType == SearchInfo::ChannelsAndUsers) {
        const QJsonArray users = obj.value(QLatin1String("users")).toArray();
        bool needToAddAll = false;
        bool needToAddHere = false;
        for (int i = 0; i < users.size(); i++) {
            const QJsonObject o = users.at(i).toObject();
            ChannelUserCompleter user;
            user.parseChannel(o, ChannelUserCompleter::ChannelUserCompleterType::DirectChannel);
            if (!mSearchInfo.searchString.isEmpty()) {
                if (!needToAddAll && InputCompleterModel::all().startsWith(mSearchInfo.searchString)) {
                    needToAddAll = true;
                }
                if (!needToAddHere && InputCompleterModel::here().startsWith(mSearchInfo.searchString)) {
                    needToAddHere = true;
                }
            }
            // Verify that it's valid
            channelList.append(std::move(user));
        }
        if (needToAddAll) {
            channelList.append(createAllChannel());
        }
        if (needToAddHere) {
            channelList.append(createHereChannel());
        }
    }
    if (channelList.isEmpty()) {
        channelList.append(noFoundChannelUser());
    }
    setChannels(channelList);
}

void InputCompleterModel::clear()
{
    if (!mChannelUserCompleters.isEmpty()) {
        beginResetModel();
        mChannelUserCompleters.clear();
        endResetModel();
    }
}

int InputCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mChannelUserCompleters.count();
}

QVariant InputCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mChannelUserCompleters.count()) {
        return {};
    }
    const ChannelUserCompleter channelUserCompleter = mChannelUserCompleters.at(index.row());
    switch (role) {
    case InputCompleterModel::DisplayName:
    case Qt::DisplayRole:
        return channelUserCompleter.name();
    case InputCompleterModel::CompleterName:
        return channelUserCompleter.completerName();
    case InputCompleterModel::IconStatus:
    case Qt::DecorationRole:
        return channelUserCompleter.statusIcon();
    case InputCompleterModel::AvatarInfo:
        return QVariant::fromValue(channelUserCompleter.avatarInfo());
    case InputCompleterModel::Description:
        return channelUserCompleter.description();
    case InputCompleterModel::UserName:
        return channelUserCompleter.userName();
    case InputCompleterModel::OutsideRoom:
        return channelUserCompleter.outsideRoom();
    case InputCompleterModel::ChannelType:
        return channelUserCompleter.type();
    case InputCompleterModel::Identifier:
        return channelUserCompleter.identifier();
    }
    return {};
}

#include "moc_inputcompletermodel.cpp"
