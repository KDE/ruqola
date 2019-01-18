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

#include "mentions.h"
#include <QJsonArray>
#include <QJsonObject>
Mentions::Mentions()
{

}

void Mentions::setMentions(const QVector<Mention> &mentions)
{
    mMentions = mentions;
}

QVector<Mention> Mentions::mentions() const
{
    return mMentions;
}

void Mentions::parseMentions(const QJsonObject &reacts)
{
    mMentions.clear();
//    const QStringList lst = reacts.keys();
//    QStringList users;
//    for (const QString &str : lst) {
//        users.clear();
//        const QJsonObject obj = reacts.value(str).toObject();
//        QJsonValue usernames = obj.value(QLatin1String("usernames"));
//        if (!usernames.isUndefined()) {
//            QJsonArray array = usernames.toArray();
//            for (int i = 0; i < array.count(); ++i) {
//                users.append(array.at(i).toString());
//            }
//        }
//        if (!users.isEmpty()) {
//            Mention r;
//            r.setMentionName(str);
//            r.setUserNames(users);
//            mMentions.append(r);
//        }
//    }
}

bool Mentions::operator ==(const Mentions &other) const
{
    return mMentions == other.mentions();
}

QDebug operator <<(QDebug d, const Mentions &t)
{
    for (int i = 0; i < t.mentions().count(); i++) {
        d << t.mentions().at(i);
    }
    return d;
}

QJsonObject Mentions::serialize(const Mentions &mentions)
{
    QJsonObject obj;
//    for (int i = 0; i < mentions.mentions().count(); ++i) {
//        QJsonObject react;
//        react[QLatin1String("usernames")] = QJsonArray::fromStringList(mentions.mentions().at(i).userNames());
//        obj[mentions.mentions().at(i).mentionName()] = react;
//    }
    return obj;
}

Mentions Mentions::fromJSon(const QJsonObject &o)
{
//    QVector<Mention> reacts;
//    const QStringList lst = o.keys();
//    QStringList users;
//    for (const QString &str : lst) {
//        const QJsonObject obj = o.value(str).toObject();
//        QJsonValue usernames = obj.value(QLatin1String("usernames"));
//        if (!usernames.isUndefined()) {
//            QJsonArray array = usernames.toArray();
//            for (int i = 0; i < array.count(); ++i) {
//                users.append(array.at(i).toString());
//            }
//        }
//        if (!users.isEmpty()) {
//            Mention r;
//            r.setMentionName(str);
//            r.setUserNames(users);
//            reacts.append(r);
//        }
//        users.clear();
//    }
//    Mentions final;
//    final.setMentions(reacts);
//    return final;
    return {};
}

bool Mentions::isEmpty() const
{
    return mMentions.isEmpty();
}

