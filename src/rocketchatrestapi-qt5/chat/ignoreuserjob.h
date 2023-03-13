/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT IgnoreUserJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit IgnoreUserJob(QObject *parent = nullptr);
    ~IgnoreUserJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QString ignoreUserId() const;
    void setIgnoreUserId(const QString &ignoreUserId);

    Q_REQUIRED_RESULT bool ignore() const;
    void setIgnore(bool ignore);

Q_SIGNALS:
    void ignoreUserDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(IgnoreUserJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mIgnoreUserId;
    bool mIgnore = true;
};
}
