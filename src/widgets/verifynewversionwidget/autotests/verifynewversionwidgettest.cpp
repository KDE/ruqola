/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "verifynewversionwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mCheckVersionResultLabel = w.findChild<QLabel *>(u"mCheckVersionResultLabel"_s);
    QVERIFY(mCheckVersionResultLabel);
    QVERIFY(mCheckVersionResultLabel->openExternalLinks());
    QVERIFY(mCheckVersionResultLabel->wordWrap());
    QCOMPARE(mCheckVersionResultLabel->textFormat(), Qt::RichText);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());
}

#include "moc_verifynewversionwidgettest.cpp"
