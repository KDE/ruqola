/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] bool operator==(const VideoConference &other) const;

    void parseVideoConference(const QJsonObject &content);
    [[nodiscard]] QString callId() const;
    void setCallId(const QString &newCallId);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &newUserId);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] Action action() const;
    void setAction(Action newAction);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT VideoConference::Action convertActionToEnum(const QString &str);
    QString mCallId;
    QString mRoomId;
    QString mUserId;
    Action mAction = Unknown;
};
Q_DECLARE_TYPEINFO(VideoConference, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConference &t);
