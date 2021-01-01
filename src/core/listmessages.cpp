/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#include "listmessages.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

ListMessages::ListMessages()
{
}

void ListMessages::parseMessages(const QJsonObject &messagesObj, const QString &arrayName)
{
    mMessagesCount = messagesObj[QStringLiteral("count")].toInt();
    mOffset = messagesObj[QStringLiteral("offset")].toInt();
    mTotal = messagesObj[QStringLiteral("total")].toInt();
    const QJsonArray messagesArray = messagesObj[arrayName.isEmpty() ? QStringLiteral("messages") : arrayName].toArray();
    mListMessages.clear();
    mListMessages.reserve(messagesArray.count());
    for (const QJsonValue &current : messagesArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject messageObject = current.toObject();
            Message m;
            m.parseMessage(messageObject, true);
            mListMessages.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing thread" << current;
        }
    }
}

int ListMessages::offset() const
{
    return mOffset;
}

void ListMessages::setOffset(int offset)
{
    mOffset = offset;
}

int ListMessages::total() const
{
    return mTotal;
}

void ListMessages::setTotal(int total)
{
    mTotal = total;
}

int ListMessages::messagesCount() const
{
    return mMessagesCount;
}

void ListMessages::setMessagesCount(int count)
{
    mMessagesCount = count;
}

bool ListMessages::isEmpty() const
{
    return mListMessages.isEmpty();
}

void ListMessages::clear()
{
    mListMessages.clear();
}

int ListMessages::count() const
{
    return mListMessages.count();
}

Message ListMessages::at(int index) const
{
    return mListMessages.at(index);
}

QVector<Message> ListMessages::listMessages() const
{
    return mListMessages;
}

QDebug operator <<(QDebug d, const ListMessages &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "threadMessagesCount " << t.messagesCount() << "\n";
    for (int i = 0, total = t.count(); i < total; ++i) {
        d << t.at(i) << "\n";
    }
    return d;
}
