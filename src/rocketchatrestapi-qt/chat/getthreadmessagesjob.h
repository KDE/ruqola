/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GetThreadMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetThreadMessagesJob(QObject *parent = nullptr);
    ~GetThreadMessagesJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QByteArray threadMessageId() const;
    void setThreadMessageId(const QByteArray &threadMessageId);

    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;
Q_SIGNALS:
    void getThreadMessagesDone(const QJsonObject &obj, const QByteArray &threadMessageId);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mThreadMessageId;
};
}
