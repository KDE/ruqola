/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "roles/roleinfo.h"
#include <QList>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class User;
class QFormLayout;
class RocketChatAccount;
class ResizablePixmapLabel;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectChannelActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectChannelActionWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DirectChannelActionWidget() override;
    void setUser(const User &user);

Q_SIGNALS:
    void ignoreUser();
    void reportUser();

private:
    QToolButton *const mToolButton;
    RocketChatAccount *const mRocketChatAccount;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectChannelInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectChannelInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DirectChannelInfoWidget() override;

    void setUserName(const QString &userName);

    void setRoles(const QList<RoleInfo> &newRoles);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotIgnoreUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotReportUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUserInfoDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void fetchUserInfo(const QString &userName);
    LIBRUQOLAWIDGETS_NO_EXPORT void setUser(const User &user);
    QList<RoleInfo> mListRoleInfos;
    ResizablePixmapLabel *const mAvatar;
    QFormLayout *const mFormLayout;
    DirectChannelActionWidget *const mDirectChannelActionWidget;
    RocketChatAccount *const mRocketChatAccount;
};
