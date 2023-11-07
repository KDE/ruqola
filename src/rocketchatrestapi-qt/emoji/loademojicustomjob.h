/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librocketchatrestapi-qt_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT LoadEmojiCustomJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit LoadEmojiCustomJob(QObject *parent = nullptr);
    ~LoadEmojiCustomJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool hasQueryParameterSupport() const override;
Q_SIGNALS:
    void loadEmojiCustomDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(LoadEmojiCustomJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
