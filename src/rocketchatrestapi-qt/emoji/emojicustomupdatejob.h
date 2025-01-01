/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT EmojiCustomUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit EmojiCustomUpdateJob(QObject *parent = nullptr);
    ~EmojiCustomUpdateJob() override;

    struct LIBROCKETCHATRESTAPI_QT_EXPORT EmojiInfo {
        QString emojiId;
        QString alias;
        QString name;
        QUrl fileNameUrl;
        [[nodiscard]] bool isValid() const;
    };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] const EmojiInfo &emojiInfo() const;
    void setEmojiInfo(const EmojiInfo &newEmojiInfo);

Q_SIGNALS:
    void emojiCustomUpdateDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotEmojiCustomUpdateFinished();
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    EmojiInfo mEmojiInfo;
};
}
