/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ratelimiterwidgettest.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QTest>
QTEST_MAIN(RateLimiterWidgetTest)
RateLimiterWidgetTest::RateLimiterWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RateLimiterWidgetTest::shouldHaveDefaultValues()
{
    RateLimiterWidget w(nullptr);

    auto apiRateLimiterLabel = w.findChild<QLabel *>(QStringLiteral("apiRateLimiterLabel"));
    QVERIFY(apiRateLimiterLabel);
    QVERIFY(!apiRateLimiterLabel->text().isEmpty());

    auto mEnableRateLimiter = w.findChild<QCheckBox *>(QStringLiteral("mEnableRateLimiter"));
    QVERIFY(mEnableRateLimiter);
    QVERIFY(!mEnableRateLimiter->isChecked());
    QVERIFY(!mEnableRateLimiter->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableRateLimiter), QStringLiteral("API_Enable_Rate_Limiter"));
}
