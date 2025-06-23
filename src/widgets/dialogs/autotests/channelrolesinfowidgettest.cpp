/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelrolesinfowidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/channelrolesinfowidget.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QTest>
QTEST_MAIN(ChannelRolesInfoWidgetTest)

ChannelRolesInfoWidgetTest::ChannelRolesInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ChannelRolesInfoWidgetTest::shouldHaveDefaultValues()
{
    ChannelRolesInfoWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto box = w.findChild<QGroupBox *>(u"box"_s);
    QVERIFY(box);
    QVERIFY(!box->title().isEmpty());

    auto mFormLayout = w.findChild<QFormLayout *>(u"mFormLayout"_s);
    QVERIFY(mFormLayout);
}

#include "moc_channelrolesinfowidgettest.cpp"
