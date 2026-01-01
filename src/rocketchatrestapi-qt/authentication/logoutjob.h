/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT LogoutJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit LogoutJob(QObject *parent = nullptr);
    ~LogoutJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void logoutDone();

protected:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotLogout();
};
}
