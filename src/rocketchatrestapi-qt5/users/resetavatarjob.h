/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "userbasejob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ResetAvatarJob : public UserBaseJob
{
    Q_OBJECT
public:
    explicit ResetAvatarJob(QObject *parent = nullptr);
    ~ResetAvatarJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void resetAvatarDone();

private:
    Q_DISABLE_COPY(ResetAvatarJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
};
}
