/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUserInfoDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void fetchUserInfo(const QString &userName);
    LIBRUQOLAWIDGETS_NO_EXPORT void setUser(const User &user);
    QVector<RoleInfo> mListRoleInfos;
    QLabel *const mAvatar;
    QFormLayout *const mMainLayout;
    RocketChatAccount *const mRocketChatAccount;
};
