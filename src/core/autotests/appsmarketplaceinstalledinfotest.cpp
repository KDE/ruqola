/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinstalledinfotest.h"
#include "apps/appsmarketplaceinstalledinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AppsMarketPlaceInstalledInfoTest)

AppsMarketPlaceInstalledInfoTest::AppsMarketPlaceInstalledInfoTest(QObject *parent)
    : QObject{parent}
{
}

void AppsMarketPlaceInstalledInfoTest::shouldHaveDefaultValues()
{
    AppsMarketPlaceInstalledInfo w;
    // TODO
}
