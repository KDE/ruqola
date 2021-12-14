/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetDiscussionsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetDiscussionsJob(QObject *parent = nullptr);
    ~GetDiscussionsJob() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);
    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void getDiscussionsDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(GetDiscussionsJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
