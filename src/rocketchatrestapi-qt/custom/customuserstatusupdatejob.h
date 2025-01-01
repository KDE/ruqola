/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT CustomUserStatusUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT StatusUpdateInfo {
        QByteArray identifier;
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
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    StatusUpdateInfo mStatusUpdateInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::CustomUserStatusUpdateJob::StatusUpdateInfo, Q_RELOCATABLE_TYPE);
