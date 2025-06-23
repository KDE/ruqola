/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfowidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/directchannelinfowidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(DirectChannelInfoWidgetTest)
DirectChannelInfoWidgetTest::DirectChannelInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void DirectChannelInfoWidgetTest::shouldHaveDefaultValues()
{
    DirectChannelInfoWidget w(nullptr);

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAvatar = w.findChild<QLabel *>(u"mAvatar"_s);
    QVERIFY(mAvatar);
    QVERIFY(mAvatar->text().isEmpty());
}

#include "moc_directchannelinfowidgettest.cpp"
