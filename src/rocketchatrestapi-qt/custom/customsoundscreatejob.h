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
class LIBROCKETCHATRESTAPI_QT_EXPORT CustomSoundsCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomSoundsCreateJob(QObject *parent = nullptr);
    ~CustomSoundsCreateJob() override;

    struct LIBROCKETCHATRESTAPI_QT_EXPORT SoundInfo {
        QString name;
        QUrl fileNameUrl;
        [[nodiscard]] bool isValid() const;
    };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] const SoundInfo &soundInfo() const;
    void setSoundInfo(const SoundInfo &newEmojiInfo);

Q_SIGNALS:
    void customSoundCreateDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotCustomSoundCreateFinished();
    SoundInfo mSoundInfo;
};
}
