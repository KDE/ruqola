/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listmessages.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

ListMessages::ListMessages() = default;
ListMessages::~ListMessages() = default;

void ListMessages::parseListInfo(const QJsonObject &messagesObj)
{
    mMessagesCount = messagesObj[QLatin1String("count")].toInt();
    mOffset = messagesObj[QLatin1String("offset")].toInt();
    mTotal = messagesObj[QLatin1String("total")].toInt();
    mListMessages.clear();
}

void ListMessages::parseMessagesList(const QJsonObject &messagesObj, const QString &arrayName)
{
    const QJsonArray messagesArray = messagesObj[arrayName.isEmpty() ? QStringLiteral("messages") : arrayName].toArray();
    mListMessages.reserve(messagesArray.count());
    for (const QJsonValue &current : messagesArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject messageObject = current.toObject();
            Message m;
            m.parseMessage(messageObject, true);
            mListMessages.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing thread" << current;
        }
    }
}

void ListMessages::parseMessages(const QJsonObject &messagesObj, const QString &arrayName)
{
    parseListInfo(messagesObj);
    parseMessagesList(messagesObj, arrayName);
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
    if (index < 0 || index > mListMessages.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return Message();
    }
    return mListMessages.at(index);
}

QVector<Message> ListMessages::listMessages() const
{
    return mListMessages;
}

QDebug operator<<(QDebug d, const ListMessages &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "threadMessagesCount " << t.messagesCount() << "\n";
    for (int i = 0, total = t.count(); i < total; ++i) {
        d << t.at(i) << "\n";
    }
    return d;
}
