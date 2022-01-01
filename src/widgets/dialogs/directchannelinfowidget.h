/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "roles/roleinfo.h"
#include <QVector>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLabel;
class User;
class QFormLayout;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectChannelInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectChannelInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DirectChannelInfoWidget() override;

    void setUserName(const QString &userName);

    void setRoles(const QVector<RoleInfo> &newRoles);

private:
    void slotUserInfoDone(const QJsonObject &obj);
    void fetchUserInfo(const QString &userName);
    void setUser(const User &user);
    void hideWidget(QLabel *label);
    QVector<RoleInfo> mListRoleInfos;
    QLabel *const mName;
    QLabel *const mUserName;
    QLabel *const mCustomStatus;
    QLabel *const mStatus;
    QLabel *const mTimeZone;
    QLabel *const mAvatar;
    QLabel *const mRoles;
    QLabel *const mCreateAt;
    QLabel *const mLastLogin;
    QLabel *const mEmailsInfo;
    QFormLayout *const mMainLayout;
    RocketChatAccount *const mRocketChatAccount;
};

