/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SetGroupTypeJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit SetGroupTypeJob(QObject *parent = nullptr);
    ~SetGroupTypeJob() override;

    enum GroupType {
        Unknown = 0,
        Private,
        Public,
    };

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT GroupType type() const;
    void setType(GroupType type);

Q_SIGNALS:
    void setGroupTypeDone();

private:
    Q_DISABLE_COPY(SetGroupTypeJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    GroupType mType = Unknown;
};
}
