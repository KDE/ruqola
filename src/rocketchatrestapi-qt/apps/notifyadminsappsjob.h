/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT NotifyAdminsAppsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT NotifyAdminsAppsInfo {
        QByteArray appId;
        QString appName;
        QString appVersion;
        QString message;
        [[nodiscard]] bool isValid() const;
    };
    explicit NotifyAdminsAppsJob(QObject *parent = nullptr);
    ~NotifyAdminsAppsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] NotifyAdminsAppsInfo info() const;
    void setInfo(const NotifyAdminsAppsInfo &newInfo);

Q_SIGNALS:
    void notifyAdminsAppsDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

    NotifyAdminsAppsInfo mInfo;
};
}
