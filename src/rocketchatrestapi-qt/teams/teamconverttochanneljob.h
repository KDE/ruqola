/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QByteArray teamId() const;
    void setTeamId(const QByteArray &teamId);

    [[nodiscard]] const QList<QByteArray> &roomsToRemove() const;
    void setRoomsToRemove(const QList<QByteArray> &newRoomsToRemove);

Q_SIGNALS:
    void teamConvertToChannelDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mTeamId;
    QList<QByteArray> mRoomsToRemove;
};
}
