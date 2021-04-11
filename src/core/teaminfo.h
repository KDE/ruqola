/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

    static Q_REQUIRED_RESULT QJsonObject serialize(const TeamInfo &retention);
    static Q_REQUIRED_RESULT TeamInfo fromJSon(const QJsonObject &o);

    Q_REQUIRED_RESULT bool isValid() const;

private:
    QString mTeamId;
    bool mMainTeam = false;
};

Q_DECLARE_METATYPE(TeamInfo)
Q_DECLARE_TYPEINFO(TeamInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TeamInfo &t);
