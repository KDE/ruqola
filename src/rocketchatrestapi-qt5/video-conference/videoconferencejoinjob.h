/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceJoinJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceJoinJob(QObject *parent = nullptr);
    ~VideoConferenceJoinJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void permissionUpdateDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(VideoConferenceJoinJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
};
}
