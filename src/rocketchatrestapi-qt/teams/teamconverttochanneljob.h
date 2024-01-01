/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamConvertToChannelJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamConvertToChannelJob(QObject *parent = nullptr);
    ~TeamConvertToChannelJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString teamId() const;
    void setTeamId(const QString &teamId);

    [[nodiscard]] const QStringList &roomsToRemove() const;
    void setRoomsToRemove(const QStringList &newRoomsToRemove);

Q_SIGNALS:
    void teamConvertToChannelDone();

private:
    Q_DISABLE_COPY(TeamConvertToChannelJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTeamId;
    QStringList mRoomsToRemove;
};
}
