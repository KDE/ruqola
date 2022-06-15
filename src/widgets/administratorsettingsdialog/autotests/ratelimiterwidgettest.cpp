/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ratelimiterwidgettest.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "settingswidgetshelper.h"
#include <QTest>
QTEST_MAIN(RateLimiterWidgetTest)
RateLimiterWidgetTest::RateLimiterWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RateLimiterWidgetTest::shouldHaveDefaultValues()
{
    RateLimiterWidget w(nullptr);
    // TODO
}
