/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceCapabilitiesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceCapabilitiesJob(QObject *parent = nullptr);
    ~VideoConferenceCapabilitiesJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void videoConferenceCapabilitiesDone(const QJsonObject &obj);
    void noVideoConferenceProviderApps();

private:
    Q_DISABLE_COPY(VideoConferenceCapabilitiesJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
