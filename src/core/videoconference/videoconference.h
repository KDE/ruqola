/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT VideoConference
{
    Q_GADGET
public:
    enum Action {
        Unknown,
        IncomingCall,
        Canceled,
        Confirmed,
        Accepted,
        Rejected,
    };
    Q_ENUM(Action)

    VideoConference();
    ~VideoConference();

    Q_REQUIRED_RESULT bool operator==(const VideoConference &other) const;

    void parseVideoConference(const QJsonObject &content);
    Q_REQUIRED_RESULT QString callId() const;
    void setCallId(const QString &newCallId);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &newRoomId);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &newUserId);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT Action action() const;
    void setAction(Action newAction);

private:
    Q_REQUIRED_RESULT VideoConference::Action convertActionToEnum(const QString &str);
    QString mCallId;
    QString mRoomId;
    QString mUserId;
    Action mAction = Unknown;
};
Q_DECLARE_METATYPE(VideoConference)
Q_DECLARE_TYPEINFO(VideoConference, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConference &t);
