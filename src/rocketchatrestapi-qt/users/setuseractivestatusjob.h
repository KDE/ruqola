/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SetUserActiveStatusJob : public RestApiAbstractJob
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

    [[nodiscard]] const QByteArray &activateUserId() const;
    void setActivateUserId(const QByteArray &newActivateUserId);

Q_SIGNALS:
    void setUserActiveStatusDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mActivateUserId;
    bool mActivate = true;
};
}
