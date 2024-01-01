/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationDismissReportsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationDismissReportsJob(QObject *parent = nullptr);
    ~ModerationDismissReportsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString userIdForMessages() const;
    void setUserIdForMessages(const QString &newUserIdForMessages);

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &newMessageId);

Q_SIGNALS:
    void moderationDismissReportsDone();

private:
    Q_DISABLE_COPY(ModerationDismissReportsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mUserIdForMessages;
    QString mMessageId;
};
}
