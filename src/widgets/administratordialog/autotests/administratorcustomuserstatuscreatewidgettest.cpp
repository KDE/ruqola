/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuscreatewidgettest.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.h"
#include "misc/statuscombobox.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorCustomUserStatusCreateWidgetTest)
AdministratorCustomUserStatusCreateWidgetTest::AdministratorCustomUserStatusCreateWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomUserStatusCreateWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomUserStatusCreateWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QVERIFY(mName->isClearButtonEnabled());

    auto mStatusCombobox = w.findChild<StatusCombobox *>(QStringLiteral("mStatusCombobox"));
    QVERIFY(mStatusCombobox);
    QVERIFY(!w.userStatusInfo().isValid());
}

#include "moc_administratorcustomuserstatuscreatewidgettest.cpp"
