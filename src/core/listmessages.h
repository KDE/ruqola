/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"

#include "messages/message.h"
#include "paginatedinfolist.h"
class QDebug;

// Message::parseMessage() takes more than the element's json object, and a subclass may pick the
// elements apart differently, so the elements go through parseMessagesList() below rather than
// through PaginatedInfoList::parseElements().
class LIBRUQOLACORE_TESTS_EXPORT ListMessages : public PaginatedInfoList<Message, &Message::parseMessage>
{
public:
    ListMessages();
    virtual ~ListMessages();

    void parseMessages(const QJsonObject &messagesObj, QLatin1StringView arrayName = QLatin1StringView("messages"));

protected:
    virtual void parseMessagesList(const QJsonObject &messagesObj, QLatin1StringView arrayName);
};
QT_DECL_METATYPE_EXTERN_TAGGED(ListMessages, Ruqola_ListMessages, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ListMessages &t);
