/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsUnbanUserJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class IdentifierType : uint8_t {
        Unknown = 0,
        UserId = 1,
        UserName = 2,
    };

    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoomsUnbanUserInfo {
        IdentifierType type = IdentifierType::Unknown;
        QString identifier;
        QByteArray roomId;
        [[nodiscard]] bool isValid() const;
    };

    explicit RoomsUnbanUserJob(QObject *parent = nullptr);
    ~RoomsUnbanUserJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] RoomsUnbanUserInfo roomsUnbanUserInfo() const;
    void setRoomsUnbanUserInfo(const RoomsUnbanUserInfo &newRoomsUnbanUserInfo);

Q_SIGNALS:
    void roomsUnbanUserDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoomsUnbanUserInfo mRoomsUnbanUserInfo;
};
}
