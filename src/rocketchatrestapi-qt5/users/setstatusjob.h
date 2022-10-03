/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SetStatusJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SetStatusJob(QObject *parent = nullptr);
    ~SetStatusJob() override;
    enum StatusType { OnLine, Away, Offline, Busy, Unknown };

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString statusMessage() const;
    void setStatusMessage(const QString &statusMessage);

    Q_REQUIRED_RESULT StatusType status() const;
    void setStatus(StatusType status);

    Q_REQUIRED_RESULT QString statusUserId() const;
    void setStatusUserId(const QString &statusUserId);

Q_SIGNALS:
    void setStatusDone();

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(SetStatusJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mStatusUserId;
    QString mStatusMessage;
    StatusType mStatus = Unknown;
};
}
