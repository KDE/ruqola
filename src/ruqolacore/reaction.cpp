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
#include <KTextToHTML>
Reaction::Reaction()
{
}

QString Reaction::convertedReactionName() const
{
    QString str = mReactionName;
    const KTextToHTML::Options convertFlags = KTextToHTML::PreserveSpaces | KTextToHTML::HighlightText | KTextToHTML::ReplaceSmileys;
    str = KTextToHTML::convertToHtml(str, convertFlags);
    return str;
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

int Reaction::count() const
{
    return mUserNames.count();
}

bool Reaction::operator ==(const Reaction &other) const
{
    return (mUserNames == other.userNames()) && (mReactionName == other.reactionName());
}

QDebug operator <<(QDebug d, const Reaction &t)
{
    d << "ReactionName " << t.reactionName();
    d << "UserNames " << t.userNames();
    return d;
}
