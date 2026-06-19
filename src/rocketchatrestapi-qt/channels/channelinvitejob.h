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
        Identifier,
        Name,
    };
    Q_ENUM(ChannelInviteInfoType)

    struct ChannelInviteInfo {
        QString identifier;
        ChannelInviteJob::ChannelInviteInfoType channelGroupInfoType = ChannelInviteJob::ChannelInviteInfoType::Unknown;
    };

    explicit ChannelInviteJob(QObject *parent = nullptr);
    ~ChannelInviteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString inviteUserId() const;
    void setInviteUserId(const QString &userId);

    [[nodiscard]] QString inviteUserName() const;
    void setInviteUserName(const QString &userName);

Q_SIGNALS:
    void inviteDone();
    void needUnbanned(const QString &userId, const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &info);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;
    [[nodiscard]] bool interceptError(const QJsonObject &replyObject) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mInviteUserId;
    QString mInviteUserName;
};
}
