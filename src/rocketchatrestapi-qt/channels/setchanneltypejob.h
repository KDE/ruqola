/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SetChannelTypeJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit SetChannelTypeJob(QObject *parent = nullptr);
    ~SetChannelTypeJob() override;

    enum class GroupType : uint8_t {
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
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    GroupType mType = GroupType::Unknown;
};
}
