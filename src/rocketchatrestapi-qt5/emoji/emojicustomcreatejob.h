/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT EmojiCustomCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit EmojiCustomCreateJob(QObject *parent = nullptr);
    ~EmojiCustomCreateJob() override;

    struct LIBROCKETCHATRESTAPI_QT5_EXPORT EmojiInfo {
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
    void emojiCustomCreateDone(const QJsonObject &replyObject);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(EmojiCustomCreateJob)
    void slotEmojiCustomCreateFinished();
    EmojiInfo mEmojiInfo;
};
}
