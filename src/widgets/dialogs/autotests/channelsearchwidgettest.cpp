/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsearchwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/channelsearchnamelineedit.h"
#include "dialogs/channelsearchwidget.h"
#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ChannelSearchWidgetTest)
ChannelSearchWidgetTest::ChannelSearchWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelSearchWidgetTest::shouldHaveDefaultValues()
{
    ChannelSearchWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);

    auto mChannelSearchNameLineEdit = w.findChild<ChannelSearchNameLineEdit *>(u"mChannelSearchNameLineEdit"_s);
    QVERIFY(mChannelSearchNameLineEdit);
    // TODO
}

#include "moc_channelsearchwidgettest.cpp"
