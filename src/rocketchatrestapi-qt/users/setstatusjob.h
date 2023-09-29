/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT SetStatusJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SetStatusJob(QObject *parent = nullptr);
    ~SetStatusJob() override;
    enum StatusType { OnLine, Away, Offline, Busy, Unknown };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString statusMessage() const;
    void setStatusMessage(const QString &statusMessage);

    [[nodiscard]] StatusType status() const;
    void setStatus(StatusType status);

    [[nodiscard]] QString statusUserId() const;
    void setStatusUserId(const QString &statusUserId);

Q_SIGNALS:
    void setStatusDone();

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(SetStatusJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mStatusUserId;
    QString mStatusMessage;
    StatusType mStatus = Unknown;
};
}
