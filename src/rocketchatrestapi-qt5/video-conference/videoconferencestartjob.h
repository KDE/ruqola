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
class LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceStartJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT VideoConferenceStartInfo {
        QString roomId;
        QString title;
        bool allowRinging = true;

        Q_REQUIRED_RESULT bool isValid() const;
    };

    explicit VideoConferenceStartJob(QObject *parent = nullptr);
    ~VideoConferenceStartJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT VideoConferenceStartInfo info() const;
    void setInfo(const VideoConferenceStartInfo &newInfo);

Q_SIGNALS:
    void videoConferenceStartDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(VideoConferenceStartJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

    VideoConferenceStartInfo mInfo;
};
}
