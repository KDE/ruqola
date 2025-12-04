/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT TeamCompleter
{
public:
    TeamCompleter();
    [[nodiscard]] bool operator==(const TeamCompleter &other) const;

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString fname() const;
    void setFname(const QString &fname);

    void parse(const QJsonObject &obj);

    [[nodiscard]] QByteArray teamId() const;
    void setTeamId(const QByteArray &identifier);

private:
    QString mName;
    QString mFname;
    QByteArray mTeamId;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TeamCompleter &t);
QT_DECL_METATYPE_EXTERN_TAGGED(TeamCompleter, Ruqola_TeamCompleter, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(TeamCompleter, Q_RELOCATABLE_TYPE);
