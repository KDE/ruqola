/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SettingsRemoveCustomOAuthJob final : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SettingsRemoveCustomOAuthJob(QObject *parent = nullptr);
    ~SettingsRemoveCustomOAuthJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

Q_SIGNALS:
    void settingsRemoveCustomOAuthDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mName;
};
}
