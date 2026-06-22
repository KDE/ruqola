/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "channels/channelinvitejob.h"
#include "libruqolacore_export.h"
#include <QObject>
class Connection;
class QDebug;
class LIBRUQOLACORE_EXPORT AddUserInChannelJob : public QObject
{
    Q_OBJECT
public:
    struct UserInChannelNeedUnBanJobInfo {
        QByteArray roomId;
        QString userName;
        [[nodiscard]] bool isValid() const;
    };

    struct AddUserInChannelJobInfo {
        QByteArray roomId;
        QByteArray userId;
        [[nodiscard]] bool isValid() const;
    };

    explicit AddUserInChannelJob(Connection *restApiConnection, QObject *parent = nullptr);
    ~AddUserInChannelJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] AddUserInChannelJobInfo info() const;
    void setInfo(const AddUserInChannelJobInfo &newInfo);

Q_SIGNALS:
    void userNeedUnbanned(const AddUserInChannelJob::UserInChannelNeedUnBanJobInfo &info);

private:
    LIBRUQOLACORE_NO_EXPORT void slotNeedUnbanned(const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &info);
    AddUserInChannelJobInfo mInfo;
    Connection *const mConnection;
};
Q_DECLARE_TYPEINFO(AddUserInChannelJob::AddUserInChannelJobInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AddUserInChannelJob::UserInChannelNeedUnBanJobInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AddUserInChannelJob::UserInChannelNeedUnBanJobInfo &arg);
