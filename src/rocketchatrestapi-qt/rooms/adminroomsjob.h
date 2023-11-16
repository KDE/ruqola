/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AdminRoomsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class AdminRoomSearchType {
        None = 0,
        Direct = 1,
        Public = 2,
        Private = 4,
        Channel = 8,
        Team = 16,
        OmniChannel = 32,
        All = Direct | Private | Channel | Team | OmniChannel,
    };
    Q_DECLARE_FLAGS(AdminRoomSearchTypes, AdminRoomSearchType)
    Q_FLAG(AdminRoomSearchType)

    struct LIBROCKETCHATRESTAPI_QT_EXPORT AdminRoomsJobInfo {
        QString filter;
        AdminRoomSearchTypes searchType = AdminRoomSearchType::None;
    };

    explicit AdminRoomsJob(QObject *parent = nullptr);
    ~AdminRoomsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] AdminRoomsJobInfo roomsAdminInfo() const;
    void setRoomsAdminInfo(const AdminRoomsJobInfo &roomsAdminInfo);
    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void adminRoomsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(AdminRoomsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void initialUrlParameters(QUrlQuery &urlQuery) const;
    AdminRoomsJobInfo mRoomsAdminInfo;
};
}
