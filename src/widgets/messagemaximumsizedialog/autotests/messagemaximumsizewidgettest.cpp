/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizewidgettest.h"
#include "messagemaximumsizedialog/messagemaximumsizewidget.h"
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MessageMaximumSizeWidgetTest)
MessageMaximumSizeWidgetTest::MessageMaximumSizeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MessageMaximumSizeWidgetTest::shouldHaveDefaultValues()
{
    MessageMaximumSizeWidget d;

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mFileName = d.findChild<QLineEdit *>(QStringLiteral("mFileName"));
    QVERIFY(mFileName);
    QVERIFY(mFileName->text().isEmpty());

    auto mDescription = d.findChild<QLineEdit *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());

    QVERIFY(d.description().isEmpty());
    QVERIFY(d.fileName().isEmpty());
}
