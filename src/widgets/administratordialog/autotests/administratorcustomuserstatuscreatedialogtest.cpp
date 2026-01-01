/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuscreatedialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/customuserstatus/administratorcustomuserstatuscreatedialog.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorCustomUserStatusCreateDialogTest)

AdministratorCustomUserStatusCreateDialogTest::AdministratorCustomUserStatusCreateDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorCustomUserStatusCreateDialogTest::shouldHaveDefaultValues()
{
    AdministratorCustomUserStatusCreateDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorCustomUserStatusCreateWidget *>(u"mCreateWidget"_s);
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);

    QVERIFY(!w.userStatusInfo().isValid());
}

#include "moc_administratorcustomuserstatuscreatedialogtest.cpp"
