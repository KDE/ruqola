/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT PostMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PostMessageJob(QObject *parent = nullptr);
    ~PostMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString text() const;
    void setText(const QString &text);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void postMessageDone(const QJsonObject &replyObject);

protected:
    Q_REQUIRED_RESULT QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(PostMessageJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mText;
    QString mRoomId;
};
}
