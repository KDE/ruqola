/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlinewidgettest.h"
#include "dialogs/channelnamevalidlinewidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ChannelNameValidLineWidgetTest)
ChannelNameValidLineWidgetTest::ChannelNameValidLineWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ChannelNameValidLineWidgetTest::shouldHaveDefaultValues()
{
    ChannelNameValidLineWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QVERIFY(w.text().isEmpty());
}
