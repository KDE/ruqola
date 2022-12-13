/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "block.h"

Block::Block() = default;

void Block::parseBlock(const QJsonObject &block)
{
    // "blocks":[{"appId":"videoconf-core","blockId":"63981f8a4ef3f3baa965a0d8","callId":"63981f8a4ef3f3baa965a0d8","type":"video_conf"}]
    // TODO
    mBlockId = block[QLatin1String("blockId")].toString();
    mCallId = block[QLatin1String("callId")].toString();
}

QString Block::blockId() const
{
    return mBlockId;
}

void Block::setBlockId(const QString &newBlockId)
{
    mBlockId = newBlockId;
}

QString Block::callId() const
{
    return mCallId;
}

void Block::setCallId(const QString &newCallId)
{
    mCallId = newCallId;
}

bool Block::operator==(const Block &other) const
{
    return mBlockId == other.blockId() && mCallId == other.callId();
}

QJsonObject Block::serialize(const Block &block)
{
    // TODO
    return {};
}

Block Block::fromJSon(const QJsonObject &o)
{
    // TODO
    return {};
}

QDebug operator<<(QDebug d, const Block &t)
{
    d << "blockId " << t.blockId();
    d << "callId " << t.callId();
    return d;
}
