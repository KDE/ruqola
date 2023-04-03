/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ServerInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ServerInfoJob(QObject *parent = nullptr);
    ~ServerInfoJob() override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool useDeprecatedVersion() const;

    void setUseDeprecatedVersion(bool useDeprecatedVersion);

    Q_REQUIRED_RESULT bool forceRequiresAuthentication() const;
    void setForceRequiresAuthentication(bool forceRequiresAuthentication);

Q_SIGNALS:
    void serverInfoDone(const QString &versionInfo, const QJsonObject &obj);
    void serverInfoFailed(bool useDeprectedVersion);

private:
    Q_DISABLE_COPY(ServerInfoJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mUseDeprecatedVersion = true;
    bool mForceRequiresAuthentication = false;
};
}
