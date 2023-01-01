/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsearchnamelineresultwidgettest.h"
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
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mLabel = w.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);
    QVERIFY(mLabel->text().isEmpty());

    auto mClearToolButton = w.findChild<QToolButton *>(QStringLiteral("mClearToolButton"));
    QVERIFY(mClearToolButton);
    QVERIFY(!mClearToolButton->toolTip().isEmpty());
}
