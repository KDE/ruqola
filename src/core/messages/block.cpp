/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "block.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QJsonObject>

Block::Block() = default;

void Block::parseBlock(const QJsonObject &block)
{
    // "blocks":[{"appId":"videoconf-core","blockId":"63981f8a4ef3f3baa965a0d8","callId":"63981f8a4ef3f3baa965a0d8","type":"video_conf"}]
    mBlockId = block[QLatin1String("blockId")].toString();
    mCallId = block[QLatin1String("callId")].toString();
    mAppId = block[QLatin1String("appId")].toString();
    setBlockTypeStr(block[QLatin1String("type")].toString());
    if (mBlockType == Unknown) {
        qCWarning(RUQOLA_LOG) << " Unknown type " << block;
    }
}

QString Block::convertEnumToStr(BlockType newBlockType) const
{
    switch (newBlockType) {
    case Unknown:
        return {};
    case VideoConf:
        return QStringLiteral("video_conf");
    }
    return {};
}

Block::BlockType Block::convertBlockTypeToEnum(const QString &typeStr)
{
    if (typeStr == QLatin1String("video_conf")) {
        return VideoConf;
    }
    qCWarning(RUQOLA_LOG) << " Invalid BlockType " << typeStr;
    return Unknown;
}

VideoConferenceInfo Block::videoConferenceInfo() const
{
    return mVideoConferenceInfo;
}

void Block::setVideoConferenceInfo(const VideoConferenceInfo &newInfo)
{
    mVideoConferenceInfo = newInfo;
}

QString Block::blockTypeStr() const
{
    return mBlockStr;
}

void Block::setBlockTypeStr(const QString &newBlockStr)
{
    if (mBlockStr != newBlockStr) {
        mBlockStr = newBlockStr;
        mBlockType = convertBlockTypeToEnum(mBlockStr);
    }
}

bool Block::isValid() const
{
    return mBlockType != Unknown;
}

QString Block::title() const
{
    switch (mBlockType) {
    case Block::BlockType::Unknown:
        break;
    case Block::BlockType::VideoConf: {
        if (mVideoConferenceInfo.isValid()) {
            const QString title = mVideoConferenceInfo.title();
            if (!title.isEmpty()) {
                return title;
            }
        }
        return i18n("Conference Call");
    }
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
    mBlockStr = convertEnumToStr(mBlockType);
}

bool Block::operator==(const Block &other) const
{
    return mBlockId == other.blockId() && mCallId == other.callId() && mAppId == other.appId() && mBlockStr == other.blockTypeStr();
}

QJsonObject Block::serialize(const Block &block)
{
    QJsonObject o;
    o[QLatin1String("blockId")] = block.blockId();
    o[QLatin1String("callId")] = block.callId();
    o[QLatin1String("appId")] = block.appId();
    o[QLatin1String("type")] = block.blockTypeStr();
    if (block.mVideoConferenceInfo.isValid()) {
        o[QLatin1String("videoconferenceinfo")] = VideoConferenceInfo::serialize(block.mVideoConferenceInfo);
    } else {
        qCWarning(RUQOLA_LOG) << "block.mVideoConferenceInfo is invalid " << block.mVideoConferenceInfo;
    }
    return o;
}

Block Block::deserialize(const QJsonObject &o)
{
    Block block;
    block.setBlockId(o[QLatin1String("blockId")].toString());
    block.setCallId(o[QLatin1String("callId")].toString());
    block.setAppId(o[QLatin1String("appId")].toString());
    block.setBlockTypeStr(o[QLatin1String("type")].toString());
    const VideoConferenceInfo info = VideoConferenceInfo::deserialize(o[QLatin1String("videoconferenceinfo")].toObject());
    if (info.isValid()) {
        block.mVideoConferenceInfo = info;
    } else {
        qCWarning(RUQOLA_LOG) << "info is invalid " << info;
    }
    return block;
}

QDebug operator<<(QDebug d, const Block &t)
{
    d.space() << "blockId" << t.blockId();
    d.space() << "callId" << t.callId();
    d.space() << "appId" << t.appId();
    d.space() << "blockTypeStr" << t.blockTypeStr();
    d.space() << "mBlockType" << t.blockType();
    d.space() << "Video conf info" << t.videoConferenceInfo();
    return d;
}

#include "moc_block.cpp"
