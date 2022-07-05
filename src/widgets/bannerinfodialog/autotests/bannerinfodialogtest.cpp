/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfodialogtest.h"
#include "bannerinfodialog/bannerinfodialog.h"
#include <QTest>

QTEST_MAIN(BannerInfoDialogTest)
BannerInfoDialogTest::BannerInfoDialogTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoDialogTest::shouldHaveDefaultValues()
{
    BannerInfoDialog w;
    // TODO
}
