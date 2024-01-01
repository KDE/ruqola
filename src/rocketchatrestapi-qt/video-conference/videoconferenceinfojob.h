/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceInfoJob(QObject *parent = nullptr);
    ~VideoConferenceInfoJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString callId() const;
    void setCallId(const QString &newCallId);

Q_SIGNALS:
    void videoConferenceInfoDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(VideoConferenceInfoJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mCallId;
};
}
