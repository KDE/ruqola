/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceProvidersJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceProvidersJob(QObject *parent = nullptr);
    ~VideoConferenceProvidersJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void videoConferenceProvidersDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(VideoConferenceProvidersJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
