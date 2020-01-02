/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>
Mentions::Mentions()
{
}

void Mentions::setMentions(const QVector<Mention> &mentions)
{
    mMentions = mentions;
}

void Mentions::clear()
{
    mMentions.clear();
}

int Mentions::count() const
{
    return mMentions.count();
}

int Mentions::mentionsCount() const
{
    return mMentionsCount;
}

void Mentions::setMentionsCount(int mentionsCount)
{
    mMentionsCount = mentionsCount;
}

int Mentions::offset() const
{
    return mOffset;
}

void Mentions::setOffset(int offset)
{
    mOffset = offset;
}

int Mentions::total() const
{
    return mTotal;
}

void Mentions::setTotal(int total)
{
    mTotal = total;
}

QVector<Mention> Mentions::mentions() const
{
    return mMentions;
}

void Mentions::parseMoreMentions(const QJsonObject &mentionsObj)
{
    const int mentionsCount = mentionsObj[QStringLiteral("count")].toInt();
    mOffset = mentionsObj[QStringLiteral("offset")].toInt();
    mTotal = mentionsObj[QStringLiteral("total")].toInt();
    parseMentionsObj(mentionsObj);
    mMentionsCount += mentionsCount;
}

void Mentions::parseMentions(const QJsonObject &mentions)
{
    mMentions.clear();
    mMentionsCount = mentions[QStringLiteral("count")].toInt();
    mOffset = mentions[QStringLiteral("offset")].toInt();
    mTotal = mentions[QStringLiteral("total")].toInt();
    mMentions.reserve(mMentionsCount);
    parseMentionsObj(mentions);
}

void Mentions::parseMentionsObj(const QJsonObject &discussionsObj)
{
    const QJsonArray discussionsArray = discussionsObj[QStringLiteral("mentions")].toArray();
    for (const QJsonValue &current : discussionsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject mentionsObject = current.toObject();
            Mention m;
            m.parseMessage(mentionsObject, true);
            mMentions.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing mentions" << current;
        }
    }
}

bool Mentions::operator ==(const Mentions &other) const
{
    return mMentions == other.mentions();
}

QDebug operator <<(QDebug d, const Mentions &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "mentionsCount " << t.mentionsCount();
    for (int i = 0, total = t.mentions().count(); i < total; ++i) {
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
//    QVector<Mention> mentions;
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

Mention Mentions::at(int index) const
{
    return mMentions.at(index);
}
