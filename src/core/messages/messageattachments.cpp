/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachments.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageAttachments::MessageAttachments()
    : QSharedData()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageAttachments created " << this;
}

MessageAttachments::MessageAttachments(const MessageAttachments &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageAttachments(const MessageAttachments &other) created " << this;
    mMessageAttachments = other.blocks();
}

MessageAttachments::~MessageAttachments()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageAttachments deleted " << this;
}

void MessageAttachments::setMessageAttachments(const QList<Block> &blocks)
{
    mMessageAttachments = blocks;
}

QList<Block> MessageAttachments::blocks() const
{
    return mMessageAttachments;
}

void MessageAttachments::parseMessageAttachments(const QJsonObject &reactsr)
{
    mMessageAttachments.clear();
}

bool MessageAttachments::operator==(const MessageAttachments &other) const
{
    return mMessageAttachments == other.blocks();
}

QDebug operator<<(QDebug d, const MessageAttachments &t)
{
    for (int i = 0; i < t.blocks().count(); i++) {
        d.space() << t.blocks().at(i) << "\n";
    }
    return d;
}

QJsonObject MessageAttachments::serialize(const MessageAttachments &blocks)
{
    QJsonObject obj;
    // TODO
    return obj;
}

MessageAttachments *MessageAttachments::deserialize(const QJsonObject &o)
{
    // TODO
    return {};
}

bool MessageAttachments::isEmpty() const
{
    return mMessageAttachments.isEmpty();
}
