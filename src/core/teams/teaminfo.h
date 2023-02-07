/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT TeamInfo
{
public:
    TeamInfo();
    ~TeamInfo() = default;

    void parseTeamInfo(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT QString teamId() const;
    void setTeamId(const QString &teamId);

    Q_REQUIRED_RESULT bool mainTeam() const;
    void setMainTeam(bool mainTeam);

    Q_REQUIRED_RESULT bool hasTeamRoom() const;

    static void serialize(const TeamInfo &teams, QJsonObject &obj);
    Q_REQUIRED_RESULT static TeamInfo deserialize(const QJsonObject &o);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT bool operator==(const TeamInfo &other) const;
    Q_REQUIRED_RESULT bool operator!=(const TeamInfo &other) const;

    Q_REQUIRED_RESULT bool autoJoin() const;
    void setAutoJoin(bool autoJoin);

    Q_REQUIRED_RESULT int roomsCount() const;
    void setRoomsCount(int newRoomsCount);

private:
    QString mTeamId;
    int mRoomsCount = 0;
    bool mMainTeam = false;
    bool mAutoJoin = false;
};

Q_DECLARE_METATYPE(TeamInfo)
Q_DECLARE_TYPEINFO(TeamInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TeamInfo &t);
