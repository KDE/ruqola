/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QDebug>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RunCommandJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RunCommandInfo {
        QString commandName;
        QString roomId;
        QString threadMessageId;
        QString triggerId;
        QString params;
        [[nodiscard]] bool isValid() const;
    };
    explicit RunCommandJob(QObject *parent = nullptr);
    ~RunCommandJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] RunCommandInfo runCommandInfo() const;
    void setRunCommandInfo(const RunCommandInfo &runCommandInfo);

    static RunCommandInfo parseString(const QString &str, const QString &roomId, const QString &tmid);
Q_SIGNALS:
    void runCommandDone();

private:
    Q_DISABLE_COPY(RunCommandJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RunCommandInfo mRunCommandInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::RunCommandJob::RunCommandInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::RunCommandJob::RunCommandInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::RunCommandJob::RunCommandInfo &t);
