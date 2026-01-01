/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TranslateSaveSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateSaveSettingsJob(QObject *parent = nullptr);
    ~TranslateSaveSettingsJob() override;

    enum class SettingType : uint8_t {
        AutoTranslateSetting,
        LanguageSetting,
        Undefined
    };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] bool autoTranslate() const;
    void setAutoTranslate(bool autoTranslate);

    [[nodiscard]] QString language() const;
    void setLanguage(const QString &language);

    [[nodiscard]] SettingType type() const;
    void setType(SettingType type);

Q_SIGNALS:
    void translateSavesettingsDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mLanguage;
    TranslateSaveSettingsJob::SettingType mType = SettingType::Undefined;
    bool mAutoTranslate = false;
};
}
