/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT CustomUserStatusUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT StatusUpdateInfo {
        QString identifier;
        QString name;
        QString statusType;
        [[nodiscard]] bool isValid() const
        {
            return !name.isEmpty() && !statusType.isEmpty() && !identifier.isEmpty();
        }
    };
    explicit CustomUserStatusUpdateJob(QObject *parent = nullptr);
    ~CustomUserStatusUpdateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] StatusUpdateInfo statusUpdateInfo() const;
    void setStatusUpdateInfo(const StatusUpdateInfo &statusUpdateInfo);

Q_SIGNALS:
    void customUserUpdateDone();

private:
    Q_DISABLE_COPY(CustomUserStatusUpdateJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    StatusUpdateInfo mStatusUpdateInfo;
};
}
