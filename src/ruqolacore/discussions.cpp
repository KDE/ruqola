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

#include "discussions.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

Discussions::Discussions()
{
}

QVector<Discussion> Discussions::discussions() const
{
    return mDiscussion;
}

void Discussions::append(const Discussion &discussion)
{
    mDiscussion.append(discussion);
}

void Discussions::setDiscussions(const QVector<Discussion> &discussion)
{
    mDiscussion = discussion;
}

void Discussions::parseDiscussions(const QJsonObject &discussionsObj)
{
    mDiscussion.clear();
    mDiscussionsCount = discussionsObj[QStringLiteral("count")].toInt();
    mOffset = discussionsObj[QStringLiteral("offset")].toInt();
    mTotal = discussionsObj[QStringLiteral("total")].toInt();
    const QJsonArray discussionsArray = discussionsObj[QStringLiteral("discussions")].toArray();
    mDiscussion.reserve(discussionsArray.count());
    for (const QJsonValue &current : discussionsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject discussionObject = current.toObject();
            Discussion m;
            m.parseDiscussion(discussionObject);
            mDiscussion.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing mentions" << current;
        }
    }
}

bool Discussions::isEmpty() const
{
    return mDiscussion.isEmpty();
}

void Discussions::clear()
{
    mDiscussion.clear();
}

int Discussions::count() const
{
    return mDiscussion.count();
}

Discussion Discussions::at(int index) const
{
    return mDiscussion.at(index);
}

int Discussions::discussionsCount() const
{
    return mDiscussionsCount;
}

void Discussions::setDiscussionsCount(int discussionsCount)
{
    mDiscussionsCount = discussionsCount;
}

int Discussions::offset() const
{
    return mOffset;
}

void Discussions::setOffset(int offset)
{
    mOffset = offset;
}

int Discussions::total() const
{
    return mTotal;
}

void Discussions::setTotal(int total)
{
    mTotal = total;
}

QDebug operator <<(QDebug d, const Discussions &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "discussionsCount " << t.discussionsCount();
    for (int i = 0, total = t.discussions().count(); i < total; ++i) {
        d << t.discussions().at(i);
    }
    return d;
}
