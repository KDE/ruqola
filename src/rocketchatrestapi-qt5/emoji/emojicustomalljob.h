/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librocketchatrestapi-qt5_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT EmojiCustomAllJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit EmojiCustomAllJob(QObject *parent = nullptr);
    ~EmojiCustomAllJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void emojiCustomAllDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(EmojiCustomAllJob)
    void slotEmojiCustomAllDone();
};
}
