/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistsearchlinewidgettest.h"
#include "bannerinfodialog/bannerinfolistsearchlinewidget.h"
#include <QTest>

QTEST_MAIN(BannerInfoListSearchLineWidgetTest)
BannerInfoListSearchLineWidgetTest::BannerInfoListSearchLineWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoListSearchLineWidgetTest::shouldHaveDefaultValues()
{
    BannerInfoListSearchLineWidget w;
    // TODO
}
