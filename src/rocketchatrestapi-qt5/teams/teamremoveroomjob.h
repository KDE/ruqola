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

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);
    [[nodiscard]] QString teamId() const;
    void setTeamId(const QString &teamId);

Q_SIGNALS:
    void removeTeamRoomDone();

private:
    Q_DISABLE_COPY(TeamRemoveRoomJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mTeamId;
};
}
