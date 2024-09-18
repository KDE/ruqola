/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ModerationReportsByUsersJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT ModerationReportsByUsersInfo {
        QDateTime mLatest;
        QDateTime mOldest;
        QString mSelector;
        [[nodiscard]] bool isValid() const
        {
            return mLatest.isValid() && mOldest.isValid();
        }
    };

    explicit ModerationReportsByUsersJob(QObject *parent = nullptr);
    ~ModerationReportsByUsersJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] ModerationReportsByUsersInfo moderationReportsByUsersInfo() const;
    void setModerationReportsByUsersInfo(const ModerationReportsByUsersInfo &newModerationReportsByUsersInfo);

Q_SIGNALS:
    void moderationReportByUserDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    ModerationReportsByUsersInfo mModerationReportsByUsersInfo;
};
}
