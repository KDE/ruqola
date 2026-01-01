/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
class AdministratorUsersTabWidget;
class AdministratorInvitesWidget;
class ViewLogWidget;
class AdministratorCustomEmojiWidget;
class PermissionsWidget;
class AdministratorRolesWidget;
class RocketChatAccount;
class AdministratorOauthWidget;
class AdministratorModerationConsoleWidget;
class FeaturePreviewWidget;
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
    AdministratorUsersTabWidget *const mAdministratorUsersWidget;
    AdministratorInvitesWidget *const mAdministratorInvitesWidget;
    AdministratorCustomEmojiWidget *const mAdministratorCustomEmojiWidget;
    ViewLogWidget *const mViewLogWidget;
    PermissionsWidget *const mPermissionsWidget;
    AdministratorRolesWidget *const mRolesWidget;
    AdministratorOauthWidget *const mOauthWidget;
    AdministratorModerationConsoleWidget *const mAdministratorModerationConsoleWidget;
    FeaturePreviewWidget *const mFeaturePreviewWidget;
    RocketChatAccount *const mRocketChatAccount;
};
