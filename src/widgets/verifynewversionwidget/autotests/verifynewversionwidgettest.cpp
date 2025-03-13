/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "verifynewversionwidgettest.h"
#include "verifynewversionwidget/verifynewversionwidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(VerifyNewVersionWidgetTest)

VerifyNewVersionWidgetTest::VerifyNewVersionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void VerifyNewVersionWidgetTest::shouldHaveDefaultValues()
{
    VerifyNewVersionWidget w;
    auto mCheckVersionResultLabel = w.findChild<QLabel *>(QStringLiteral("mCheckVersionResultLabel"));
    QVERIFY(mCheckVersionResultLabel);
    QVERIFY(mCheckVersionResultLabel->openExternalLinks());
    QVERIFY(mCheckVersionResultLabel->wordWrap());
    QCOMPARE(mCheckVersionResultLabel->textFormat(), Qt::RichText);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());
}
