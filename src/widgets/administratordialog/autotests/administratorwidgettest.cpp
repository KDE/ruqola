/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorwidgettest.h"
#include "administratordialog/administratorwidget.h"
#include "administratordialog/customemoji/administratorcustomemojiwidget.h"
#include "administratordialog/customsounds/administratorcustomsoundswidget.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuswidget.h"
#include "administratordialog/invites/administratorinviteswidget.h"
#include "administratordialog/logs/viewlogwidget.h"
#include "administratordialog/permissions/permissionswidget.h"
#include "administratordialog/rooms/administratorroomswidget.h"
#include "administratordialog/serverinfo/administratorserverinfowidget.h"
#include "administratordialog/users/administratoruserswidget.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(QStringLiteral("mTabWidget"));
    QVERIFY(mTabWidget);

    auto mAdministratorRoomsWidget = w.findChild<AdministratorRoomsWidget *>(QStringLiteral("mAdministratorRoomsWidget"));
    QVERIFY(mAdministratorRoomsWidget);

    auto mAdministratorCustomUserStatusWidget = w.findChild<AdministratorCustomUserStatusWidget *>(QStringLiteral("mAdministratorCustomUserStatusWidget"));
    QVERIFY(mAdministratorCustomUserStatusWidget);

    auto mAdministratorCustomSoundsWidget = w.findChild<AdministratorCustomSoundsWidget *>(QStringLiteral("mAdministratorCustomSoundsWidget"));
    QVERIFY(mAdministratorCustomSoundsWidget);

    auto mAdministratorServerInfoWidget = w.findChild<AdministratorServerInfoWidget *>(QStringLiteral("mAdministratorServerInfoWidget"));
    QVERIFY(mAdministratorServerInfoWidget);

    auto mAdministratorUsersWidget = w.findChild<AdministratorUsersWidget *>(QStringLiteral("mAdministratorUsersWidget"));
    QVERIFY(mAdministratorUsersWidget);

    auto mAdministratorInvitesWidget = w.findChild<AdministratorInvitesWidget *>(QStringLiteral("mAdministratorInvitesWidget"));
    QVERIFY(mAdministratorInvitesWidget);

    auto mViewLogWidget = w.findChild<ViewLogWidget *>(QStringLiteral("mViewLogWidget"));
    QVERIFY(mViewLogWidget);

    auto mAdministratorCustomEmojiWidget = w.findChild<AdministratorCustomEmojiWidget *>(QStringLiteral("mAdministratorCustomEmojiWidget"));
    QVERIFY(mAdministratorCustomEmojiWidget);

    auto mPermissionsWidget = w.findChild<PermissionsWidget *>(QStringLiteral("mPermissionsWidget"));
    QVERIFY(mPermissionsWidget);
}
