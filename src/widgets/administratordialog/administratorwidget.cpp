/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorwidget.h"
#include "administratordialog/logs/viewlogwidget.h"
#include "administratordialog/oauth/administratoroauthwidget.h"
#include "administratordialog/permissions/permissionswidget.h"
#include "administratordialog/roles/administratorroleswidget.h"
#include "customemoji/administratorcustomemojiwidget.h"
#include "customsounds/administratorcustomsoundswidget.h"
#include "customuserstatus/administratorcustomuserstatuswidget.h"
#include "invites/administratorinviteswidget.h"
#include "rocketchataccount.h"
#include "rooms/administratorroomswidget.h"
#include "serverinfo/administratorserverinfowidget.h"
#include "users/administratoruserswidget.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

AdministratorWidget::AdministratorWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTabWidget(new QTabWidget(this))
    , mAdministratorRoomsWidget(new AdministratorRoomsWidget(account, this))
    , mAdministratorCustomUserStatusWidget(new AdministratorCustomUserStatusWidget(account, this))
    , mAdministratorCustomSoundsWidget(new AdministratorCustomSoundsWidget(account, this))
    , mAdministratorServerInfoWidget(new AdministratorServerInfoWidget(account, this))
    , mAdministratorUsersWidget(new AdministratorUsersWidget(account, this))
    , mAdministratorInvitesWidget(new AdministratorInvitesWidget(account, this))
    , mAdministratorCustomEmojiWidget(new AdministratorCustomEmojiWidget(account, this))
    , mViewLogWidget(new ViewLogWidget(account, this))
    , mPermissionsWidget(new PermissionsWidget(account, this))
    , mRolesWidget(new AdministratorRolesWidget(account, this))
    , mOauthWidget(new AdministratorOauthWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    mainLayout->addWidget(mTabWidget);

    mAdministratorServerInfoWidget->setObjectName(QStringLiteral("mAdministratorServerInfoWidget"));
    mServerInfoTabIndex = mTabWidget->addTab(mAdministratorServerInfoWidget, i18n("Server Info"));

    mAdministratorRoomsWidget->setObjectName(QStringLiteral("mAdministratorRoomsWidget"));
    mTabWidget->addTab(mAdministratorRoomsWidget, i18n("Rooms"));

    mAdministratorCustomUserStatusWidget->setObjectName(QStringLiteral("mAdministratorCustomUserStatusWidget"));
    mTabWidget->addTab(mAdministratorCustomUserStatusWidget, i18n("Custom User Status"));

    mAdministratorCustomSoundsWidget->setObjectName(QStringLiteral("mAdministratorCustomSoundsWidget"));
    mCustomSoundsTabIndex = mTabWidget->addTab(mAdministratorCustomSoundsWidget, i18n("Custom Sounds"));

    mAdministratorCustomEmojiWidget->setObjectName(QStringLiteral("mAdministratorCustomEmojiWidget"));
    mTabWidget->addTab(mAdministratorCustomEmojiWidget, i18n("Custom Emojis"));

    mAdministratorUsersWidget->setObjectName(QStringLiteral("mAdministratorUsersWidget"));
    mTabWidget->addTab(mAdministratorUsersWidget, i18n("Users"));

    mAdministratorInvitesWidget->setObjectName(QStringLiteral("mAdministratorInvitesWidget"));
    mTabWidget->addTab(mAdministratorInvitesWidget, i18n("Invites"));

    mViewLogWidget->setObjectName(QStringLiteral("mViewLogWidget"));
    mViewLogTagIndex = mTabWidget->addTab(mViewLogWidget, i18n("View Log"));

    mPermissionsWidget->setObjectName(QStringLiteral("mPermissionsWidget"));
    mTabWidget->addTab(mPermissionsWidget, i18n("Permissions"));

    mRolesWidget->setObjectName(QStringLiteral("mRolesWidget"));
    mTabWidget->addTab(mRolesWidget, i18n("Roles"));

    mOauthWidget->setObjectName(QStringLiteral("mOauthWidget"));
    mTabWidget->addTab(mOauthWidget, i18n("Oauth"));
}

AdministratorWidget::~AdministratorWidget() = default;

void AdministratorWidget::initialize()
{
    mAdministratorUsersWidget->initialize();
    mAdministratorRoomsWidget->initialize();
    mAdministratorCustomSoundsWidget->initialize();
    mAdministratorCustomEmojiWidget->initialize();
    mPermissionsWidget->initialize();
    mAdministratorCustomUserStatusWidget->initialize();
    mRolesWidget->initialize();
    mAdministratorServerInfoWidget->initialize();
    mAdministratorInvitesWidget->initialize();
    mOauthWidget->initialize();
}

void AdministratorWidget::updateUiFromPermission()
{
    if (!mRocketChatAccount->hasPermission(QStringLiteral("manage-sounds"))) {
        mTabWidget->setTabVisible(mCustomSoundsTabIndex, false);
    }

    if (!mRocketChatAccount->hasPermission(QStringLiteral("view-statistics"))) {
        mTabWidget->setTabVisible(mServerInfoTabIndex, false);
    }
    if (!mRocketChatAccount->hasPermission(QStringLiteral("view-logs"))) {
        mTabWidget->setTabVisible(mViewLogTagIndex, false);
    }
}
