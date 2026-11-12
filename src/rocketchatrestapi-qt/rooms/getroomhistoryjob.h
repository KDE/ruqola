/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
class QDebug;
class QUrl;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GetRoomHistoryJob final : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT GetRoomHistoryInfo {
        [[nodiscard]] bool isValid() const;
        void addQueryItem(QUrl &url) const;
        QByteArray roomId;
        int count = 20; // Default
        bool showThreadMessages = true; // Default
        QByteArray aroundId;
        QString lastSeen;
        QString previous;
        QString next;
    };

    explicit GetRoomHistoryJob(QObject *parent = nullptr);
    ~GetRoomHistoryJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] GetRoomHistoryInfo roomHistoryInfo() const;
    void setRoomHistoryInfo(const GetRoomHistoryInfo &newRoomHistoryInfo);

Q_SIGNALS:
    void getRoomHistoryDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    GetRoomHistoryInfo mRoomHistoryInfo;
};
}
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::GetRoomHistoryJob::GetRoomHistoryInfo &t);
