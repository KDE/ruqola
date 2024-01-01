/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createupdateuserinfo.h"
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UsersCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UsersCreateJob(QObject *parent = nullptr);
    ~UsersCreateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] CreateUpdateUserInfo createInfo() const;
    void setCreateInfo(const CreateUpdateUserInfo &createInfo);

Q_SIGNALS:
    void usersCreateDone(const QJsonObject &obj);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(UsersCreateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    CreateUpdateUserInfo mCreateInfo;
};
}
