/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT EmojiCustomCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit EmojiCustomCreateJob(QObject *parent = nullptr);
    ~EmojiCustomCreateJob() override;

    struct LIBROCKETCHATRESTAPI_QT_EXPORT EmojiInfo {
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
    void emojiCustomCreateDone(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(EmojiCustomCreateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotEmojiCustomCreateFinished();
    EmojiInfo mEmojiInfo;
};
}
