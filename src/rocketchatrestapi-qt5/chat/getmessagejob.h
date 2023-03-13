/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT GetMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetMessageJob(QObject *parent = nullptr);
    ~GetMessageJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void getMessageDone(const QJsonObject &obj, const QString &messageId, const QString &roomId);

private:
    Q_DISABLE_COPY(GetMessageJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    QString mRoomId;
};
}
