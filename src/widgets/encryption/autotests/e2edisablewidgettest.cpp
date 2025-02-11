/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto separator = w.findChild<KSeparator *>(QStringLiteral("separator"));
    QVERIFY(separator);

    auto labelReset = w.findChild<QLabel *>(QStringLiteral("labelReset"));
    QVERIFY(labelReset);
    QVERIFY(!labelReset->text().isEmpty());

    auto pushButton = w.findChild<QPushButton *>(QStringLiteral("pushButton"));
    QVERIFY(pushButton);
    QVERIFY(!pushButton->text().isEmpty());
}

#include "moc_e2edisablewidgettest.cpp"
