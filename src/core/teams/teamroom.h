/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_EXPORT TeamRoom
{
public:
    TeamRoom();
    [[nodiscard]] bool operator==(const TeamRoom &other) const;

    [[nodiscard]] bool autoJoin() const;
    void setAutoJoin(bool autoJoin);

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString fname() const;
    void setFname(const QString &fname);

    void parse(const QJsonObject &obj);

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &identifier);
    [[nodiscard]] static QVector<TeamRoom> parseTeamRooms(const QJsonObject &obj);

private:
    QString mName;
    QString mFname;
    QString mIdentifier;
    bool mAutoJoin = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TeamRoom &t);
Q_DECLARE_METATYPE(TeamRoom)
Q_DECLARE_TYPEINFO(TeamRoom, Q_RELOCATABLE_TYPE);
