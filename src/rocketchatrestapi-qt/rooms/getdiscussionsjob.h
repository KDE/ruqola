/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GetDiscussionsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetDiscussionsJob(QObject *parent = nullptr);
    ~GetDiscussionsJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);
    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void getDiscussionsDone(const QJsonObject &obj, const QByteArray &roomId);

private:
    Q_DISABLE_COPY(GetDiscussionsJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mRoomId;
};
}
