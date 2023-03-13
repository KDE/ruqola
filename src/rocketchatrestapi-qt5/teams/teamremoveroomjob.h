/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamRemoveRoomJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamRemoveRoomJob(QObject *parent = nullptr);
    ~TeamRemoveRoomJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);
    Q_REQUIRED_RESULT QString teamId() const;
    void setTeamId(const QString &teamId);

Q_SIGNALS:
    void removeTeamRoomDone();

private:
    Q_DISABLE_COPY(TeamRemoveRoomJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mTeamId;
};
}
