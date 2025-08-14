/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasedialogtest.h"

#include "databasedialog/exploredatabaseaccountstoragewidget.h"
#include "databasedialog/exploredatabasedialog.h"
#include "databasedialog/exploredatabasejsonwidget.h"
#include "databasedialog/exploredatabasemessagesstoragewidget.h"
#include "databasedialog/exploredatabaseviewmessageswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
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

    auto tabWidget = d.findChild<QTabWidget *>(u"tabWidget"_s);
    QVERIFY(tabWidget);

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mExploreDatabaseWidget = d.findChild<ExploreDatabaseViewMessagesWidget *>(u"mExploreDatabaseWidget"_s);
    QVERIFY(mExploreDatabaseWidget);

    auto mExploreDatabaseJsonWidget = d.findChild<ExploreDatabaseJsonWidget *>(u"mExploreDatabaseJsonWidget"_s);
    QVERIFY(mExploreDatabaseJsonWidget);

    auto mExploreDatabaseStorageWidget = d.findChild<ExploreDatabaseMessagesStorageWidget *>(u"mExploreDatabaseStorageWidget"_s);
    QVERIFY(mExploreDatabaseStorageWidget);

    auto mExploreDatabaseAccountStorageWidget = d.findChild<ExploreDatabaseAccountStorageWidget *>(u"mExploreDatabaseAccountStorageWidget"_s);
    QVERIFY(mExploreDatabaseAccountStorageWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_exploredatabasedialogtest.cpp"
