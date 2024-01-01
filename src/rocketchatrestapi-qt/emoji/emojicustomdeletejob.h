/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT EmojiCustomDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit EmojiCustomDeleteJob(QObject *parent = nullptr);
    ~EmojiCustomDeleteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString emojiId() const;
    void setEmojiId(const QString &emojiId);

Q_SIGNALS:
    void emojiCustomDeleteDone();

private:
    Q_DISABLE_COPY(EmojiCustomDeleteJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mEmojiId;
};
}
