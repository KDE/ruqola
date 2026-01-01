/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfowidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "bannerinfodialog/bannerinfowidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(BannerInfoWidgetTest)
BannerInfoWidgetTest::BannerInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoWidgetTest::shouldHaveDefaultValues()
{
    BannerInfoWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_bannerinfowidgettest.cpp"
