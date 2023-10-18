/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "userbasejob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UserInfoJob : public UserBaseJob
{
    Q_OBJECT
public:
    explicit UserInfoJob(QObject *parent = nullptr);
    ~UserInfoJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;
Q_SIGNALS:
    void userInfoDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UserInfoJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
