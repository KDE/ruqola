/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasejsonwidgettest.h"
#include "databasedialog/exploredatabasejsonwidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ExploreDatabaseJsonWidgetTest)
ExploreDatabaseJsonWidgetTest::ExploreDatabaseJsonWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ExploreDatabaseJsonWidgetTest::shouldHaveDefaultValues()
{
    ExploreDatabaseJsonWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mPlainTextEdit = w.findChild<JsonPlainTextEditWidget *>(u"mPlainTextEdit"_s);
    QVERIFY(mPlainTextEdit);
}

#include "moc_exploredatabasejsonwidgettest.cpp"
