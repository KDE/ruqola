/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceStartJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceStartInfo {
        QByteArray roomId;
        QString title;
        bool allowRinging = true;

        [[nodiscard]] bool isValid() const;
    };

    explicit VideoConferenceStartJob(QObject *parent = nullptr);
    ~VideoConferenceStartJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] VideoConferenceStartInfo info() const;
    void setInfo(const VideoConferenceStartInfo &newInfo);

Q_SIGNALS:
    void videoConferenceStartDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

    VideoConferenceStartInfo mInfo;
};
}
