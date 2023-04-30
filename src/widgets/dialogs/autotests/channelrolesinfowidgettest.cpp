/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelrolesinfowidgettest.h"
#include "dialogs/channelrolesinfowidget.h"
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(ChannelRolesInfoWidgetTest)

ChannelRolesInfoWidgetTest::ChannelRolesInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ChannelRolesInfoWidgetTest::shouldHaveDefaultValues()
{
    ChannelRolesInfoWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}
