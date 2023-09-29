/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamsListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamsListJob(QObject *parent = nullptr);
    ~TeamsListJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;
    [[nodiscard]] const QString &pattern() const;
    void setPattern(const QString &newPattern);

Q_SIGNALS:
    void teamsListDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(TeamsListJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void initialQueryParameters();
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mPattern;
};
}
