/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT FetchUsersWaitingForGroupKeyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit FetchUsersWaitingForGroupKeyJob(QObject *parent = nullptr);
    ~FetchUsersWaitingForGroupKeyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    // Rocket.Chat asks for a batch of rooms at once (E2E::initiateKeyDistribution()).
    [[nodiscard]] QList<QByteArray> roomIds() const;
    void setRoomIds(const QList<QByteArray> &newRoomIds);

Q_SIGNALS:
    void fetchUsersWaitingForGroupKeyDone(const QJsonObject &replyObject);

protected:
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

private:
    QList<QByteArray> mRoomIds;
};
}
