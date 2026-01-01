/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "encryption/e2edisablewidget.h"
#include <KSeparator>
#include <QLabel>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(E2eDisableWidgetTest)

E2eDisableWidgetTest::E2eDisableWidgetTest(QObject *parent)
    : QObject{parent}
{
}

E2eDisableWidgetTest::~E2eDisableWidgetTest() = default;

void E2eDisableWidgetTest::shouldHaveDefaultValues()
{
    E2eDisableWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto separator = w.findChild<KSeparator *>(u"separator"_s);
    QVERIFY(separator);

    auto labelReset = w.findChild<QLabel *>(u"labelReset"_s);
    QVERIFY(labelReset);
    QVERIFY(!labelReset->text().isEmpty());

    auto pushButton = w.findChild<QPushButton *>(u"pushButton"_s);
    QVERIFY(pushButton);
    QVERIFY(!pushButton->text().isEmpty());
}

#include "moc_e2edisablewidgettest.cpp"
