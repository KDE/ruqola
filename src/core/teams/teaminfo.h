/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString teamId() const;
    void setTeamId(const QString &teamId);

    [[nodiscard]] bool mainTeam() const;
    void setMainTeam(bool mainTeam);

    [[nodiscard]] bool hasTeamRoom() const;

    static void serialize(const TeamInfo &teams, QJsonObject &obj);
    [[nodiscard]] static TeamInfo deserialize(const QJsonObject &o);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] bool operator==(const TeamInfo &other) const;
    [[nodiscard]] bool operator!=(const TeamInfo &other) const;

    [[nodiscard]] bool autoJoin() const;
    void setAutoJoin(bool autoJoin);

    [[nodiscard]] int roomsCount() const;
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
