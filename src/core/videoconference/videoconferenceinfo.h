/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "user.h"
#include <QDateTime>
#include <QDebug>
#include <QVector>

class LIBRUQOLACORE_EXPORT VideoConferenceInfo
{
    Q_GADGET
public:
    enum VideoConferenceType {
        Unknown,
        Direct,
        Conference,
    };
    Q_ENUM(VideoConferenceType)

    VideoConferenceInfo();
    ~VideoConferenceInfo();
    void parse(const QJsonObject &content);

    Q_REQUIRED_RESULT QString url() const;
    void setUrl(const QString &newUrl);

    Q_REQUIRED_RESULT int status() const;
    void setStatus(int newStatus);

    Q_REQUIRED_RESULT bool ringing() const;
    void setRinging(bool newRinging);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &newRoomId);

    Q_REQUIRED_RESULT qint64 createdAtDateTime() const;
    void setCreatedAtDateTime(qint64 newCreatedAtDateTime);

    Q_REQUIRED_RESULT qint64 endedAtDateTime() const;
    void setEndedAtDateTime(qint64 newEndedAtDateTime);

    Q_REQUIRED_RESULT VideoConferenceType conferenceType() const;
    void setConferenceType(VideoConferenceType newConferenceType);

    Q_REQUIRED_RESULT QString providerName() const;
    void setProviderName(const QString &newProviderName);

    Q_REQUIRED_RESULT bool operator==(const VideoConferenceInfo &other) const;

    Q_REQUIRED_RESULT QVector<User> users() const;
    void setUsers(const QVector<User> &newUsers);

    Q_REQUIRED_RESULT QString statusInformation() const;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &newMessageId);

    Q_REQUIRED_RESULT QString blockId() const;
    void setBlockId(const QString &newBlockId);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT bool canJoin() const;

private:
    Q_REQUIRED_RESULT VideoConferenceInfo::VideoConferenceType convertTypeToEnum(const QString &str) const;
    QVector<User> mUsers;
    QString mBlockId;
    QString mUrl;
    QString mRoomId;
    QString mProviderName;
    QString mMessageId;
    VideoConferenceType mConferenceType = Unknown;
    qint64 mCreatedAtDateTime = -1;
    qint64 mEndedAtDateTime = -1;
    int mStatus = -1;
    bool mRinging = false;
};
Q_DECLARE_METATYPE(VideoConferenceInfo)
Q_DECLARE_TYPEINFO(VideoConferenceInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConferenceInfo &t);
