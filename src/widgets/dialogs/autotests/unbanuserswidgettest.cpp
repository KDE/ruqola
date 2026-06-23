/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "unbanuserswidgettest.h"
#include "dialogs/unbanuserswidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(UnbanUsersWidgetTest)
using namespace Qt::Literals::StringLiterals;
UnbanUsersWidgetTest::UnbanUsersWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void UnbanUsersWidgetTest::shouldHaveDefaultValues()
{
    const UnbanUsersWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}
