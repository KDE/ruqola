/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT LogoutJob : public RestApiAbstractJob
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
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

private:
    Q_DISABLE_COPY(LogoutJob)
    void slotLogout();
};
}
