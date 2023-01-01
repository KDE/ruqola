/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ratelimiterwidgettest.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QSpinBox>
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

    auto ddpRateLimiterLabel = w.findChild<QLabel *>(QStringLiteral("ddpRateLimiterLabel"));
    QVERIFY(ddpRateLimiterLabel);
    QVERIFY(!ddpRateLimiterLabel->text().isEmpty());

    auto mLimitByIP = w.findChild<QCheckBox *>(QStringLiteral("mLimitByIP"));
    QVERIFY(mLimitByIP);
    QVERIFY(!mLimitByIP->isChecked());
    QVERIFY(!mLimitByIP->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByIP), QStringLiteral("DDP_Rate_Limit_IP_Enabled"));

    auto mLimitByUser = w.findChild<QCheckBox *>(QStringLiteral("mLimitByUser"));
    QVERIFY(mLimitByUser);
    QVERIFY(!mLimitByUser->isChecked());
    QVERIFY(!mLimitByUser->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByUser), QStringLiteral("DDP_Rate_Limit_User_Enabled"));

    auto mLimitByConnection = w.findChild<QCheckBox *>(QStringLiteral("mLimitByConnection"));
    QVERIFY(mLimitByConnection);
    QVERIFY(!mLimitByConnection->isChecked());
    QVERIFY(!mLimitByConnection->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByConnection), QStringLiteral("DDP_Rate_Limit_Connection_Enabled"));

    auto mLimiteByIpRequestsAllowed = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByIpRequestsAllowed"));
    QVERIFY(mLimiteByIpRequestsAllowed);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByIpRequestsAllowed), QStringLiteral("DDP_Rate_Limit_IP_Requests_Allowed"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_IP_Requests_Allowed"));

    auto mLimiteByIpIntervalTime = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByIpIntervalTime"));
    QVERIFY(mLimiteByIpIntervalTime);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByIpIntervalTime), QStringLiteral("DDP_Rate_Limit_IP_Interval_Time"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_IP_Interval_Time"));

    auto mLimiteByUserRequestsAllowed = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByUserRequestsAllowed"));
    QVERIFY(mLimiteByUserRequestsAllowed);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserRequestsAllowed), QStringLiteral("DDP_Rate_Limit_User_Requests_Allowed"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_User_Requests_Allowed"));

    auto mLimiteByUserIntervalTime = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByUserIntervalTime"));
    QVERIFY(mLimiteByUserIntervalTime);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserIntervalTime), QStringLiteral("DDP_Rate_Limit_User_Interval_Time"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_User_Interval_Time"));

    auto mLimiteByConnectionRequestsAllowed = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByConnectionRequestsAllowed"));
    QVERIFY(mLimiteByConnectionRequestsAllowed);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionRequestsAllowed), QStringLiteral("DDP_Rate_Limit_Connection_Requests_Allowed"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_Connection_Requests_Allowed"));

    auto mLimiteByConnectionIntervalTime = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByConnectionIntervalTime"));
    QVERIFY(mLimiteByConnectionIntervalTime);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionIntervalTime), QStringLiteral("DDP_Rate_Limit_Connection_Interval_Time"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_Connection_Interval_Time"));

    auto mLimitByUserPerMethod = w.findChild<QCheckBox *>(QStringLiteral("mLimitByUserPerMethod"));
    QVERIFY(mLimitByUserPerMethod);
    QVERIFY(!mLimitByUserPerMethod->isChecked());
    QVERIFY(!mLimitByUserPerMethod->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByUserPerMethod), QStringLiteral("DDP_Rate_Limit_User_By_Method_Enabled"));

    auto mLimiteByUserRequestsAllowedPerMethod = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByUserRequestsAllowedPerMethod"));
    QVERIFY(mLimiteByUserRequestsAllowedPerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserRequestsAllowedPerMethod), QStringLiteral("DDP_Rate_Limit_User_By_Method_Requests_Allowed"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_User_By_Method_Requests_Allowed"));

    auto mLimiteByUserIntervalTimePerMethod = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByUserIntervalTimePerMethod"));
    QVERIFY(mLimiteByUserIntervalTimePerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserIntervalTimePerMethod), QStringLiteral("DDP_Rate_Limit_User_By_Method_Interval_Time"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_User_By_Method_Interval_Time"));

    auto mLimitByConnectionPerMethod = w.findChild<QCheckBox *>(QStringLiteral("mLimitByConnectionPerMethod"));
    QVERIFY(mLimitByConnectionPerMethod);
    QVERIFY(!mLimitByConnectionPerMethod->isChecked());
    QVERIFY(!mLimitByConnectionPerMethod->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByConnectionPerMethod), QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Enabled"));

    auto mLimiteByConnectionRequestsAllowedPerMethod = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByConnectionRequestsAllowedPerMethod"));
    QVERIFY(mLimiteByConnectionRequestsAllowedPerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionRequestsAllowedPerMethod),
             QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Requests_Allowed"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Requests_Allowed"));

    auto mLimiteByConnectionIntervalTimePerMethod = w.findChild<QSpinBox *>(QStringLiteral("mLimiteByConnectionIntervalTimePerMethod"));
    QVERIFY(mLimiteByConnectionIntervalTimePerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionIntervalTimePerMethod),
             QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Interval_Time"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Interval_Time"));
}
