/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librocketchatrestapi-qt_export.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT CustomSoundsListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomSoundsListJob(QObject *parent = nullptr);
    ~CustomSoundsListJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void customSoundsListDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(CustomSoundsListJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mPattern;
};
}
