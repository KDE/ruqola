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

#include "administratorcustomuserstatuswidgettest.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuswidget.h"
#include <KTreeWidgetSearchLineWidget>
#include <QTest>
#include <QTreeWidget>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorCustomUserStatusWidgetTest)

AdministratorCustomUserStatusWidgetTest::AdministratorCustomUserStatusWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomUserStatusWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomUserStatusWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCustomUserStatusTreeWidget = w.findChild<QTreeWidget *>(QStringLiteral("mCustomUserStatusTreeWidget"));
    QVERIFY(mCustomUserStatusTreeWidget);

    auto mSearchLineWidget = w.findChild<KTreeWidgetSearchLineWidget *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
}
