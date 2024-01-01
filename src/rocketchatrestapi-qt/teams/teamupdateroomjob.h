/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamUpdateRoomJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamUpdateRoomJob(QObject *parent = nullptr);
    ~TeamUpdateRoomJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);
    [[nodiscard]] bool isDefault() const;
    void setIsDefault(bool isDefault);

Q_SIGNALS:
    void teamUpdateRoomDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(TeamUpdateRoomJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    bool mIsDefault = false;
};
}
