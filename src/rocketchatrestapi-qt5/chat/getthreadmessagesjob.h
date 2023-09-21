/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT GetThreadMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetThreadMessagesJob(QObject *parent = nullptr);
    ~GetThreadMessagesJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;
Q_SIGNALS:
    void getThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);

private:
    Q_DISABLE_COPY(GetThreadMessagesJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mThreadMessageId;
};
}
