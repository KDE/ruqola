/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT CustomUserStatusUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT StatusUpdateInfo {
        QString identifier;
        QString name;
        QString statusType;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !name.isEmpty() && !statusType.isEmpty() && !identifier.isEmpty();
        }
    };
    explicit CustomUserStatusUpdateJob(QObject *parent = nullptr);
    ~CustomUserStatusUpdateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT StatusUpdateInfo statusUpdateInfo() const;
    void setStatusUpdateInfo(const StatusUpdateInfo &statusUpdateInfo);

Q_SIGNALS:
    void customUserUpdateDone();

private:
    Q_DISABLE_COPY(CustomUserStatusUpdateJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    StatusUpdateInfo mStatusUpdateInfo;
};
}
