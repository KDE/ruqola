/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "block.h"
#include "ruqola_debug.h"
#include <KLocalizedString>

Block::Block() = default;

void Block::parseBlock(const QJsonObject &block)
{
    // "blocks":[{"appId":"videoconf-core","blockId":"63981f8a4ef3f3baa965a0d8","callId":"63981f8a4ef3f3baa965a0d8","type":"video_conf"}]
    mBlockId = block[QLatin1String("blockId")].toString();
    mCallId = block[QLatin1String("callId")].toString();
    mAppId = block[QLatin1String("appId")].toString();
    mBlockType = convertBlockTypeToEnum(block[QLatin1String("type")].toString());
    if (mBlockType == Unknown) {
        qDebug() << " Unknown type " << block;
    }
}

Block::BlockType Block::convertBlockTypeToEnum(const QString &typeStr)
{
    if (typeStr == QLatin1String("video_conf")) {
        return VideoConf;
    }
    qCWarning(RUQOLA_LOG) << " Invalid BlockType " << typeStr;
    return Unknown;
}

bool Block::isValid() const
{
    return mBlockType != Unknown;
}

QString Block::generateDescription() const
{
    switch (mBlockType) {
    case Block::BlockType::Unknown:
        break;
    case Block::BlockType::VideoConf:
        return i18n("Conference Call");
    }
    return {};
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

QString Block::appId() const
{
    return mAppId;
}

void Block::setAppId(const QString &newAppId)
{
    mAppId = newAppId;
}

Block::BlockType Block::blockType() const
{
    return mBlockType;
}

void Block::setBlockType(BlockType newBlockType)
{
    mBlockType = newBlockType;
}

bool Block::operator==(const Block &other) const
{
    return mBlockId == other.blockId() && mCallId == other.callId() && mAppId == other.appId();
}

QJsonObject Block::serialize(const Block &block)
{
    QJsonObject o;
    o[QStringLiteral("blockId")] = block.blockId();
    o[QStringLiteral("callId")] = block.callId();
    o[QStringLiteral("appId")] = block.appId();
    // TODO type
    // TODO
    return o;
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
    d << "appId " << t.appId();
    return d;
}
