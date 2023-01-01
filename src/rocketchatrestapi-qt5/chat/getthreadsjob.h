/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetThreadsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class TheadSearchType {
        All = 0,
        Following,
        Unread,
    };
    explicit GetThreadsJob(QObject *parent = nullptr);
    ~GetThreadsJob() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);
    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

    Q_REQUIRED_RESULT TheadSearchType searchType() const;
    void setSearchType(TheadSearchType newSearchType);

Q_SIGNALS:
    void getThreadsDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(GetThreadsJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    TheadSearchType mSearchType = TheadSearchType::All;
    QString mRoomId;
};
}
