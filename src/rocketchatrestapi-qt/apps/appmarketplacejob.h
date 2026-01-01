/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AppMarketPlaceJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit AppMarketPlaceJob(QObject *parent = nullptr);
    ~AppMarketPlaceJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool isAdminUser() const;
    void setIsAdminUser(bool newIsAdminUser);

Q_SIGNALS:
    void appMarketPlaceDone(const QJsonArray &replyArray);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mIsAdminUser = false;
};
}
