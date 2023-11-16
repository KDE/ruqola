/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT StatisticsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit StatisticsJob(QObject *parent = nullptr);
    ~StatisticsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool refresh() const;
    void setRefresh(bool refresh);

Q_SIGNALS:
    void statisticDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(StatisticsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mRefresh = false;
};
}
