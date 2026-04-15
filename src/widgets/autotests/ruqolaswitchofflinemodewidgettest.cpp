/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaswitchofflinemodewidgettest.h"
#include "loginwidget/ruqolaswitchofflinemodewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTest>
QTEST_MAIN(RuqolaSwitchOfflineModeWidgetTest)
using namespace Qt::Literals::StringLiterals;
RuqolaSwitchOfflineModeWidgetTest::RuqolaSwitchOfflineModeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RuqolaSwitchOfflineModeWidgetTest::shouldHaveDefaultValues()
{
    const RuqolaSwitchOfflineModeWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto button = w.findChild<QPushButton *>(u"button"_s);
    QVERIFY(button);
    QVERIFY(!button->text().isEmpty());
}

#include "moc_ruqolaswitchofflinemodewidgettest.cpp"
