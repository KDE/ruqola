/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamRemoveMembersJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamRemoveMembersJob(QObject *parent = nullptr);
    ~TeamRemoveMembersJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString teamId() const;
    void setTeamId(const QString &teamId);

    [[nodiscard]] QStringList roomsId() const;
    void setRoomsId(const QStringList &roomsId);

Q_SIGNALS:
    void removeTeamMembersDone();

private:
    Q_DISABLE_COPY(TeamRemoveMembersJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QStringList mRoomsId;
    QString mTeamId;
};
}
