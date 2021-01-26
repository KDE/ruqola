/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "administratorwidgettest.h"
#include "administratordialog/administratorwidget.h"
#include "administratordialog/customsounds/administratorcustomsoundswidget.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuswidget.h"
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
    AdministratorWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto mTabWidget = w.findChild<QTabWidget *>(QStringLiteral("mTabWidget"));
    QVERIFY(mTabWidget);

    auto mAdministratorRoomsWidget = w.findChild<AdministratorRoomsWidget *>(QStringLiteral("mAdministratorRoomsWidget"));
    QVERIFY(mAdministratorRoomsWidget);

    auto mAdministratorCustomUserStatusWidget = w.findChild<AdministratorCustomUserStatusWidget *>(QStringLiteral("mAdministratorCustomUserStatusWidget"));
    QVERIFY(mAdministratorCustomUserStatusWidget);

    auto mAdministratorCustomSoundsWidget = w.findChild<AdministratorCustomSoundsWidget *>(QStringLiteral("mAdministratorCustomSoundsWidget"));
    QVERIFY(mAdministratorCustomSoundsWidget);

    // TODO we need to fix mRocketChatAccount::hasPermission in autotest.
    auto mAdministratorServerInfoWidget = w.findChild<AdministratorServerInfoWidget *>(QStringLiteral("mAdministratorServerInfoWidget"));
    QVERIFY(mAdministratorServerInfoWidget);

    auto mAdministratorUsersWidget = w.findChild<AdministratorUsersWidget *>(QStringLiteral("mAdministratorServerInfoWidget"));
    QVERIFY(mAdministratorUsersWidget);
}
