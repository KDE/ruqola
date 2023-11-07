/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inputcompletermodel.h"
#include <KLocalizedString>

#include <QJsonArray>

#include <QIcon>
#include <QModelIndex>

InputCompleterModel::InputCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

InputCompleterModel::~InputCompleterModel() = default;

ChannelUserCompleter InputCompleterModel::createHereChannel()
{
    ChannelUserCompleter here;
    here.setName(QStringLiteral("here"));
    here.setDescription(i18n("Notify all in this room"));
    here.setType(ChannelUserCompleter::ChannelUserCompleterType::DirectChannel);
    return here;
}

ChannelUserCompleter InputCompleterModel::createAllChannel()
{
    ChannelUserCompleter all;
    all.setName(QStringLiteral("all"));
    all.setDescription(i18n("Notify active users in this room"));
    all.setType(ChannelUserCompleter::ChannelUserCompleterType::DirectChannel);
    return all;
}

void InputCompleterModel::setDefaultUserCompletion()
{
    // Show here/all when we only use "@"
    QVector<ChannelUserCompleter> customCompletion;

    customCompletion.append(createHereChannel());
    customCompletion.append(createAllChannel());

    setChannels(customCompletion);
}

void InputCompleterModel::setSearchUserString(const QString &str)
{
    mSetSearchUserString = str;
}

void InputCompleterModel::setChannels(const QVector<ChannelUserCompleter> &channels)
{
    if (rowCount() != 0) {
        beginResetModel();
        mChannel.clear();
        endResetModel();
    }
    if (!channels.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        mChannel = channels;
        endInsertRows();
    }
}

void InputCompleterModel::parseChannels(const QJsonObject &obj)
{
    QVector<ChannelUserCompleter> channelList;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
    channelList.reserve(rooms.size());
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        ChannelUserCompleter channel;
        channel.parseChannel(o, ChannelUserCompleter::ChannelUserCompleterType::Room);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    const QJsonArray users = obj.value(QLatin1String("users")).toArray();
    bool needToAddAll = false;
    bool needToAddHere = false;
    for (int i = 0; i < users.size(); i++) {
        const QJsonObject o = users.at(i).toObject();
        ChannelUserCompleter user;
        user.parseChannel(o, ChannelUserCompleter::ChannelUserCompleterType::DirectChannel);
        if (!needToAddAll && mSetSearchUserString.startsWith(QLatin1Char('a'))) {
            needToAddAll = true;
        }
        if (!needToAddHere && mSetSearchUserString.startsWith(QLatin1Char('h'))) {
            needToAddHere = true;
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
    Q_UNUSED(parent)
    return mChannel.count();
}

QVariant InputCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mChannel.count()) {
        return {};
    }
    const ChannelUserCompleter channel = mChannel.at(index.row());
    switch (role) {
    case InputCompleterModel::DisplayName:
    case Qt::DisplayRole:
        return channel.name();
    case InputCompleterModel::CompleterName:
        return channel.name();
    case InputCompleterModel::IconStatus:
    case Qt::DecorationRole:
        return channel.statusIcon();
    case InputCompleterModel::ChannelUserIcon:
        return channel.channelUserIcon();
    case InputCompleterModel::Description:
        return channel.description();
    case InputCompleterModel::UserName:
        return channel.userName();
    }
    return {};
}

#include "moc_inputcompletermodel.cpp"
