/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SetChannelTypeJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit SetChannelTypeJob(QObject *parent = nullptr);
    ~SetChannelTypeJob() override;

    enum GroupType {
        Unknown = 0,
        Private,
        Public,
    };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] GroupType type() const;
    void setType(GroupType type);

Q_SIGNALS:
    void setGroupTypeDone();

private:
    Q_DISABLE_COPY(SetChannelTypeJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    GroupType mType = Unknown;
};
}
