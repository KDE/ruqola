/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QDateTime>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SyncMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SyncMessagesJob(QObject *parent = nullptr);
    ~SyncMessagesJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] QDateTime lastUpdate() const;
    void setLastUpdate(const QDateTime &newLastUpdate);

Q_SIGNALS:
    void syncMessagesDone(const QJsonObject &obj, const QByteArray &roomId);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mRoomId;
    QDateTime mLastUpdate;
};
}
