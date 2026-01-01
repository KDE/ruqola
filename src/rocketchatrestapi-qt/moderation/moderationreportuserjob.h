/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationReportUserJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationReportUserJob(QObject *parent = nullptr);
    ~ModerationReportUserJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QByteArray reportedUserId() const;
    void setReportedUserId(const QByteArray &newReportedUserId);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &newDescription);

Q_SIGNALS:
    void moderationReportUserDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mReportedUserId;
    QString mDescription;
};
}
