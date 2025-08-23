/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "quicktextformatmessagetest.h"
#include "room/quicktextformatmessage.h"
#include <QHBoxLayout>
#include <QTest>
QTEST_MAIN(QuickTextFormatMessageTest)
using namespace Qt::Literals::StringLiterals;
QuickTextFormatMessageTest::QuickTextFormatMessageTest(QObject *parent)
    : QObject{parent}
{
}

void QuickTextFormatMessageTest::shouldHaveDefaultValues()
{
    const QuickTextFormatMessage t;
    auto mainLayout = t.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QCOMPARE(mainLayout->spacing(), 0);
    // TODO
}
