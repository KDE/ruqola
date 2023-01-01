/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsearchwidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);

    auto mChannelSearchNameLineEdit = w.findChild<ChannelSearchNameLineEdit *>(QStringLiteral("mChannelSearchNameLineEdit"));
    QVERIFY(mChannelSearchNameLineEdit);
    // TODO
}
