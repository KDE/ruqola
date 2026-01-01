/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatedialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorCustomSoundsCreateWidget *>(u"mCreateWidget"_s);
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
}

#include "moc_administratorcustomsoundscreatedialogtest.cpp"
