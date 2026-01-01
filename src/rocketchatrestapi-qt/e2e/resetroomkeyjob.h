/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ResetRoomKeyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT ResetRoomKeyInfo {
        [[nodiscard]] bool isValid() const;
        QString rid;
        QString e2eKey;
        QString e2eKeyId;
    };

    explicit ResetRoomKeyJob(QObject *parent = nullptr);
    ~ResetRoomKeyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] ResetRoomKeyInfo resetRoomKeyInfo() const;
    void setResetRoomKeyInfo(const ResetRoomKeyInfo &newResetRoomKeyInfo);

Q_SIGNALS:
    void resetRoomKeyDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    ResetRoomKeyInfo mResetRoomKeyInfo;
};
}
