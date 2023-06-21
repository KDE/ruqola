/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfowidgettest.h"
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

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAvatar = w.findChild<QLabel *>(QStringLiteral("mAvatar"));
    QVERIFY(mAvatar);
    QVERIFY(mAvatar->text().isEmpty());
}

#include "moc_directchannelinfowidgettest.cpp"
