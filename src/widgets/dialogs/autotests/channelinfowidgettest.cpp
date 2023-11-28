/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfowidgettest.h"
#include "dialogs/channelinfowidget.h"
#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ChannelInfoWidgetTest)
ChannelInfoWidgetTest::ChannelInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoWidgetTest::shouldHaveDefaultValues()
{
    ChannelInfoWidget w(nullptr, nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
}

#include "moc_channelinfowidgettest.cpp"
