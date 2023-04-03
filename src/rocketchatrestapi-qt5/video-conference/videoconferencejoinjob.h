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
class LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceJoinJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceJoinInfo {
        QString callId;
        bool useMicro = false;
        bool useCamera = false;
        Q_REQUIRED_RESULT bool isValid() const;
    };
    explicit VideoConferenceJoinJob(QObject *parent = nullptr);
    ~VideoConferenceJoinJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT VideoConferenceJoinInfo info() const;
    void setInfo(const VideoConferenceJoinInfo &newInfo);

Q_SIGNALS:
    void videoConferenceJoinDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(VideoConferenceJoinJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    VideoConferenceJoinInfo mInfo;
};
}
