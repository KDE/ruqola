/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librestapi_private_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT LoadEmojiCustomJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit LoadEmojiCustomJob(QObject *parent = nullptr);
    ~LoadEmojiCustomJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;
Q_SIGNALS:
    void loadEmojiCustomDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(LoadEmojiCustomJob)
    void slotloadEmojiCustomDone();
};
}
