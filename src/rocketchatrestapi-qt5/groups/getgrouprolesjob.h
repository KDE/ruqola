/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetGroupRolesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetGroupRolesJob(QObject *parent = nullptr);
    ~GetGroupRolesJob() override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void groupRolesDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(GetGroupRolesJob)
    void slotGetGroupRolesFinished();
    QString mRoomId;
};
}
