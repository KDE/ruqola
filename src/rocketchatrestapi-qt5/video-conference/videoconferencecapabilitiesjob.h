/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceCapabilitiesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceCapabilitiesJob(QObject *parent = nullptr);
    ~VideoConferenceCapabilitiesJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

Q_SIGNALS:
    void videoConferenceCapabilitiesDone(const QJsonObject &obj);
    void noVideoConferenceProviderApps();

private:
    Q_DISABLE_COPY(VideoConferenceCapabilitiesJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
