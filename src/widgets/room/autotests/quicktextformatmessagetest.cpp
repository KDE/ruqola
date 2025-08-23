/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "quicktextformatmessagetest.h"
#include "room/quicktextformatmessage.h"
#include <QHBoxLayout>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(QuickTextFormatMessageTest)
using namespace Qt::Literals::StringLiterals;
QuickTextFormatMessageTest::QuickTextFormatMessageTest(QObject *parent)
    : QObject{parent}
{
}

void QuickTextFormatMessageTest::shouldHaveDefaultValues()
{
    const QuickTextFormatMessage t(nullptr);
    auto mainLayout = t.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QCOMPARE(mainLayout->spacing(), 0);

    auto boldButton = t.findChild<QToolButton *>(u"boldButton"_s);
    QCOMPARE(boldButton->iconSize(), QSize(12, 12));
    QVERIFY(boldButton->autoRaise());

    auto italicButton = t.findChild<QToolButton *>(u"italicButton"_s);
    QCOMPARE(italicButton->iconSize(), QSize(12, 12));
    QVERIFY(italicButton->autoRaise());

    auto strikeThroughButton = t.findChild<QToolButton *>(u"strikeThroughButton"_s);
    QCOMPARE(strikeThroughButton->iconSize(), QSize(12, 12));
    QVERIFY(strikeThroughButton->autoRaise());

    auto blockQuoteButton = t.findChild<QToolButton *>(u"blockQuoteButton"_s);
    QCOMPARE(blockQuoteButton->iconSize(), QSize(12, 12));
    QVERIFY(blockQuoteButton->autoRaise());

    auto insertLinkButton = t.findChild<QToolButton *>(u"insertLinkButton"_s);
    QCOMPARE(insertLinkButton->iconSize(), QSize(12, 12));
    QVERIFY(insertLinkButton->autoRaise());
}

#include "moc_quicktextformatmessagetest.cpp"
