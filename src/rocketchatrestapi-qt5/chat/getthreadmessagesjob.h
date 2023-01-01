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

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;
Q_SIGNALS:
    void getThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);

private:
    Q_DISABLE_COPY(GetThreadMessagesJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    QString mThreadMessageId;
};
}
