/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

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
    void parseMessages(const QJsonObject &messagesObj, const QString &arrayName = QStringLiteral("messages"));

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    Q_REQUIRED_RESULT int messagesCount() const;
    void setMessagesCount(int listMessagesCount);

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT Message at(int index) const;
    Q_REQUIRED_RESULT QVector<Message> listMessages() const;

private:
    QVector<Message> mListMessages;
    int mMessagesCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ListMessages)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ListMessages &t);
