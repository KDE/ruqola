/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#ifndef MENTION_H
#define MENTION_H
#include "libruqola_private_export.h"
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT Mention
{
    Q_GADGET
//    Q_PROPERTY(int count READ count CONSTANT)
//    Q_PROPERTY(QString reactionName READ reactionName CONSTANT)
//    Q_PROPERTY(QString convertedReactionName READ convertedReactionName CONSTANT)
public:
    Mention();

//    Q_REQUIRED_RESULT QString reactionName() const;
//    void setReactionName(const QString &reactionName);

//    Q_REQUIRED_RESULT QStringList userNames() const;
//    void setUserNames(const QStringList &userNames);

//    Q_REQUIRED_RESULT int count() const;

//    Q_REQUIRED_RESULT bool operator ==(const Reaction &other) const;

//    Q_REQUIRED_RESULT QString convertedReactionName() const;
private:
//    QString mReactionName;
//    QStringList mUserNames;
};
Q_DECLARE_METATYPE(Mention)
Q_DECLARE_TYPEINFO(Mention, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Mention &t);

#endif // MENTION_H
