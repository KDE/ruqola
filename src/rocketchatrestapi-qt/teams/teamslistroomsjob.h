/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamsListRoomsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamsListRoomsJob(QObject *parent = nullptr);
    ~TeamsListRoomsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString teamId() const;
    void setTeamId(const QString &teamId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void teamListRoomsDone(const QJsonObject &obj);

protected:
    [[nodiscard]] QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(TeamsListRoomsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTeamId;
};
}
