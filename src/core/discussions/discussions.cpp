/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussions/discussions.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(Discussions, Ruqola_Discussions)

#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Discussions::Discussions() = default;

QList<Discussion> Discussions::discussions() const
{
    return mDiscussion;
}

void Discussions::append(const Discussion &discussion)
{
    mDiscussion.append(discussion);
}

void Discussions::setDiscussions(const QList<Discussion> &discussion)
{
    mDiscussion = discussion;
}

void Discussions::parseDiscussions(const QJsonObject &discussionsObj)
{
    mDiscussion.clear();
    mDiscussionsCount = discussionsObj["count"_L1].toInt();
    mOffset = discussionsObj["offset"_L1].toInt();
    mTotal = discussionsObj["total"_L1].toInt();
    mDiscussion.reserve(mDiscussionsCount);
    parseDiscussionsObj(discussionsObj);
}

void Discussions::parseDiscussionsObj(const QJsonObject &discussionsObj)
{
    const QJsonArray discussionsArray = discussionsObj["discussions"_L1].toArray();
    for (const QJsonValue &current : discussionsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject discussionObject = current.toObject();
            Discussion m;
            m.parseDiscussion(discussionObject);
            mDiscussion.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing discussions" << current;
        }
    }
}

void Discussions::parseMoreDiscussions(const QJsonObject &discussionsObj)
{
    const int discussionsCount = discussionsObj["count"_L1].toInt();
    mOffset = discussionsObj["offset"_L1].toInt();
    mTotal = discussionsObj["total"_L1].toInt();
    parseDiscussionsObj(discussionsObj);
    mDiscussionsCount += discussionsCount;
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
    if (index < 0 || index >= mDiscussion.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
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

QDebug operator<<(QDebug d, const Discussions &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "discussionsCount" << t.discussionsCount() << "\n";
    for (int i = 0, total = t.discussions().count(); i < total; ++i) {
        d.space() << t.discussions().at(i) << "\n";
    }
    return d;
}
