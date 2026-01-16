/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QJsonObject>
#include <QNetworkRequest>
class QDebug;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT MethodCallJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT MethodCallJobInfo {
        QJsonObject messageObj;
        QString methodName;
        bool anonymous = false;
        [[nodiscard]] bool isValid() const;
    };
    explicit MethodCallJob(QObject *parent = nullptr);
    ~MethodCallJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] MethodCallJobInfo methodCallJobInfo() const;
    void setMethodCallJobInfo(const MethodCallJobInfo &newMethodCallJobInfo);

Q_SIGNALS:
    void methodCallDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    MethodCallJobInfo mMethodCallJobInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::MethodCallJob::MethodCallJobInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::MethodCallJob::MethodCallJobInfo &t);
