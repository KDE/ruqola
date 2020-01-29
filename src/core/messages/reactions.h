/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef REACTIONS_H
#define REACTIONS_H

#include "libruqolacore_export.h"
#include "reaction.h"
#include <QVector>
class EmojiManager;
class LIBRUQOLACORE_EXPORT Reactions
{
public:
    Reactions();

    void setReactions(const QVector<Reaction> &reactions);
    Q_REQUIRED_RESULT QVector<Reaction> reactions() const;

    void parseReactions(const QJsonObject &array, EmojiManager *emojiManager = nullptr);

    Q_REQUIRED_RESULT bool operator ==(const Reactions &other) const;

    Q_REQUIRED_RESULT static QJsonObject serialize(const Reactions &reactions);
    Q_REQUIRED_RESULT static Reactions fromJSon(const QJsonObject &o);

    Q_REQUIRED_RESULT bool isEmpty() const;

private:
    QVector<Reaction> mReactions;
};
Q_DECLARE_METATYPE(Reactions)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Reactions &t);

#endif // REACTIONS_H
