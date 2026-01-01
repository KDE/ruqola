/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "messagesmodel.h"

class LIBRUQOLACORE_TESTS_EXPORT ThreadMessageModel : public MessagesModel
{
    Q_OBJECT
public:
    explicit ThreadMessageModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~ThreadMessageModel() override;

    [[nodiscard]] QByteArray threadMessageId() const;
    void setThreadMessageId(const QByteArray &threadMessageId);

    void parseThreadMessages(const QJsonObject &obj);
    void loadMoreThreadMessages(const QJsonObject &obj);

    [[nodiscard]] int total() const;
    void setTotal(int total);
    void setPreviewMessage(const Message &message);

private:
    LIBRUQOLACORE_NO_EXPORT void parse(const QJsonObject &obj);
    Message mPreviewMessage;
    QByteArray mThreadMessageId;
    int mTotal = 0;
};
