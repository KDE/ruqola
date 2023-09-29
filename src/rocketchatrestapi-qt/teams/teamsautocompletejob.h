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
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamsAutoCompleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamsAutoCompleteJob(QObject *parent = nullptr);
    ~TeamsAutoCompleteJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] const QString &name() const;
    void setName(const QString &newName);

Q_SIGNALS:
    void teamAutoCompleteDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(TeamsAutoCompleteJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mName;
};
}
