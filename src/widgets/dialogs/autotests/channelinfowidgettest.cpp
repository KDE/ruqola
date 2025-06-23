/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfowidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);
}

#include "moc_channelinfowidgettest.cpp"
