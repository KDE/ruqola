/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationDismissUserReportsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationDismissUserReportsJob(QObject *parent = nullptr);
    ~ModerationDismissUserReportsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QByteArray moderationReportedUserId() const;
    void setModerationReportedUserId(const QByteArray &newModerationReportedUserId);

Q_SIGNALS:
    void moderationDismissReportedUserDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mModerationReportedUserId;
};
}
