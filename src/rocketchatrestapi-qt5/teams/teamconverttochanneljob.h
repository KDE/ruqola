/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamConvertToChannelJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamConvertToChannelJob(QObject *parent = nullptr);
    ~TeamConvertToChannelJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString teamId() const;
    void setTeamId(const QString &teamId);

    Q_REQUIRED_RESULT const QStringList &roomsToRemove() const;
    void setRoomsToRemove(const QStringList &newRoomsToRemove);

Q_SIGNALS:
    void teamConvertToChannelDone();

private:
    Q_DISABLE_COPY(TeamConvertToChannelJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTeamId;
    QStringList mRoomsToRemove;
};
}
