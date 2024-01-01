/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationUserReportedMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationUserReportedMessagesJob(QObject *parent = nullptr);
    ~ModerationUserReportedMessagesJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString reportedMessageFromUserId() const;
    void setReportedMessageFromUserId(const QString &newReportedMessageFromUserId);

Q_SIGNALS:
    void moderationUserReportedMessagesDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(ModerationUserReportedMessagesJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mReportedMessageFromUserId;
};
}
