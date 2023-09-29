/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT VideoConferenceCancelJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit VideoConferenceCancelJob(QObject *parent = nullptr);
    ~VideoConferenceCancelJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString callId() const;
    void setCallId(const QString &newCallId);

Q_SIGNALS:
    void videoConferenceCancelDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(VideoConferenceCancelJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

    QString mCallId;
};
}
