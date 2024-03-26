/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamDeleteJob(QObject *parent = nullptr);
    ~TeamDeleteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString teamId() const;
    void setTeamId(const QString &teamId);

    [[nodiscard]] QList<QByteArray> roomsId() const;
    void setRoomsId(const QList<QByteArray> &roomsId);

Q_SIGNALS:
    void deleteTeamDone();

private:
    Q_DISABLE_COPY(TeamDeleteJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QList<QByteArray> mRoomToDeleteId;
    QString mTeamId;
};
}
