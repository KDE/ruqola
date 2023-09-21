/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "videoconference/videoconferenceinfo.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT Block
{
    Q_GADGET
public:
    enum BlockType {
        Unknown,
        VideoConf,
    };
    Q_ENUM(BlockType)

    Block();
    [[nodiscard]] bool operator==(const Block &other) const;

    static QJsonObject serialize(const Block &block);
    static Block deserialize(const QJsonObject &o);
    void parseBlock(const QJsonObject &block);

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

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString convertEnumToStr(BlockType newBlockType) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Block::BlockType convertBlockTypeToEnum(const QString &type);
    QString mBlockId;
    QString mCallId;
    QString mAppId;
    QString mBlockStr;
    BlockType mBlockType = Unknown;
    VideoConferenceInfo mVideoConferenceInfo;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Block &t);
Q_DECLARE_TYPEINFO(Block, Q_MOVABLE_TYPE);
