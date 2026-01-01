/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationReportsByUserIdJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationReportsByUserIdJob(QObject *parent = nullptr);
    ~ModerationReportsByUserIdJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] QByteArray moderationUserId() const;
    void setModerationUserId(const QByteArray &newModerationUserId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void moderationReportsByUserIdDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mModerationUserId;
};
}
