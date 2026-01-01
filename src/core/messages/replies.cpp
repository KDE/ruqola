/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "replies.h"

#include "ruqola_message_memory_debug.h"

using namespace Qt::Literals::StringLiterals;
Replies::Replies()

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

void Replies::parseReplies(const QJsonArray &replieArray)
{
    mReplies.clear();
    QList<QByteArray> lst;
    const auto nbReplieArrayCount{replieArray.count()};
    lst.reserve(nbReplieArrayCount);
    for (auto i = 0; i < nbReplieArrayCount; ++i) {
        lst.append(replieArray.at(i).toString().toLatin1());
    }
    mReplies = lst;
}

bool Replies::operator==(const Replies &other) const
{
    return mReplies == other.replies();
}

QJsonArray Replies::serialize(const Replies &replies)
{
    QJsonArray array;
    QStringList serialize;
    serialize.reserve(replies.replies().count());
    for (const QByteArray &i : replies.replies()) {
        serialize << QString::fromLatin1(i);
    }
    return QJsonArray::fromStringList(serialize);
}

Replies *Replies::deserialize(const QJsonArray &repliesArray)
{
    QList<QByteArray> replies;
    replies.reserve(repliesArray.count());
    for (int i = 0, total = repliesArray.count(); i < total; ++i) {
        replies.append(repliesArray.at(i).toString().toLatin1());
    }
    auto final = new Replies;
    final->setReplies(replies);
    return final;
}

bool Replies::isEmpty() const
{
    return mReplies.isEmpty();
}

QDebug operator<<(QDebug d, const Replies &t)
{
    d.space() << t.replies() << "\n";
    return d;
}
