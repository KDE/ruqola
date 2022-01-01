/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "messagemodel.h"

class LIBRUQOLACORE_TESTS_EXPORT ThreadMessageModel : public MessageModel
{
    Q_OBJECT
public:
    explicit ThreadMessageModel(const QString &roomID = QStringLiteral("no_room"),
                                RocketChatAccount *account = nullptr,
                                Room *room = nullptr,
                                QObject *parent = nullptr);
    ~ThreadMessageModel() override;

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    void parseThreadMessages(const QJsonObject &obj);
    void loadMoreThreadMessages(const QJsonObject &obj);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

private:
    void parse(const QJsonObject &obj);
    QString mThreadMessageId;
    int mTotal = 0;
};

