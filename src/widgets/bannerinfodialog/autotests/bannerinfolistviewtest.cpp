/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistviewtest.h"
#include "bannerinfodialog/bannerinfolistview.h"
#include <QTest>
QTEST_MAIN(BannerInfoListViewTest)
BannerInfoListViewTest::BannerInfoListViewTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoListViewTest::shouldHaveDefaultValues()
{
    BannerInfoListView w(nullptr);
    // TODO
}
