/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfostest.h"
#include "bannerinfo/bannerinfos.h"
#include <QTest>
QTEST_GUILESS_MAIN(BannerInfosTest)
BannerInfosTest::BannerInfosTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfosTest::shouldHaveDefaultValues()
{
    BannerInfos w;
}
