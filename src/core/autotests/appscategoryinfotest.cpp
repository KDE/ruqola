/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscategoryinfotest.h"
#include "apps/appscategoryinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(AppsCategoryInfoTest)
AppsCategoryInfoTest::AppsCategoryInfoTest(QObject *parent)
    : QObject{parent}
{
}

void AppsCategoryInfoTest::shouldHaveDefaultValues()
{
    AppsCategoryInfo d;

    // TODO
}

#include "moc_appscategoryinfotest.cpp"
