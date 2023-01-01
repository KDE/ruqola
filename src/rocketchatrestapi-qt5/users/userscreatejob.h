/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createupdateuserinfo.h"
#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UsersCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UsersCreateJob(QObject *parent = nullptr);
    ~UsersCreateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT CreateUpdateUserInfo createInfo() const;
    void setCreateInfo(const CreateUpdateUserInfo &createInfo);

Q_SIGNALS:
    void usersCreateDone(const QJsonObject &obj);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(UsersCreateJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    CreateUpdateUserInfo mCreateInfo;
};
}
