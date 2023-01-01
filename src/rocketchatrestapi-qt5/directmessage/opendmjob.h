/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT OpenDmJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit OpenDmJob(QObject *parent = nullptr);
    ~OpenDmJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString directUserId() const;
    void setDirectUserId(const QString &userId);

Q_SIGNALS:
    void openDmDone();

private:
    Q_DISABLE_COPY(OpenDmJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mDirectUserId;
};
}
