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
    [[nodiscard]] QByteArray callId() const;
    void setCallId(const QByteArray &newCallId);

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &newUserId);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] Action action() const;
    void setAction(Action newAction);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT VideoConference::Action convertActionToEnum(const QString &str);
    QByteArray mCallId;
    QByteArray mRoomId;
    QByteArray mUserId;
    Action mAction = Unknown;
};
Q_DECLARE_TYPEINFO(VideoConference, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConference &t);
