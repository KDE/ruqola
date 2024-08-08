/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channels.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Channels::Channels()
    : QSharedData()
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

void Channels::setChannels(const QList<ChannelInfo> &reactions)
{
    mChannels = reactions;
}

QList<Channels::ChannelInfo> Channels::channels() const
{
    return mChannels;
}

void Channels::parseChannels(const QJsonObject &reacts)
{
    // TODO
    mChannels.clear();
}

bool Channels::operator==(const Channels &other) const
{
    return mChannels == other.channels();
}

QDebug operator<<(QDebug d, const Channels &t)
{
    for (int i = 0; i < t.reactions().count(); i++) {
        d.space() << t.reactions().at(i) << "\n";
    }
    return d;
}

QJsonObject Channels::serialize(const Channels &reactions)
{
    QJsonObject obj;
    for (int i = 0; i < reactions.reactions().count(); ++i) {
        QJsonObject react;
        react["usernames"_L1] = QJsonArray::fromStringList(reactions.reactions().at(i).userNames());
        obj[reactions.reactions().at(i).reactionName()] = react;
    }
    return obj;
}

Channels *Channels::deserialize(const QJsonObject &o)
{
    QList<Reaction> reacts;
    const QStringList lst = o.keys();
    QStringList users;
    for (const QString &str : lst) {
        const QJsonObject obj = o.value(str).toObject();
        const QJsonValue usernames = obj.value("usernames"_L1);
        if (!usernames.isUndefined()) {
            const QJsonArray array = usernames.toArray();
            for (int i = 0; i < array.count(); ++i) {
                users.append(array.at(i).toString());
            }
            if (!users.isEmpty()) {
                Reaction r;
                r.setReactionName(str, emojiManager);
                r.setUserNames(users);
                reacts.append(std::move(r));
            }
        }
        users.clear();
    }
    auto final = new Channels;
    final->setChannels(reacts);
    return final;
}

bool Channels::isEmpty() const
{
    return mChannels.isEmpty();
}
