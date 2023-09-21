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

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool activate() const;
    void setActivate(bool newActivate);

    [[nodiscard]] const QString &activateUserId() const;
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
