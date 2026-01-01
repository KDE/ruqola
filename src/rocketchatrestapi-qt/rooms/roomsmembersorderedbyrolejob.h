/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsMembersOrderedByRoleJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoomsMembersOrderedByRoleJobInfo {
        QByteArray roomId;
        QString filter;
        int offset = 0;
        int count = 0;
        [[nodiscard]] bool isValid() const;
        void generateRequest(QUrlQuery &query) const;
    };

    explicit RoomsMembersOrderedByRoleJob(QObject *parent = nullptr);
    ~RoomsMembersOrderedByRoleJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] RoomsMembersOrderedByRoleJobInfo roomsMembersOrderedByRoleJobInfo() const;
    void setRoomsMembersOrderedByRoleJobInfo(const RoomsMembersOrderedByRoleJobInfo &newRoomsMembersOrderedByRoleJobInfo);

Q_SIGNALS:
    void roomsMembersOrderedByRoleDone(const QJsonObject &obj, const QByteArray &roomId, const QString &filter);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoomsMembersOrderedByRoleJobInfo mRoomsMembersOrderedByRoleJobInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo &t);
