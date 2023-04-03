/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SetUserActiveStatusJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SetUserActiveStatusJob(QObject *parent = nullptr);
    ~SetUserActiveStatusJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT bool activate() const;
    void setActivate(bool newActivate);

    Q_REQUIRED_RESULT const QString &activateUserId() const;
    void setActivateUserId(const QString &newActivateUserId);

Q_SIGNALS:
    void setUserActiveStatusDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(SetUserActiveStatusJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mActivateUserId;
    bool mActivate = true;
};
}
