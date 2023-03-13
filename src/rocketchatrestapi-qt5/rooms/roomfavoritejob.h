/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT RoomFavoriteJob : public RestApiAbstractJob
{
    Q_OBJECT

    // Since rocketchat 0.64
public:
    explicit RoomFavoriteJob(QObject *parent = nullptr);
    ~RoomFavoriteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool favorite() const;
    void setFavorite(bool favorite);

    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;
Q_SIGNALS:
    void changeFavoriteDone();

private:
    Q_DISABLE_COPY(RoomFavoriteJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;

    bool mFavorite = true;
};
}
