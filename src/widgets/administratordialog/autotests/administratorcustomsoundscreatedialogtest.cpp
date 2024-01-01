/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatedialogtest.h"
#include "administratordialog/customsounds/administratorcustomsoundscreatedialog.h"
#include "administratordialog/customsounds/administratorcustomsoundscreatewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorCustomSoundsCreateDialogTest)
AdministratorCustomSoundsCreateDialogTest::AdministratorCustomSoundsCreateDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorCustomSoundsCreateDialogTest::shouldHaveDefaultValues()
{
    AdministratorCustomSoundsCreateDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorCustomSoundsCreateWidget *>(QStringLiteral("mCreateWidget"));
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}

#include "moc_administratorcustomsoundscreatedialogtest.cpp"
