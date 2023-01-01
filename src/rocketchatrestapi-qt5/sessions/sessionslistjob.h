/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librocketchatrestapi-qt5_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SessionsListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SessionsListJob(QObject *parent = nullptr);
    ~SessionsListJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void sessionsListDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(SessionsListJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    QString mPattern;
};
}
