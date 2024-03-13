/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QJsonObject>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AppsUiInteractionJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT AppsUiInteractionJobInfo {
        QJsonObject messageObj;
        QString methodName;
        void generateMessageObj(const QString &actionId, const QString &value, const QString &blockId, const QString &roomId, const QString &messageId);
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
    void appsUiInteractionDone();

private:
    Q_DISABLE_COPY(AppsUiInteractionJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    AppsUiInteractionJobInfo mAppsUiInteractionJobInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo, Q_RELOCATABLE_TYPE);
