/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasedialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    auto mExploreDatabaseWidget = d.findChild<ExploreDatabaseWidget *>(u"mExploreDatabaseWidget"_s);
    QVERIFY(mExploreDatabaseWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_exploredatabasedialogtest.cpp"
