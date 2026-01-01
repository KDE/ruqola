/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/administratorwidget.h"
#include "administratordialog/customemoji/administratorcustomemojiwidget.h"
#include "administratordialog/customsounds/administratorcustomsoundswidget.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuswidget.h"
#include "administratordialog/featurepreview/featurepreviewwidget.h"
#include "administratordialog/invites/administratorinviteswidget.h"
#include "administratordialog/logs/viewlogwidget.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include "administratordialog/oauth/administratoroauthwidget.h"
#include "administratordialog/permissions/permissionswidget.h"
#include "administratordialog/roles/administratorroleswidget.h"
#include "administratordialog/rooms/administratorroomswidget.h"
#include "administratordialog/serverinfo/administratorserverinfowidget.h"
#include "administratordialog/users/administratoruserstabwidget.h"

#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorWidgetTest)
AdministratorWidgetTest::AdministratorWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorWidgetTest::shouldHaveDefaultValues()
{
    AdministratorWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(u"mTabWidget"_s);
    QVERIFY(mTabWidget);

    auto mAdministratorRoomsWidget = w.findChild<AdministratorRoomsWidget *>(u"mAdministratorRoomsWidget"_s);
    QVERIFY(mAdministratorRoomsWidget);

    auto mAdministratorCustomUserStatusWidget = w.findChild<AdministratorCustomUserStatusWidget *>(u"mAdministratorCustomUserStatusWidget"_s);
    QVERIFY(mAdministratorCustomUserStatusWidget);

    auto mAdministratorCustomSoundsWidget = w.findChild<AdministratorCustomSoundsWidget *>(u"mAdministratorCustomSoundsWidget"_s);
    QVERIFY(mAdministratorCustomSoundsWidget);

    auto mAdministratorServerInfoWidget = w.findChild<AdministratorServerInfoWidget *>(u"mAdministratorServerInfoWidget"_s);
    QVERIFY(mAdministratorServerInfoWidget);

    auto mAdministratorUsersWidget = w.findChild<AdministratorUsersTabWidget *>(u"mAdministratorUsersWidget"_s);
    QVERIFY(mAdministratorUsersWidget);

    auto mAdministratorInvitesWidget = w.findChild<AdministratorInvitesWidget *>(u"mAdministratorInvitesWidget"_s);
    QVERIFY(mAdministratorInvitesWidget);

    auto mViewLogWidget = w.findChild<ViewLogWidget *>(u"mViewLogWidget"_s);
    QVERIFY(mViewLogWidget);

    auto mAdministratorCustomEmojiWidget = w.findChild<AdministratorCustomEmojiWidget *>(u"mAdministratorCustomEmojiWidget"_s);
    QVERIFY(mAdministratorCustomEmojiWidget);

    auto mPermissionsWidget = w.findChild<PermissionsWidget *>(u"mPermissionsWidget"_s);
    QVERIFY(mPermissionsWidget);

    auto mRolesWidget = w.findChild<AdministratorRolesWidget *>(u"mRolesWidget"_s);
    QVERIFY(mRolesWidget);

    auto mOauthWidget = w.findChild<AdministratorOauthWidget *>(u"mOauthWidget"_s);
    QVERIFY(mOauthWidget);

    auto mFeaturePreviewWidget = w.findChild<FeaturePreviewWidget *>(u"mFeaturePreviewWidget"_s);
    QVERIFY(mFeaturePreviewWidget);

    auto mAdministratorModerationConsoleWidget = w.findChild<AdministratorModerationConsoleWidget *>(u"mAdministratorModerationConsoleWidget"_s);
    QVERIFY(mAdministratorModerationConsoleWidget);
}

#include "moc_administratorwidgettest.cpp"
