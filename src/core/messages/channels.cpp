/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channels.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Channels::Channels()

{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Channels created " << this;
}

Channels::Channels(const Channels &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Channels(const Channels &other) created " << this;
    mChannels = other.channels();
}

Channels::~Channels()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Channels deleted " << this;
}

void Channels::setChannels(const QList<ChannelInfo> &channels)
{
    mChannels = channels;
}

QList<Channels::ChannelInfo> Channels::channels() const
{
    return mChannels;
}

void Channels::parseChannels(const QJsonArray &channels)
{
    mChannels.clear();
    for (int i = 0, total = channels.size(); i < total; ++i) {
        const QJsonObject channel = channels.at(i).toObject();
        ChannelInfo info;
        info.name = channel.value("name"_L1).toString();
        info.fname = channel.value("fname"_L1).toString();
        info.identifier = channel.value("_id"_L1).toString().toLatin1();

        mChannels.append(std::move(info));
    }
}

bool Channels::operator==(const Channels &other) const
{
    return mChannels == other.channels();
}

QDebug operator<<(QDebug d, const Channels &t)
{
    for (int i = 0; i < t.channels().count(); i++) {
        d.space() << t.channels().at(i) << "\n";
    }
    return d;
}

QJsonArray Channels::serialize(const Channels &channels)
{
    QJsonArray array;
    // Channels
    for (const ChannelInfo &info : channels.channels()) {
        QJsonObject channel;
        channel.insert("_id"_L1, QString::fromLatin1(info.identifier));
        channel.insert("name"_L1, info.name);
        if (!info.fname.isEmpty()) {
            channel.insert("fname"_L1, info.fname);
        }
        array.append(std::move(channel));
    }
    return array;
}

Channels *Channels::deserialize(const QJsonArray &channelsArray)
{
    QList<ChannelInfo> channels;
    const int total = channelsArray.count();
    channels.reserve(total);
    for (int i = 0; i < total; ++i) {
        const QJsonObject channel = channelsArray.at(i).toObject();
        ChannelInfo info;
        info.name = channel.value("name"_L1).toString();
        info.fname = channel.value("fname"_L1).toString();
        info.identifier = channel.value("_id"_L1).toString().toLatin1();
        channels.append(std::move(info));
    }
    auto final = new Channels;
    final->setChannels(channels);
    return final;
}

bool Channels::isEmpty() const
{
    return mChannels.isEmpty();
}

bool Channels::ChannelInfo::operator==(const ChannelInfo &other) const
{
    return other.fname == fname && other.identifier == identifier && other.name == name;
}

QDebug operator<<(QDebug d, const Channels::ChannelInfo &t)
{
    d.space() << "fname:" << t.fname;
    d.space() << "name:" << t.name;
    d.space() << "identifier:" << t.identifier;
    return d;
}
