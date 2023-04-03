/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT CustomUserStatusCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT StatusCreateInfo {
        QString name;
        QString statusType;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !name.isEmpty() && !statusType.isEmpty();
        }
    };
    explicit CustomUserStatusCreateJob(QObject *parent = nullptr);
    ~CustomUserStatusCreateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;
    Q_REQUIRED_RESULT StatusCreateInfo statusCreateInfo() const;
    void setStatusCreateInfo(const StatusCreateInfo &statusCreateInfo);

Q_SIGNALS:
    void createUserStatusDone();

private:
    Q_DISABLE_COPY(CustomUserStatusCreateJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    StatusCreateInfo mStatusCreateInfo;
};
}
