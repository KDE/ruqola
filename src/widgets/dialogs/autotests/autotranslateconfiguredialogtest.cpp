/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfiguredialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/autotranslateconfiguredialog.h"
#include "dialogs/autotranslateconfigurewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AutoTranslateConfigureDialogTest)

AutoTranslateConfigureDialogTest::AutoTranslateConfigureDialogTest(QObject *parent)
    : QObject(parent)
{
}

void AutoTranslateConfigureDialogTest::shouldHaveDefaultValues()
{
    AutoTranslateConfigureDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAutoTranslateConfigureWidget = w.findChild<AutoTranslateConfigureWidget *>(u"mAutoTranslateConfigureWidget"_s);
    QVERIFY(mAutoTranslateConfigureWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
}

#include "moc_autotranslateconfiguredialogtest.cpp"
