/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannerinfotest.h"
#include "bannerinfo/bannerinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(BannerInfoTest)
BannerInfoTest::BannerInfoTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoTest::shouldHaveDefaultValues()
{
    BannerInfo w;
    QVERIFY(!w.read());
    QVERIFY(w.text().isEmpty());
    QVERIFY(w.title().isEmpty());
    QVERIFY(w.link().isEmpty());
    QVERIFY(w.textArguments().isEmpty());
    QVERIFY(w.identifier().isEmpty());
    QCOMPARE(w.priority(), -1);
}
