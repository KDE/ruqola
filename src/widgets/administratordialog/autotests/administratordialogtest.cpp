/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratordialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/administratordialog.h"
#include "administratordialog/administratorwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorDialogTest)
AdministratorDialogTest::AdministratorDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorDialogTest::shouldHaveDefaultValues()
{
    AdministratorDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAdministratorWidget = d.findChild<AdministratorWidget *>(u"mAdministratorWidget"_s);
    QVERIFY(mAdministratorWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_administratordialogtest.cpp"
