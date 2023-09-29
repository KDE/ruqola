/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createupdateuserinfo.h"
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UsersUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UsersUpdateJob(QObject *parent = nullptr);
    ~UsersUpdateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] UpdateUserInfo updateInfo() const;
    void setUpdateInfo(const UpdateUserInfo &updateInfo);
    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void usersUpdateDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersUpdateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UpdateUserInfo mUpdateInfo;
};
}
