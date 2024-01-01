/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GroupsConvertToTeamJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GroupsConvertToTeamJob(QObject *parent = nullptr);
    ~GroupsConvertToTeamJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void groupConvertToTeamDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GroupsConvertToTeamJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
