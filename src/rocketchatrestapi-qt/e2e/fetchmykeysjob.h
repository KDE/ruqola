/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT FetchMyKeysJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit FetchMyKeysJob(QObject *parent = nullptr);
    ~FetchMyKeysJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void fetchMyKeysDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(FetchMyKeysJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
