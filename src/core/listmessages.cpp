/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listmessages.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ListMessages, Ruqola_ListMessages)

#include "ruqola_debug.h"

ListMessages::ListMessages() = default;
ListMessages::~ListMessages() = default;

void ListMessages::parseMessages(const QJsonObject &messagesObj, QLatin1StringView arrayName)
{
    parseFirstPageCounters(messagesObj);
    parseMessagesList(messagesObj, arrayName);
}

void ListMessages::parseMessagesList(const QJsonObject &messagesObj, QLatin1StringView arrayName)
{
    const QJsonArray messagesArray = messagesObj[arrayName].toArray();
    mList.reserve(mList.count() + messagesArray.count());
    for (const QJsonValue &current : messagesArray) {
        if (current.type() == QJsonValue::Object) {
            mList.emplace_back().parseMessage(current.toObject(), true, nullptr);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing thread" << current;
        }
    }
}

QDebug operator<<(QDebug d, const ListMessages &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "MessagesCount" << t.loadedCount() << "\n";
    for (const Message &message : t.list()) {
        d.space() << message << "\n";
    }
    return d;
}
