/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamDeleteJob(QObject *parent = nullptr);
    ~TeamDeleteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString teamId() const;
    void setTeamId(const QString &teamId);

    Q_REQUIRED_RESULT QStringList roomsId() const;
    void setRoomsId(const QStringList &roomsId);

Q_SIGNALS:
    void deleteTeamDone();

private:
    Q_DISABLE_COPY(TeamDeleteJob)
    void slotDeleteTeamFinished();
    QStringList mRoomToDeleteId;
    QString mTeamId;
};
}
