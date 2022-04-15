/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT TranslateSaveSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateSaveSettingsJob(QObject *parent = nullptr);
    ~TranslateSaveSettingsJob() override;

    enum SettingType { AutoTranslateSetting, LanguageSetting, Undefined };

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool autoTranslate() const;
    void setAutoTranslate(bool autoTranslate);

    Q_REQUIRED_RESULT QString language() const;
    void setLanguage(const QString &language);

    Q_REQUIRED_RESULT SettingType type() const;
    void setType(SettingType type);

Q_SIGNALS:
    void translateSavesettingsDone();

private:
    Q_DISABLE_COPY(TranslateSaveSettingsJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mLanguage;
    TranslateSaveSettingsJob::SettingType mType = Undefined;
    bool mAutoTranslate = false;
};
}
