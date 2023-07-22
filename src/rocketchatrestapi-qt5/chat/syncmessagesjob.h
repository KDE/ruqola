/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SyncMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SyncMessagesJob(QObject *parent = nullptr);
    ~SyncMessagesJob() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

    Q_REQUIRED_RESULT QDateTime lastUpdate() const;
    void setLastUpdate(const QDateTime &newLastUpdate);

Q_SIGNALS:
    void syncMessagesDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(SyncMessagesJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QDateTime mLastUpdate;
};
}
