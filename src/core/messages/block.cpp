/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "block.h"

#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Block::Block() = default;

void Block::parseBlock(const QJsonObject &block)
{
    // "blocks":[{"appId":"videoconf-core","blockId":"63981f8a4ef3f3baa965a0d8","callId":"63981f8a4ef3f3baa965a0d8","type":"video_conf"}]
    mBlockId = block["blockId"_L1].toString();
    mCallId = block["callId"_L1].toString();
    mAppId = block["appId"_L1].toString();
    setBlockTypeStr(block["type"_L1].toString());
    if (mBlockType == BlockType::Unknown) {
        qCWarning(RUQOLA_LOG) << " Unknown type " << block;
    } else if (mBlockType == BlockType::Section) {
        if (block.contains("text"_L1)) {
            const QJsonObject objText = block["text"_L1].toObject();
            mSectionText = objText["text"_L1].toString();
        }
    }

    const QJsonArray elements = block["elements"_L1].toArray();
    const auto elementsCount = elements.count();
    mBlockActions.reserve(elementsCount);
    for (auto i = 0; i < elementsCount; ++i) {
        BlockAction action;
        action.parseAction(elements.at(i).toObject());
        if (action.isValid()) {
            mBlockActions.append(std::move(action));
        } else {
            qDebug() << "Invalid elements" << elements.at(i).toObject() << " action " << action;
            qCWarning(RUQOLA_LOG) << "Invalid elements" << elements.at(i).toObject();
        }
    }
}

QString Block::convertEnumToStr(BlockType newBlockType) const
{
    switch (newBlockType) {
    case BlockType::Unknown:
        return {};
    case BlockType::VideoConf:
        return QStringLiteral("video_conf");
    case BlockType::Context:
        return QStringLiteral("context");
    case BlockType::Section:
        return QStringLiteral("section");
    case BlockType::Actions:
        return QStringLiteral("actions");
    case BlockType::Divider:
        return QStringLiteral("divider");
    case BlockType::Overflow:
        return QStringLiteral("overflow");
    }
    return {};
}

Block::BlockType Block::convertBlockTypeToEnum(const QString &typeStr)
{
    if (typeStr == "video_conf"_L1) {
        return BlockType::VideoConf;
    } else if (typeStr == "section"_L1) {
        return BlockType::Section;
    } else if (typeStr == "actions"_L1) {
        return BlockType::Actions;
    } else if (typeStr == "context"_L1) {
        return BlockType::Context;
    } else if (typeStr == "divider"_L1) {
        return BlockType::Divider;
    } else if (typeStr == "overflow"_L1) {
        return BlockType::Overflow;
    }

    qCWarning(RUQOLA_LOG) << " Invalid BlockType " << typeStr;
    return BlockType::Unknown;
}

QList<BlockAction> Block::blockActions() const
{
    return mBlockActions;
}

void Block::setBlockActions(const QList<BlockAction> &newBlockActions)
{
    mBlockActions = newBlockActions;
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
    return mBlockType != BlockType::Unknown;
}

QString Block::title() const
{
    switch (mBlockType) {
    case BlockType::Section:
    case BlockType::Actions:
    case BlockType::Context:
    case BlockType::Divider:
    case BlockType::Overflow:
    case BlockType::Unknown:
        break;
    case BlockType::VideoConf: {
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
    return mBlockId == other.blockId() && mCallId == other.callId() && mAppId == other.appId() && mBlockStr == other.blockTypeStr()
        && mBlockActions == other.blockActions() && mSectionText == other.sectionText();
}

QString Block::sectionText() const
{
    return mSectionText;
}

void Block::setSectionText(const QString &newSectionText)
{
    mSectionText = newSectionText;
}

QJsonObject Block::serialize(const Block &block)
{
    QJsonObject o;
    o["blockId"_L1] = block.blockId();
    o["callId"_L1] = block.callId();
    o["appId"_L1] = block.appId();
    o["type"_L1] = block.blockTypeStr();
    if (!block.sectionText().isEmpty()) {
        o["sectionText"_L1] = block.sectionText();
    }
    if (block.mVideoConferenceInfo.isValid()) {
        o["videoconferenceinfo"_L1] = VideoConferenceInfo::serialize(block.mVideoConferenceInfo);
    } else {
        qCWarning(RUQOLA_LOG) << "block.mVideoConferenceInfo is invalid " << block.mVideoConferenceInfo;
    }
    return o;
}

Block Block::deserialize(const QJsonObject &o)
{
    Block block;
    block.setBlockId(o["blockId"_L1].toString());
    block.setCallId(o["callId"_L1].toString());
    block.setAppId(o["appId"_L1].toString());
    block.setBlockTypeStr(o["type"_L1].toString());
    block.setSectionText(o["sectionText"_L1].toString());
    const VideoConferenceInfo info = VideoConferenceInfo::deserialize(o["videoconferenceinfo"_L1].toObject());
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
    d.space() << "Text Section" << t.sectionText();
    d.space() << "Block Actions" << t.blockActions();
    return d;
}

#include "moc_block.cpp"
