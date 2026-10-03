/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SetRoomKeyIDJob : public RestApiAbstractJob
{
    Q_OBJECT

public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoomKeyIDInfo {
        QByteArray roomId;
        QByteArray keyId;
        [[nodiscard]] bool isValid() const;
    };

    explicit SetRoomKeyIDJob(QObject *parent = nullptr);
    ~SetRoomKeyIDJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] RoomKeyIDInfo roomKeyIDInfo() const;
    void setRoomKeyIDInfo(const RoomKeyIDInfo &newSetRoomKeyIDInfo);

Q_SIGNALS:
    void setRoomKeyIdDone();
    void roomKeyIdAlreadyExists();

protected:
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

private:
    RoomKeyIDInfo mSetRoomKeyIDInfo;
};
}
