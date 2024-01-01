/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT OwnInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit OwnInfoJob(QObject *parent = nullptr);
    ~OwnInfoJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void ownInfoDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(OwnInfoJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
