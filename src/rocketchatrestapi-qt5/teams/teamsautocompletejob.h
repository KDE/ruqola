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
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamsAutoCompleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamsAutoCompleteJob(QObject *parent = nullptr);
    ~TeamsAutoCompleteJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT const QString &name() const;
    void setName(const QString &newName);

Q_SIGNALS:
    void teamAutoCompleteDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(TeamsAutoCompleteJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mName;
};
}
