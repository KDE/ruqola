/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SendInvitationEmailJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SendInvitationEmailJob(QObject *parent = nullptr);
    ~SendInvitationEmailJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QStringList emails() const;
    void setEmails(const QStringList &newEmails);

Q_SIGNALS:
    void sendInvitationEmailsDone();

private:
    Q_DISABLE_COPY(SendInvitationEmailJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QStringList mEmails;
};
}
