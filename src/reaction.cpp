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

#include "reaction.h"

Reaction::Reaction()
{

}

void Reaction::parseReaction(const QJsonObject &obj)
{

}

QString Reaction::reactionName() const
{
    return mReactionName;
}

void Reaction::setReactionName(const QString &reactionName)
{
    mReactionName = reactionName;
}

QStringList Reaction::userNames() const
{
    return mUserNames;
}

void Reaction::setUserNames(const QStringList &userNames)
{
    mUserNames = userNames;
}

QDebug operator <<(QDebug d, const Reaction &t)
{
    d << "ReactionName " << t.reactionName();
    d << "UserNames " << t.userNames();
    return d;
}
