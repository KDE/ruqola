/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "replies.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Replies::Replies()
    : QSharedData()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Replies created " << this;
}

Replies::Replies(const Replies &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Replies(const Replies &other) created " << this;
    mReplies = other.replies();
}

Replies::~Replies()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Replies deleted " << this;
}

QList<QByteArray> Replies::replies() const
{
    return mReplies;
}

void Replies::setReplies(const QList<QByteArray> &replies)
{
    mReplies = replies;
}

void Replies::parseReplies(const QJsonArray &channels)
{
    mReplies.clear();
    // TODO
}

bool Replies::operator==(const Replies &other) const
{
    return mReplies == other.replies();
}

QDebug operator<<(QDebug d, const Replies &t)
{
    d.space() << t.replies() << "\n";
    return d;
}

QJsonArray Replies::serialize(const Replies &channels)
{
    QJsonArray array;
    // Replies
    // TODO
    return array;
}

Replies *Replies::deserialize(const QJsonArray &channelsArray)
{
    // TODO
    auto final = new Replies;
    // TODO final->setReplies(channels);
    return final;
}

bool Replies::isEmpty() const
{
    return mReplies.isEmpty();
}
