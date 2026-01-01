/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelMembersJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    enum class ChannelType : uint8_t {
        Channel = 0,
        Groups,
        Direct,
        Unknown,
    };
    Q_ENUM(ChannelType)
    explicit ChannelMembersJob(QObject *parent = nullptr);
    ~ChannelMembersJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] ChannelType channelType() const;
    void setChannelType(RocketChatRestApi::ChannelMembersJob::ChannelType channelType);

    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void channelMembersDone(const QJsonObject &replyObjectj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

protected:
    [[nodiscard]] QString generateErrorMessage(const QString &errorStr) const override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    ChannelType mChannelType = ChannelType::Unknown;
};
}
