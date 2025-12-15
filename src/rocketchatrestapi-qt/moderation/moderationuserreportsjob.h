/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QDateTime>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationUserReportsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT ModerationUserReportsInfo {
        QDateTime mLatest;
        QDateTime mOldest;
        QString mSelector;
        [[nodiscard]] bool isValid() const
        {
            return mLatest.isValid() && mOldest.isValid();
        }
    };
    explicit ModerationUserReportsJob(QObject *parent = nullptr);
    ~ModerationUserReportsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] ModerationUserReportsInfo moderationUserReportsInfo() const;
    void setModerationUserReportsInfo(const ModerationUserReportsInfo &newModerationUserReportsInfo);

Q_SIGNALS:
    void moderationUserReportJobDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    ModerationUserReportsInfo mModerationUserReportsInfo;
};
}
