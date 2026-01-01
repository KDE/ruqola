/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QList>
#include <QMetaType>
class QJsonObject;

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

    [[nodiscard]] QByteArray identifier() const;
    void setIdentifier(const QByteArray &identifier);
    [[nodiscard]] static QList<TeamRoom> parseTeamRooms(const QJsonObject &obj);

private:
    QString mName;
    QString mFname;
    QByteArray mIdentifier;
    bool mAutoJoin = false;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TeamRoom &t);
QT_DECL_METATYPE_EXTERN_TAGGED(TeamRoom, Ruqola_TeamRoom, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(TeamRoom, Q_RELOCATABLE_TYPE);
