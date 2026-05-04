/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SubscriptionGetOneJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SubscriptionGetOneJob(QObject *parent = nullptr);
    ~SubscriptionGetOneJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] const QByteArray &roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void roomInfoDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mRoomId;
};
}
