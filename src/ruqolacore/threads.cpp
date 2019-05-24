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
#include "threads.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

Threads::Threads()
{
}

QVector<Thread> Threads::threads() const
{
    return mThreads;
}

void Threads::setThreads(const QVector<Thread> &threads)
{
    mThreads = threads;
}

void Threads::parseThreads(const QJsonObject &threadsObj)
{
    mThreadsCount = threadsObj[QStringLiteral("count")].toInt();
    mOffset = threadsObj[QStringLiteral("offset")].toInt();
    mTotal = threadsObj[QStringLiteral("total")].toInt();
    const QJsonArray threadsArray = threadsObj[QStringLiteral("threads")].toArray();
    mThreads.clear();
    mThreads.reserve(threadsArray.count());
    for (const QJsonValue &current : threadsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject discussionObject = current.toObject();
            Thread m;
            m.parseMessage(discussionObject);
            mThreads.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing thread" << current;
        }
    }
}

bool Threads::isEmpty() const
{
    return mThreads.isEmpty();
}

void Threads::clear()
{
    mThreads.clear();
}

int Threads::count() const
{
    return mThreads.count();
}

Thread Threads::at(int index) const
{
    return mThreads.at(index);
}

int Threads::threadsCount() const
{
    return mThreadsCount;
}

void Threads::setThreadsCount(int threadsCount)
{
    mThreadsCount = threadsCount;
}

int Threads::offset() const
{
    return mOffset;
}

void Threads::setOffset(int offset)
{
    mOffset = offset;
}

int Threads::total() const
{
    return mTotal;
}

void Threads::setTotal(int total)
{
    mTotal = total;
}

QDebug operator <<(QDebug d, const Threads &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "threadsCount " << t.threadsCount();
    for (int i = 0, total = t.threads().count(); i < total; ++i) {
        d << t.threads().at(i);
    }
    return d;
}

void Threads::append(const Thread &thread)
{
    mThreads.append(thread);
}
