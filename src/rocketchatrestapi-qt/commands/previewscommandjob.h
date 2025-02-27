/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QDebug>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT PreviewsCommandJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT PreviewsCommandInfo {
        QString commandName;
        QString roomId;
        QString params;
        [[nodiscard]] bool isValid() const;
    };
    explicit PreviewsCommandJob(QObject *parent = nullptr);
    ~PreviewsCommandJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] PreviewsCommandInfo previewsCommandInfo() const;
    void setPreviewsCommandInfo(const PreviewsCommandInfo &runCommandInfo);

    [[nodiscard]] static PreviewsCommandInfo parseString(const QString &str, const QByteArray &roomId);
Q_SIGNALS:
    void previewsCommandDone();
    void previewsCommandFailed(const PreviewsCommandJob::PreviewsCommandInfo &info);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    PreviewsCommandInfo mPreviewsCommandInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &t);
