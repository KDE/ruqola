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

Block::~Block() = default;

void Block::parseBlock(const QJsonObject &block)
{
    // "blocks":[{"appId":"videoconf-core","blockId":"63981f8a4ef3f3baa965a0d8","callId":"63981f8a4ef3f3baa965a0d8","type":"video_conf"}]
    mBlockId = block["blockId"_L1].toString();
    mCallId = block["callId"_L1].toString();
    mAppId = block["appId"_L1].toString();
    setBlockType(convertBlockTypeToEnum(block["type"_L1].toString()));
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
            // qDebug() << "Invalid elements" << elements.at(i).toObject() << " action " << action;
            qCWarning(RUQOLA_LOG) << "Invalid elements" << elements.at(i).toObject();
        }
    }
    if (block.contains("accessory"_L1)) {
        const QJsonObject accessory = block["accessory"_L1].toObject();
        mBlockAccessory.parseAccessory(accessory);
    }
}

QString Block::convertEnumToStr(BlockType newBlockType)
{
    switch (newBlockType) {
    case BlockType::Unknown:
        return {};
    case BlockType::VideoConf:
        return u"video_conf"_s;
    case BlockType::Context:
        return u"context"_s;
    case BlockType::Section:
        return u"section"_s;
    case BlockType::Actions:
        return u"actions"_s;
    case BlockType::Divider:
        return u"divider"_s;
    case BlockType::Overflow:
        return u"overflow"_s;
    case BlockType::Image:
        return u"image"_s;
    case BlockType::Preview:
        return u"preview"_s;
    case BlockType::Callout:
        return u"callout"_s;
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
    } else if (typeStr == "image"_L1) {
        return BlockType::Image;
    } else if (typeStr == "preview"_L1) {
        return BlockType::Preview;
    } else if (typeStr == "callout"_L1) {
        return BlockType::Callout;
    }

    qCWarning(RUQOLA_LOG) << " Invalid BlockType " << typeStr;
    return BlockType::Unknown;
}

BlockAccessory Block::blockAccessory() const
{
    return mBlockAccessory;
}

void Block::setBlockAccessory(const BlockAccessory &newBlockAccessory)
{
    mBlockAccessory = newBlockAccessory;
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
    case BlockType::Image:
    case BlockType::Preview:
    case BlockType::Callout:
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
}

bool Block::operator==(const Block &other) const
{
    return mBlockId == other.blockId() && mCallId == other.callId() && mAppId == other.appId() && mBlockActions == other.blockActions()
        && mSectionText == other.sectionText() && mBlockAccessory == other.blockAccessory() && mVideoConferenceInfo == other.videoConferenceInfo();
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
    if (!block.callId().isEmpty()) {
        o["callId"_L1] = block.callId();
    }
    o["appId"_L1] = block.appId();
    o["type"_L1] = Block::convertEnumToStr(block.blockType());
    if (!block.sectionText().isEmpty()) {
        o["sectionText"_L1] = block.sectionText();
    }
    if (block.mVideoConferenceInfo.isValid()) {
        o["videoconferenceinfo"_L1] = VideoConferenceInfo::serialize(block.mVideoConferenceInfo);
    } else {
        qCWarning(RUQOLA_LOG) << "block.mVideoConferenceInfo is invalid " << block.mVideoConferenceInfo;
    }
    if (block.blockAccessory().isValid()) {
        o["accessory"_L1] = BlockAccessory::serialize(block.blockAccessory());
    }
    if (!block.blockActions().isEmpty()) {
        QJsonArray array;
        for (const auto &action : block.blockActions()) {
            array.append(BlockAction::serialize(action));
        }
        o["elements"_L1] = array;
    }
    return o;
}

Block Block::deserialize(const QJsonObject &o)
{
    Block block;
    block.setBlockId(o["blockId"_L1].toString());
    block.setCallId(o["callId"_L1].toString());
    block.setAppId(o["appId"_L1].toString());
    block.setBlockType(Block::convertBlockTypeToEnum(o["type"_L1].toString()));
    block.setSectionText(o["sectionText"_L1].toString());
    const VideoConferenceInfo info = VideoConferenceInfo::deserialize(o["videoconferenceinfo"_L1].toObject());
    if (info.isValid()) {
        block.mVideoConferenceInfo = info;
    } else {
        qCWarning(RUQOLA_LOG) << "info is invalid " << info;
    }
    if (o.contains("accessory"_L1)) {
        block.setBlockAccessory(BlockAccessory::deserialize(o["accessory"_L1].toObject()));
    }
    if (o.contains("elements"_L1)) {
        QList<BlockAction> blockActions;
        const QJsonArray elements = o["elements"_L1].toArray();
        const auto elementsCount = elements.count();
        blockActions.reserve(elementsCount);
        for (auto i = 0; i < elementsCount; ++i) {
            const BlockAction action = BlockAction::deserialize(elements.at(i).toObject());
            if (action.isValid()) {
                blockActions.append(std::move(action));
            } else {
                // qDebug() << "Invalid elements" << elements.at(i).toObject() << " action " << action;
                qCWarning(RUQOLA_LOG) << "Invalid elements" << elements.at(i).toObject();
            }
        }
        block.setBlockActions(blockActions);
    }
    return block;
}

QDebug operator<<(QDebug d, const Block &t)
{
    d.space() << "blockId" << t.blockId();
    d.space() << "callId" << t.callId();
    d.space() << "appId" << t.appId();
    d.space() << "mBlockType" << t.blockType();
    d.space() << "Video conf info" << t.videoConferenceInfo();
    d.space() << "Text Section" << t.sectionText();
    d.space() << "Block Actions" << t.blockActions();
    d.space() << "Block Accessary" << t.blockAccessory();
    return d;
}

#include "moc_block.cpp"
