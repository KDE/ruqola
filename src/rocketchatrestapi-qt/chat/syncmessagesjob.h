/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
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

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] QDateTime lastUpdate() const;
    void setLastUpdate(const QDateTime &newLastUpdate);

Q_SIGNALS:
    void syncMessagesDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(SyncMessagesJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QDateTime mLastUpdate;
};
}
