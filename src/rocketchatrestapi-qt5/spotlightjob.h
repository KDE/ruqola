/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SpotlightJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SpotlightJob(QObject *parent = nullptr);
    ~SpotlightJob() override;

    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString searchPattern() const;
    void setSearchPattern(const QString &searchPattern);

Q_SIGNALS:
    void spotlightDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(SpotlightJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mSearchPattern;
};
}
