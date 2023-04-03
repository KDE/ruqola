/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librocketchatrestapi-qt5_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT CustomSoundsListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomSoundsListJob(QObject *parent = nullptr);
    ~CustomSoundsListJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void customSoundsListDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(CustomSoundsListJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mPattern;
};
}
