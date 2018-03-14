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

#include "reactions.h"
#include <QJsonArray>

Reactions::Reactions()
{

}

Reactions::~Reactions()
{

}

void Reactions::setReactions(const QVector<Reaction> &reactions)
{
    mReactions = reactions;
}

QVector<Reaction> Reactions::reactions() const
{
    return mReactions;
}

void Reactions::parseReactions(const QJsonObject &reacts)
{
    mReactions.clear();
    const QStringList lst = reacts.keys();
    QStringList users;
    for (const QString &str : lst) {
        const QJsonObject obj = reacts.value(str).toObject();
        QJsonValue usernames = obj.value(QLatin1String("usernames"));
        if (!usernames.isUndefined()) {
            QJsonArray array = usernames.toArray();
            for (int i = 0; i < array.count(); ++i) {
                users.append(array.at(i).toString());
            }

        }
        if (!users.isEmpty()) {
            Reaction r;
            r.setReactionName(str);
            r.setUserNames(users);
            mReactions.append(r);
        }
        users.clear();
    }
}

bool Reactions::operator ==(const Reactions &other) const
{
    return mReactions == other.reactions();
}

QDebug operator <<(QDebug d, const Reactions &t)
{
    for (int i = 0; i < t.reactions().count(); i++) {
        d << t.reactions().at(i);
    }
    return d;
}
