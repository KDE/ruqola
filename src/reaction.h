/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef REACTION_H
#define REACTION_H

#include "libruqola_private_export.h"
#include <QObject>
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_TESTS_EXPORT Reaction
{
    Q_GADGET
public:
    Reaction();
    void parseReaction(const QJsonObject &obj);

    QString reactionName() const;
    void setReactionName(const QString &reactionName);

    QStringList userNames() const;
    void setUserNames(const QStringList &userNames);

private:
    QString mReactionName;
    QStringList mUserNames;
};
Q_DECLARE_METATYPE(Reaction)
Q_DECLARE_TYPEINFO(Reaction, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Reaction &t);
#endif // REACTION_H
