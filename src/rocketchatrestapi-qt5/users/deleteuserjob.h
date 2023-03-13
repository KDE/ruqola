/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "userbasejob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT DeleteUserJob : public UserBaseJob
{
    Q_OBJECT
public:
    explicit DeleteUserJob(QObject *parent = nullptr);
    ~DeleteUserJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void deleteUserDone();

private:
    Q_DISABLE_COPY(DeleteUserJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
