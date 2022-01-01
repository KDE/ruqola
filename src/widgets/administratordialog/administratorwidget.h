/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QTabWidget;
class AdministratorRoomsWidget;
class AdministratorCustomUserStatusWidget;
class AdministratorCustomSoundsWidget;
class AdministratorServerInfoWidget;
class AdministratorUsersWidget;
class AdministratorInvitesWidget;
class ViewLogWidget;
class AdministratorCustomEmojiWidget;
class PermissionsWidget;
class AdministratorRolesWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorWidget() override;

    void updateUiFromPermission();
    void initialize();

private:
    QTabWidget *const mTabWidget;
    AdministratorRoomsWidget *const mAdministratorRoomsWidget;
    AdministratorCustomUserStatusWidget *const mAdministratorCustomUserStatusWidget;
    AdministratorCustomSoundsWidget *const mAdministratorCustomSoundsWidget;
    AdministratorServerInfoWidget *const mAdministratorServerInfoWidget;
    AdministratorUsersWidget *const mAdministratorUsersWidget;
    AdministratorInvitesWidget *const mAdministratorInvitesWidget;
    AdministratorCustomEmojiWidget *const mAdministratorCustomEmojiWidget;
    ViewLogWidget *const mViewLogWidget;
    PermissionsWidget *const mPermissionsWidget;
    AdministratorRolesWidget *const mRolesWidget;
    RocketChatAccount *const mRocketChatAccount;
    int mCustomSoundsTabIndex = -1;
    int mServerInfoTabIndex = -1;
    int mViewLogTagIndex = -1;
};

