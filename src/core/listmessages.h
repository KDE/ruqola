/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include "messages/message.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT ListMessages
{
public:
    ListMessages();
    virtual ~ListMessages();
    void parseMessages(const QJsonObject &messagesObj, const QString &arrayName = QStringLiteral("messages"));

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    [[nodiscard]] int messagesCount() const;
    void setMessagesCount(int listMessagesCount);

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] Message at(int index) const;
    [[nodiscard]] QList<Message> listMessages() const;

protected:
    virtual void parseMessagesList(const QJsonObject &messagesObj, const QString &arrayName);
    QList<Message> mListMessages;

private:
    LIBRUQOLACORE_NO_EXPORT void parseListInfo(const QJsonObject &messagesObj);
    int mMessagesCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ListMessages)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ListMessages &t);
