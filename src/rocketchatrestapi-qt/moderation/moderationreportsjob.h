/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationReportsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ModerationReportsJob(QObject *parent = nullptr);
    ~ModerationReportsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

Q_SIGNALS:
    void moderationReportsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(ModerationReportsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
};
}
