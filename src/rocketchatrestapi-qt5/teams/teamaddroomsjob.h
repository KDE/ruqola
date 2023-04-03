/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamAddRoomsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamAddRoomsJob(QObject *parent = nullptr);
    ~TeamAddRoomsJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString teamId() const;
    void setTeamId(const QString &teamId);

    Q_REQUIRED_RESULT QStringList roomIds() const;
    void setRoomIds(const QStringList &roomsId);

Q_SIGNALS:
    void teamAddRoomsDone(const QJsonObject &obj);

protected:
    Q_REQUIRED_RESULT QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(TeamAddRoomsJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QStringList mRoomIds;
    QString mTeamId;
};
}
