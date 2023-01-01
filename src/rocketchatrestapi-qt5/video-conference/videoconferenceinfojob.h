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
class LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceInfoJob(QObject *parent = nullptr);
    ~VideoConferenceInfoJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString callId() const;
    void setCallId(const QString &newCallId);

Q_SIGNALS:
    void videoConferenceInfoDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(VideoConferenceInfoJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    QString mCallId;
};
}
