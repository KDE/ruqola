/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "appsmarketplaceinfotest.h"
#include <QTest>
QTEST_GUILESS_MAIN(AppsMarketPlaceInfoTest)
AppsMarketPlaceInfoTest::AppsMarketPlaceInfoTest(QObject *parent)
    : QObject{parent}
{
}

#include "moc_appsmarketplaceinfotest.cpp"
