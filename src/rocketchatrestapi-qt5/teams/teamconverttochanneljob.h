/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    void slotTeamConvertToChannelFinished();
    QString mTeamId;
    QStringList mRoomsToRemove;
};
}
