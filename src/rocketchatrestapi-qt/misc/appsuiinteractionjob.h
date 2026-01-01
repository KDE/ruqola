/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QJsonObject>
#include <QNetworkRequest>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AppsUiInteractionJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT AppsUiInteractionJobInfo {
        QJsonObject messageObj;
        QString methodName;
        void generateMessageObj(const QString &actionId, const QString &value, const QString &blockId, const QByteArray &roomId, const QByteArray &messageId);
        [[nodiscard]] bool isValid() const;
    };
    explicit AppsUiInteractionJob(QObject *parent = nullptr);
    ~AppsUiInteractionJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] AppsUiInteractionJobInfo methodCallJobInfo() const;
    void setAppsUiInteractionJobInfo(const AppsUiInteractionJobInfo &newAppsUiInteractionJobInfo);

Q_SIGNALS:
    void appsUiInteractionDone(const QJsonObject &replyObject);
    void appsUiInteractionFailed(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    AppsUiInteractionJobInfo mAppsUiInteractionJobInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo &t);
