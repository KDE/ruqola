/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
// Asks the server to notify the members of every encrypted room we are subscribed to but have
// no key for, so that one of them encrypts the room key for us ("e2e.keyRequest").
class LIBROCKETCHATRESTAPI_QT_EXPORT RequestSubscriptionKeysJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RequestSubscriptionKeysJob(QObject *parent = nullptr);
    ~RequestSubscriptionKeysJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

Q_SIGNALS:
    void requestSubscriptionKeysDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
