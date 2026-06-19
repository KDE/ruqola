/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelInviteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    enum class ChannelInviteInfoType : uint8_t {
        Unknown,
        UserId,
        UserName,
    };
    Q_ENUM(ChannelInviteInfoType)

    struct ChannelInviteInfo {
        QString identifier;
        ChannelInviteJob::ChannelInviteInfoType channelGroupInfoType = ChannelInviteJob::ChannelInviteInfoType::Unknown;
        [[nodiscard]] bool isValid() const;
    };

    explicit ChannelInviteJob(QObject *parent = nullptr);
    ~ChannelInviteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] ChannelInviteInfo channelInviteInfo() const;
    void setChannelInviteInfo(const ChannelInviteInfo &newChannelInviteInfo);

Q_SIGNALS:
    void inviteDone();
    void needUnbanned(const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &info);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;
    [[nodiscard]] bool interceptError(const QJsonObject &replyObject) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    ChannelInviteInfo mChannelInviteInfo;
};
}
