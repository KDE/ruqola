/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfiguredialogtest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAutoTranslateConfigureWidget = w.findChild<AutoTranslateConfigureWidget *>(QStringLiteral("mAutoTranslateConfigureWidget"));
    QVERIFY(mAutoTranslateConfigureWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}

#include "moc_autotranslateconfiguredialogtest.cpp"
