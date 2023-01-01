/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ListInviteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ListInviteJob(QObject *parent = nullptr);
    ~ListInviteJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void listInviteDone(const QJsonDocument &obj);

private:
    Q_DISABLE_COPY(ListInviteJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}
