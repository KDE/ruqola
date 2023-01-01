/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SyncThreadMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SyncThreadMessagesJob(QObject *parent = nullptr);
    ~SyncThreadMessagesJob() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    // TODO use QTime ?
    Q_REQUIRED_RESULT QString timeStamp() const;
    void setTimeStamp(const QString &timeStamp);

Q_SIGNALS:
    void syncThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);

private:
    Q_DISABLE_COPY(SyncThreadMessagesJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    QString mThreadMessageId;
    QString mTimeStamp;
};
}
