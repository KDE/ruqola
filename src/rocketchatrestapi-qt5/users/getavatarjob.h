/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "userbasejob.h"
#include <QPointer>

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetAvatarJob : public UserBaseJob
{
    Q_OBJECT
public:
    explicit GetAvatarJob(QObject *parent = nullptr);
    ~GetAvatarJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

protected:
    Q_REQUIRED_RESULT QString jobName() const override;

Q_SIGNALS:
    void avatar(const UserBaseJob::UserInfo &info, const QUrl &url);
    void redownloadAvatar();

private:
    Q_DISABLE_COPY(GetAvatarJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
