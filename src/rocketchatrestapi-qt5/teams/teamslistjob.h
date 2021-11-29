/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamsListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamsListJob(QObject *parent = nullptr);
    ~TeamsListJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;
    Q_REQUIRED_RESULT const QString &pattern() const;
    void setPattern(const QString &newPattern);

Q_SIGNALS:
    void teamsListDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(TeamsListJob)
    void initialQueryParameters();
    void slotTeamsListFinished();
    QString mPattern;
};
}
