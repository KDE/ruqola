/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ModerationUserDeleteReportedMessagesJob : public RestApiAbstractJob
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

    [[nodiscard]] QString userIdForMessages() const;
    void setUserIdForMessages(const QString &newUserIdForMessages);

Q_SIGNALS:
    void moderationUserDeleteReportedMessagesDone();

private:
    Q_DISABLE_COPY(ModerationUserDeleteReportedMessagesJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mUserIdForMessages;
};
}
