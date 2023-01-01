/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT StatisticsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit StatisticsJob(QObject *parent = nullptr);
    ~StatisticsJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool refresh() const;
    void setRefresh(bool refresh);

Q_SIGNALS:
    void statisticDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(StatisticsJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    bool mRefresh = false;
};
}
