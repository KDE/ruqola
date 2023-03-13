/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT MarkRoomAsUnReadJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // Since 0.65
    explicit MarkRoomAsUnReadJob(QObject *parent = nullptr);
    ~MarkRoomAsUnReadJob() override;

    enum MarkAsUnReadObject { Unknown = 0, FromMessage, Room };

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT MarkAsUnReadObject unReadObject() const;
    void setUnReadObject(MarkAsUnReadObject type);

    Q_REQUIRED_RESULT QString objectId() const;
    void setObjectId(const QString &objectId);

Q_SIGNALS:
    void markAsUnReadDone();

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(MarkRoomAsUnReadJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mObjectId;
    MarkAsUnReadObject mUnReadObject = Unknown;
};
}
