/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratordialogtest.h"
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

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAdministratorWidget = d.findChild<AdministratorWidget *>(QStringLiteral("mAdministratorWidget"));
    QVERIFY(mAdministratorWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_administratordialogtest.cpp"
