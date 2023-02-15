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
    Q_REQUIRED_RESULT bool operator==(const Block &other) const;

    static QJsonObject serialize(const Block &block);
    static Block deserialize(const QJsonObject &o);
    void parseBlock(const QJsonObject &block);

    Q_REQUIRED_RESULT QString blockId() const;
    void setBlockId(const QString &newBlockId);

    Q_REQUIRED_RESULT QString callId() const;
    void setCallId(const QString &newCallId);

    Q_REQUIRED_RESULT QString appId() const;
    void setAppId(const QString &newAppId);

    Q_REQUIRED_RESULT BlockType blockType() const;
    void setBlockType(BlockType newBlockType);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT QString generateDescription() const;

    Q_REQUIRED_RESULT QString blockTypeStr() const;
    void setBlockTypeStr(const QString &newBlockStr);

    Q_REQUIRED_RESULT VideoConferenceInfo videoConferenceInfo() const;
    void setVideoConferenceInfo(const VideoConferenceInfo &newInfo);

private:
    Q_REQUIRED_RESULT Block::BlockType convertBlockTypeToEnum(const QString &type);
    QString mBlockId;
    QString mCallId;
    QString mAppId;
    QString mBlockStr;
    BlockType mBlockType = Unknown;
    VideoConferenceInfo mVideoConferenceInfo;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Block &t);
Q_DECLARE_TYPEINFO(Block, Q_MOVABLE_TYPE);
