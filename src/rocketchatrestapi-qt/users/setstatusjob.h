/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SetStatusJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SetStatusJob(QObject *parent = nullptr);
    ~SetStatusJob() override;
    enum class StatusType : uint8_t {
        OnLine,
        Away,
        Offline,
        Busy,
        Unknown,
    };

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
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mStatusUserId;
    QString mStatusMessage;
    StatusType mStatus = StatusType::Unknown;
};
}
