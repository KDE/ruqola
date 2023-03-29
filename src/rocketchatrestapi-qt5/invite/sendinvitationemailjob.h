/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SendInvitationEmailJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SendInvitationEmailJob(QObject *parent = nullptr);
    ~SendInvitationEmailJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QStringList emails() const;
    void setEmails(const QStringList &newEmails);

Q_SIGNALS:
    void sendInvitationEmailsDone();

private:
    Q_DISABLE_COPY(SendInvitationEmailJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QStringList mEmails;
};
}
