/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationUserDeleteReportedMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationUserDeleteReportedMessagesJob(QObject *parent = nullptr);
    ~ModerationUserDeleteReportedMessagesJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QByteArray userIdForMessages() const;
    void setUserIdForMessages(const QByteArray &newUserIdForMessages);

Q_SIGNALS:
    void moderationUserDeleteReportedMessagesDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mUserIdForMessages;
};
}
