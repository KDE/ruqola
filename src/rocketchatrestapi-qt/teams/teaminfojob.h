/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamInfoJob(QObject *parent = nullptr);
    ~TeamInfoJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString teamId() const;
    void setTeamId(const QString &teamId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void teamInfoDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(TeamInfoJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTeamId;
};
}
