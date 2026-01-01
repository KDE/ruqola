/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AcceptSuggestedGroupKeyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit AcceptSuggestedGroupKeyJob(QObject *parent = nullptr);
    ~AcceptSuggestedGroupKeyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString getRoomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void acceptSuggestedGroupKeyDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
