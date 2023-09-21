/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librocketchatrestapi-qt5_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT PrivateInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PrivateInfoJob(QObject *parent = nullptr);
    ~PrivateInfoJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;
Q_SIGNALS:
    void privateInfoDone(const QJsonObject &data);

private:
    Q_DISABLE_COPY(PrivateInfoJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
