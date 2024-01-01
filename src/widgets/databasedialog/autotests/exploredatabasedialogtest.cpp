/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasedialogtest.h"
#include "databasedialog/exploredatabasedialog.h"
#include "databasedialog/exploredatabasewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ExploreDatabaseDialogTest)
ExploreDatabaseDialogTest::ExploreDatabaseDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExploreDatabaseDialogTest::shouldHaveDefaultValues()
{
    ExploreDatabaseDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mExploreDatabaseWidget = d.findChild<ExploreDatabaseWidget *>(QStringLiteral("mExploreDatabaseWidget"));
    QVERIFY(mExploreDatabaseWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_exploredatabasedialogtest.cpp"
