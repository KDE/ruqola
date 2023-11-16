/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GetSupportedLanguagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetSupportedLanguagesJob(QObject *parent = nullptr);
    ~GetSupportedLanguagesJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool needTargetLanguage() const;
    void setNeedTargetLanguage(bool newNeedTargetLanguage);

Q_SIGNALS:
    void getSupportedLanguagesDone(const QJsonObject &obj);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(GetSupportedLanguagesJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mNeedTargetLanguage = false;
};
}
