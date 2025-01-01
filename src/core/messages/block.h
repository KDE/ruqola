/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "blockaction.h"
#include "libruqolacore_export.h"
#include "videoconference/videoconferenceinfo.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT Block
{
    Q_GADGET
public:
    enum class BlockType : uint8_t {
        Unknown,
        VideoConf,
        Actions,
        Section,
    };
    Q_ENUM(BlockType)

    Block();
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

    [[nodiscard]] QString blockTypeStr() const;
    void setBlockTypeStr(const QString &newBlockStr);

    [[nodiscard]] VideoConferenceInfo videoConferenceInfo() const;
    void setVideoConferenceInfo(const VideoConferenceInfo &newInfo);

    [[nodiscard]] QList<BlockAction> blockActions() const;
    void setBlockActions(const QList<BlockAction> &newBlockActions);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString convertEnumToStr(BlockType newBlockType) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Block::BlockType convertBlockTypeToEnum(const QString &type);
    QList<BlockAction> mBlockActions;
    QString mBlockId;
    QString mCallId;
    QString mAppId;
    QString mBlockStr;
    QString mSectionText;
    BlockType mBlockType = BlockType::Unknown;
    VideoConferenceInfo mVideoConferenceInfo;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Block &t);
Q_DECLARE_TYPEINFO(Block, Q_RELOCATABLE_TYPE);
