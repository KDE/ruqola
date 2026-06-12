/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QUrl>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT CustomSoundsUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomSoundsUpdateJob(QObject *parent = nullptr);
    ~CustomSoundsUpdateJob() override;

    struct LIBROCKETCHATRESTAPI_QT_EXPORT SoundInfo {
        QString soundId;
        QString name;
        QUrl fileNameUrl;
        [[nodiscard]] bool isValid() const;
    };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] const SoundInfo &soundInfo() const;
    void setEmojiInfo(const SoundInfo &newEmojiInfo);

Q_SIGNALS:
    void customSoundUpdateDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotCustomSoundUpdateFinished();
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    SoundInfo mSoundInfo;
};
}
