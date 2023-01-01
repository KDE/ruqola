/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatewidgettest.h"
#include "administratordialog/customsounds/administratorcustomsoundscreatewidget.h"
#include <KUrlRequester>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorCustomSoundsCreateWidgetTest)
AdministratorCustomSoundsCreateWidgetTest::AdministratorCustomSoundsCreateWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomSoundsCreateWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomSoundsCreateWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);

    auto mSelectFile = w.findChild<KUrlRequester *>(QStringLiteral("mSelectFile"));
    QVERIFY(mSelectFile);
}
