/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "blockaccessory.h"
#include "blockaction.h"
#include "libruqolacore_export.h"
#include "videoconference/videoconferenceinfo.h"
// packages/ui-kit/src/surfaces/message/UiKitParserMessage.ts
class LIBRUQOLACORE_EXPORT Block
{
    Q_GADGET
public:
    enum class BlockType : uint8_t {
        Unknown,
        VideoConf,
        Actions,
        Section,
        Context,
        Divider,
        Overflow,
        Image,
        Preview,
        Callout,
    };
    Q_ENUM(BlockType)

    Block();
    ~Block();
    [[nodiscard]] bool operator==(const Block &other) const;

    [[nodiscard]] static QJsonObject serialize(const Block &block);
    [[nodiscard]] static Block deserialize(const QJsonObject &o);
    void parseBlock(const QJsonObject &block);

    [[nodiscard]] QString sectionText() const;
    void setSectionText(const QString &newSectionText);

    [[nodiscard]] QString blockId() const;
    void setBlockId(const QString &newBlockId);

    [[nodiscard]] QString callId() const;
    void setCallId(const QString &newCallId);

    [[nodiscard]] QString appId() const;
    void setAppId(const QString &newAppId);

    [[nodiscard]] BlockType blockType() const;
    void setBlockType(BlockType newBlockType);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QString title() const;

    [[nodiscard]] VideoConferenceInfo videoConferenceInfo() const;
    void setVideoConferenceInfo(const VideoConferenceInfo &newInfo);

    [[nodiscard]] QList<BlockAction> blockActions() const;
    void setBlockActions(const QList<BlockAction> &newBlockActions);

    [[nodiscard]] BlockAccessory blockAccessory() const;
    void setBlockAccessory(const BlockAccessory &newBlockAccessory);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QString convertEnumToStr(BlockType newBlockType);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static Block::BlockType convertBlockTypeToEnum(const QString &type);
    QList<BlockAction> mBlockActions;
    QString mBlockId;
    QString mCallId;
    QString mAppId;
    QString mSectionText;
    BlockType mBlockType = BlockType::Unknown;
    VideoConferenceInfo mVideoConferenceInfo;
    BlockAccessory mBlockAccessory;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Block &t);
Q_DECLARE_TYPEINFO(Block, Q_RELOCATABLE_TYPE);
