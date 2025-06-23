/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsearchnamelineresultwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/channelsearchnamelineresultwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(ChannelSearchNameLineResultWidgetTest)

ChannelSearchNameLineResultWidgetTest::ChannelSearchNameLineResultWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelSearchNameLineResultWidgetTest::shouldHaveDefaultValues()
{
    ChannelSearchNameLineResultWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mLabel = w.findChild<QLabel *>(u"mLabel"_s);
    QVERIFY(mLabel);
    QVERIFY(mLabel->text().isEmpty());

    auto mClearToolButton = w.findChild<QToolButton *>(u"mClearToolButton"_s);
    QVERIFY(mClearToolButton);
    QVERIFY(!mClearToolButton->toolTip().isEmpty());
}

#include "moc_channelsearchnamelineresultwidgettest.cpp"
