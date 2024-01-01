/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceJoinJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceJoinInfo {
        QString callId;
        bool useMicro = false;
        bool useCamera = false;
        [[nodiscard]] bool isValid() const;
    };
    explicit VideoConferenceJoinJob(QObject *parent = nullptr);
    ~VideoConferenceJoinJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] VideoConferenceJoinInfo info() const;
    void setInfo(const VideoConferenceJoinInfo &newInfo);

Q_SIGNALS:
    void videoConferenceJoinDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(VideoConferenceJoinJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    VideoConferenceJoinInfo mInfo;
};
}
