/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    QVERIFY(mName);

    auto mSelectFile = w.findChild<KUrlRequester *>(u"mSelectFile"_s);
    QVERIFY(mSelectFile);
}

#include "moc_administratorcustomsoundscreatewidgettest.cpp"
