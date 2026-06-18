/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsBannedUsersJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoomsBannedUsersJobInfo {
        QByteArray roomId;
        int offset = 0;
        int count = 0;
        [[nodiscard]] bool isValid() const;
        void generateRequest(QUrlQuery &query) const;
    };

    explicit RoomsBannedUsersJob(QObject *parent = nullptr);
    ~RoomsBannedUsersJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] RoomsBannedUsersJobInfo roomsBannedUsersJobInfo() const;
    void setRoomsBannedUsersJobInfo(const RoomsBannedUsersJobInfo &newRoomsBannedUsersJobInfo);

Q_SIGNALS:
    void roomsBannedUsersDone(const QJsonObject &obj, const QByteArray &roomId);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoomsBannedUsersJobInfo mRoomsBannedUsersJobInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::RoomsBannedUsersJob::RoomsBannedUsersJobInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsBannedUsersJob::RoomsBannedUsersJobInfo &t);
