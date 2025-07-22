/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorwidget.h"
#include "administratordialog/featurepreview/featurepreviewwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/logs/viewlogwidget.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include "administratordialog/oauth/administratoroauthwidget.h"
#include "administratordialog/permissions/permissionswidget.h"
#include "administratordialog/roles/administratorroleswidget.h"
#include "administratordialog/users/administratoruserstabwidget.h"
#include "customemoji/administratorcustomemojiwidget.h"
#include "customsounds/administratorcustomsoundswidget.h"
#include "customuserstatus/administratorcustomuserstatuswidget.h"
#include "invites/administratorinviteswidget.h"
#include "rocketchataccount.h"
#include "rooms/administratorroomswidget.h"
#include "serverinfo/administratorserverinfowidget.h"
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
    , mAdministratorUsersWidget(new AdministratorUsersTabWidget(account, this))
    , mAdministratorInvitesWidget(new AdministratorInvitesWidget(account, this))
    , mAdministratorCustomEmojiWidget(new AdministratorCustomEmojiWidget(account, this))
    , mViewLogWidget(new ViewLogWidget(account, this))
    , mPermissionsWidget(new PermissionsWidget(account, this))
    , mRolesWidget(new AdministratorRolesWidget(account, this))
    , mOauthWidget(new AdministratorOauthWidget(account, this))
    , mAdministratorModerationConsoleWidget(new AdministratorModerationConsoleWidget(account, this))
    , mFeaturePreviewWidget(new FeaturePreviewWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(u"mTabWidget"_s);
    mainLayout->addWidget(mTabWidget);

    mAdministratorServerInfoWidget->setObjectName(u"mAdministratorServerInfoWidget"_s);
    mTabWidget->addTab(mAdministratorServerInfoWidget, i18n("Server Info"));

    mAdministratorRoomsWidget->setObjectName(u"mAdministratorRoomsWidget"_s);
    mTabWidget->addTab(mAdministratorRoomsWidget, i18n("Rooms"));

    mAdministratorCustomUserStatusWidget->setObjectName(u"mAdministratorCustomUserStatusWidget"_s);
    mTabWidget->addTab(mAdministratorCustomUserStatusWidget, i18n("Custom User Status"));

    mAdministratorCustomSoundsWidget->setObjectName(u"mAdministratorCustomSoundsWidget"_s);
    mTabWidget->addTab(mAdministratorCustomSoundsWidget, i18n("Custom Sounds"));

    mAdministratorCustomEmojiWidget->setObjectName(u"mAdministratorCustomEmojiWidget"_s);
    mTabWidget->addTab(mAdministratorCustomEmojiWidget, i18n("Custom Emojis"));

    mAdministratorUsersWidget->setObjectName(u"mAdministratorUsersWidget"_s);
    mTabWidget->addTab(mAdministratorUsersWidget, i18n("Users"));

    mAdministratorInvitesWidget->setObjectName(u"mAdministratorInvitesWidget"_s);
    mTabWidget->addTab(mAdministratorInvitesWidget, i18n("Invites"));

    mViewLogWidget->setObjectName(u"mViewLogWidget"_s);
    mTabWidget->addTab(mViewLogWidget, i18n("View Log"));

    mPermissionsWidget->setObjectName(u"mPermissionsWidget"_s);
    mTabWidget->addTab(mPermissionsWidget, i18n("Permissions"));

    mRolesWidget->setObjectName(u"mRolesWidget"_s);
    mTabWidget->addTab(mRolesWidget, i18n("Roles"));

    mOauthWidget->setObjectName(u"mOauthWidget"_s);
    mTabWidget->addTab(mOauthWidget, i18n("Oauth"));

    mFeaturePreviewWidget->setObjectName(u"mFeaturePreviewWidget"_s);
    mTabWidget->addTab(mFeaturePreviewWidget, i18n("Feature Preview"));

    mAdministratorModerationConsoleWidget->setObjectName(u"mAdministratorModerationConsoleWidget"_s);
    mTabWidget->addTab(mAdministratorModerationConsoleWidget, i18n("Moderation Console"));
}

AdministratorWidget::~AdministratorWidget() = default;

void AdministratorWidget::initialize()
{
    if (mRocketChatAccount->hasPermission(u"view-user-administration"_s)) {
        mAdministratorUsersWidget->initialize();
    }
    if (mRocketChatAccount->hasPermission(u"view-room-administration"_s)) {
        mAdministratorRoomsWidget->initialize();
    }
    if (mRocketChatAccount->hasPermission(u"manage-sounds"_s)) {
        mAdministratorCustomSoundsWidget->initialize();
    }
    if (mRocketChatAccount->hasPermission(u"manage-emoji"_s)) {
        mAdministratorCustomEmojiWidget->initialize();
    }
    mPermissionsWidget->initialize();
    mAdministratorCustomUserStatusWidget->initialize();
    mRolesWidget->initialize();
    if (mRocketChatAccount->hasPermission(u"view-statistics"_s)) {
        mAdministratorServerInfoWidget->initialize();
    }
    if (mRocketChatAccount->hasPermission(u"create-invite-links"_s)) {
        mAdministratorInvitesWidget->initialize();
    }
    if (mRocketChatAccount->hasPermission(u"manage-oauth-apps"_s)) {
        mOauthWidget->initialize();
    }
    if (mRocketChatAccount->hasPermission(u"view-moderation-console"_s)) {
        mAdministratorModerationConsoleWidget->initialize();
    }
    mFeaturePreviewWidget->initialize();
}

void AdministratorWidget::updateUiFromPermission()
{
    if (!mRocketChatAccount->hasPermission(u"manage-sounds"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mAdministratorCustomSoundsWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"view-statistics"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mAdministratorServerInfoWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"view-logs"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mViewLogWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"manage-oauth-apps"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mOauthWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"view-moderation-console"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mAdministratorModerationConsoleWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"manage-emoji"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mAdministratorCustomEmojiWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"create-invite-links"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mAdministratorInvitesWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"view-room-administration"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mAdministratorRoomsWidget), false);
    }
    if (!mRocketChatAccount->hasPermission(u"view-user-administration"_s)) {
        mTabWidget->setTabVisible(mTabWidget->indexOf(mAdministratorUsersWidget), false);
    }
}

#include "moc_administratorwidget.cpp"
