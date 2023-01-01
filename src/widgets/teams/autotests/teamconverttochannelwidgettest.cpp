/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochannelwidgettest.h"
#include "teams/teamconverttochannelwidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamConvertToChannelWidgetTest)
TeamConvertToChannelWidgetTest::TeamConvertToChannelWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void TeamConvertToChannelWidgetTest::shouldHaveDefaultValues()
{
    TeamConvertToChannelWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto deleteLabel = w.findChild<QLabel *>(QStringLiteral("deleteLabel"));
    QVERIFY(deleteLabel);
    QVERIFY(!deleteLabel->text().isEmpty());
    QVERIFY(deleteLabel->wordWrap());
}
