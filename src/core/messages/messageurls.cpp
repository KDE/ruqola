/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurls.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageUrls::MessageUrls()
    : QSharedData()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageUrls created " << this;
}

MessageUrls::MessageUrls(const MessageUrls &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageUrls(const MessageUrls &other) created " << this;
    mMessageUrls = other.blocks();
}

MessageUrls::~MessageUrls()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageUrls deleted " << this;
}

void MessageUrls::setMessageUrls(const QList<Block> &blocks)
{
    mMessageUrls = blocks;
}

QList<Block> MessageUrls::blocks() const
{
    return mMessageUrls;
}

void MessageUrls::parseMessageUrls(const QJsonObject &reactsr)
{
    mMessageUrls.clear();
}

bool MessageUrls::operator==(const MessageUrls &other) const
{
    return mMessageUrls == other.blocks();
}

QDebug operator<<(QDebug d, const MessageUrls &t)
{
    for (int i = 0; i < t.blocks().count(); i++) {
        d.space() << t.blocks().at(i) << "\n";
    }
    return d;
}

QJsonObject MessageUrls::serialize(const MessageUrls &blocks)
{
    QJsonObject obj;
    // TODO
    return obj;
}

MessageUrls *MessageUrls::deserialize(const QJsonObject &o)
{
    // TODO
    return {};
}

bool MessageUrls::isEmpty() const
{
    return mMessageUrls.isEmpty();
}
