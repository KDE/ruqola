/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceCancelJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceCancelJob(QObject *parent = nullptr);
    ~VideoConferenceCancelJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString callId() const;
    void setCallId(const QString &newCallId);

Q_SIGNALS:
    void videoConferenceCancelDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(VideoConferenceCancelJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

    QString mCallId;
};
}
