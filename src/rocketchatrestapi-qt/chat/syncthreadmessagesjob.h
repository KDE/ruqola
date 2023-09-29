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
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT SyncThreadMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SyncThreadMessagesJob(QObject *parent = nullptr);
    ~SyncThreadMessagesJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    // TODO use QTime ?
    [[nodiscard]] QString timeStamp() const;
    void setTimeStamp(const QString &timeStamp);

Q_SIGNALS:
    void syncThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);

private:
    Q_DISABLE_COPY(SyncThreadMessagesJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mThreadMessageId;
    QString mTimeStamp;
};
}
