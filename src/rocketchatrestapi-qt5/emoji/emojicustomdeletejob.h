/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT EmojiCustomDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit EmojiCustomDeleteJob(QObject *parent = nullptr);
    ~EmojiCustomDeleteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString emojiId() const;
    void setEmojiId(const QString &emojiId);

Q_SIGNALS:
    void emojiCustomDeleteDone();

private:
    Q_DISABLE_COPY(EmojiCustomDeleteJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mEmojiId;
};
}
