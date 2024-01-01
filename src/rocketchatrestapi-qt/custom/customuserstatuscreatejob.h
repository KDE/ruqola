/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT CustomUserStatusCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT StatusCreateInfo {
        QString name;
        QString statusType;
        [[nodiscard]] bool isValid() const
        {
            return !name.isEmpty() && !statusType.isEmpty();
        }
    };
    explicit CustomUserStatusCreateJob(QObject *parent = nullptr);
    ~CustomUserStatusCreateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
    [[nodiscard]] StatusCreateInfo statusCreateInfo() const;
    void setStatusCreateInfo(const StatusCreateInfo &statusCreateInfo);

Q_SIGNALS:
    void createUserStatusDone();

private:
    Q_DISABLE_COPY(CustomUserStatusCreateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    StatusCreateInfo mStatusCreateInfo;
};
}
