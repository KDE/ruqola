/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QDateTime>
#include <QDebug>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsCleanHistoryJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT CleanHistoryInfo {
        Q_REQUIRED_RESULT bool isValid() const;
        QDateTime latest; // required
        QDateTime oldest; // required
        QString roomId; // required
        QStringList users;
        bool inclusive = false;
        bool excludePinned = false;
        bool filesOnly = false;
        bool ignoreThreads = false;
        bool ignoreDiscussion = false;
    };
    explicit RoomsCleanHistoryJob(QObject *parent = nullptr);
    ~RoomsCleanHistoryJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT CleanHistoryInfo cleanHistoryInfo() const;
    void setCleanHistoryInfo(const CleanHistoryInfo &cleanHistoryInfo);

Q_SIGNALS:
    void cleanHistoryDone();

private:
    Q_DISABLE_COPY(RoomsCleanHistoryJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    CleanHistoryInfo mCleanHistoryInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &t);
