/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librocketchatrestapi-qt_export.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT PublicSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PublicSettingsJob(QObject *parent = nullptr);
    ~PublicSettingsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;
Q_SIGNALS:
    void publicSettingsDone(const QJsonObject &data);

private:
    Q_DISABLE_COPY(PublicSettingsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
