/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "threadmessages.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

ThreadMessages::ThreadMessages()
{

}

void ThreadMessages::parseThreadMessages(const QJsonObject &threadsObj)
{
    mThreadMessagesCount = threadsObj[QStringLiteral("count")].toInt();
    mOffset = threadsObj[QStringLiteral("offset")].toInt();
    mTotal = threadsObj[QStringLiteral("total")].toInt();
    const QJsonArray threadsArray = threadsObj[QStringLiteral("threads")].toArray();
    /*
    mThreads.clear();
    mThreads.reserve(threadsArray.count());
    for (const QJsonValue &current : threadsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject discussionObject = current.toObject();
            Thread m;
            m.parseMessage(discussionObject, true);
            mThreads.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing thread" << current;
        }
    }
    */
}

int ThreadMessages::offset() const
{
    return mOffset;
}

void ThreadMessages::setOffset(int offset)
{
    mOffset = offset;
}

int ThreadMessages::total() const
{
    return mTotal;
}

void ThreadMessages::setTotal(int total)
{
    mTotal = total;
}

int ThreadMessages::threadMessagesCount() const
{
    return mThreadMessagesCount;
}

void ThreadMessages::setThreadMessagesCount(int threadMessagesCount)
{
    mThreadMessagesCount = threadMessagesCount;
}

QDebug operator <<(QDebug d, const ThreadMessages &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "threadMessagesCount " << t.threadMessagesCount();
//    for (int i = 0, total = t.threads().count(); i < total; ++i) {
//        d << t.threads().at(i);
//    }
    return d;
}
