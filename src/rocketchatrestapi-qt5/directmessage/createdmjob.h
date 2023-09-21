/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT CreateDmJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CreateDmJob(QObject *parent = nullptr);
    ~CreateDmJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QStringList userNames() const;
    void setUserNames(const QStringList &userNames);

Q_SIGNALS:
    void createDmDone();

private:
    Q_DISABLE_COPY(CreateDmJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QStringList mUserNames;
};
}
