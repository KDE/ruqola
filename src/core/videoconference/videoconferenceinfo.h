/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>

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

    Q_REQUIRED_RESULT QDateTime createdAtDateTime() const;
    void setCreatedAtDateTime(const QDateTime &newCreatedAtDateTime);

    Q_REQUIRED_RESULT QDateTime endedAtDateTime() const;
    void setEndedAtDateTime(const QDateTime &newEndedAtDateTime);

    Q_REQUIRED_RESULT VideoConferenceType conferenceType() const;
    void setConferenceType(VideoConferenceType newConferenceType);

private:
    Q_REQUIRED_RESULT VideoConferenceInfo::VideoConferenceType convertTypeToEnum(const QString &str) const;
    QDateTime mCreatedAtDateTime;
    QDateTime mEndedAtDateTime;
    QString mUrl;
    QString mRoomId;
    VideoConferenceType mConferenceType = Unknown;
    int mStatus = -1;
    bool mRinging = false;
};
Q_DECLARE_METATYPE(VideoConferenceInfo)
Q_DECLARE_TYPEINFO(VideoConferenceInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConferenceInfo &t);
