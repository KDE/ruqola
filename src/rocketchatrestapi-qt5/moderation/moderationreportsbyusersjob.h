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
class LIBROCKETCHATRESTAPI_QT5_EXPORT ModerationReportsByUsersJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationReportsByUsersJob(QObject *parent = nullptr);
    ~ModerationReportsByUsersJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void moderationReportByUserDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(ModerationReportsByUsersJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
