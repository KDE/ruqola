/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCustomUserStatusTreeWidget = w.findChild<QTreeWidget *>(u"mCustomUserStatusTreeWidget"_s);
    QVERIFY(mCustomUserStatusTreeWidget);

    auto mSearchLineWidget = w.findChild<KTreeWidgetSearchLineWidget *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);
}

#include "moc_administratorcustomuserstatuswidgettest.cpp"
