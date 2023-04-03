/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamInfoJob(QObject *parent = nullptr);
    ~TeamInfoJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString teamId() const;
    void setTeamId(const QString &teamId);

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void teamInfoDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(TeamInfoJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTeamId;
};
}
