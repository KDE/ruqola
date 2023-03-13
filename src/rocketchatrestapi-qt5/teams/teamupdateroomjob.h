/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamUpdateRoomJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamUpdateRoomJob(QObject *parent = nullptr);
    ~TeamUpdateRoomJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);
    Q_REQUIRED_RESULT bool isDefault() const;
    void setIsDefault(bool isDefault);

Q_SIGNALS:
    void teamUpdateRoomDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(TeamUpdateRoomJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    bool mIsDefault = false;
};
}
