/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    ChannelSearchWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);

    auto mChannelSearchNameLineEdit = w.findChild<ChannelSearchNameLineEdit *>(QStringLiteral("mChannelSearchNameLineEdit"));
    QVERIFY(mChannelSearchNameLineEdit);
    // TODO
}
