/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT EmojiCustomUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit EmojiCustomUpdateJob(QObject *parent = nullptr);
    ~EmojiCustomUpdateJob() override;

    struct LIBROCKETCHATRESTAPI_QT5_EXPORT EmojiInfo {
        QString emojiId;
        QString alias;
        QString name;
        QUrl fileNameUrl;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT const EmojiInfo &emojiInfo() const;
    void setEmojiInfo(const EmojiInfo &newEmojiInfo);

Q_SIGNALS:
    void emojiCustomUpdateDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(EmojiCustomUpdateJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void slotEmojiCustomUpdateFinished();
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    EmojiInfo mEmojiInfo;
};
}
