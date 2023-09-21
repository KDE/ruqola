/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT TeamRoomCompleter
{
public:
    TeamRoomCompleter();
    [[nodiscard]] bool operator==(const TeamRoomCompleter &other) const;

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString fname() const;
    void setFname(const QString &fname);

    void parse(const QJsonObject &obj);

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &identifier);

private:
    QString mName;
    QString mFname;
    QString mIdentifier;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TeamRoomCompleter &t);
Q_DECLARE_METATYPE(TeamRoomCompleter)
Q_DECLARE_TYPEINFO(TeamRoomCompleter, Q_MOVABLE_TYPE);
