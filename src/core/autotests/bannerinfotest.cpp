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
    // TODO
}
