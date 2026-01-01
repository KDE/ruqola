/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ratelimiterwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QSpinBox>
QTEST_MAIN(RateLimiterWidgetTest)
RateLimiterWidgetTest::RateLimiterWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RateLimiterWidgetTest::shouldHaveDefaultValues()
{
    RateLimiterWidget w(nullptr);

    auto apiRateLimiterLabel = w.findChild<QLabel *>(u"apiRateLimiterLabel"_s);
    QVERIFY(apiRateLimiterLabel);
    QVERIFY(!apiRateLimiterLabel->text().isEmpty());

    auto mEnableRateLimiter = w.findChild<QCheckBox *>(u"mEnableRateLimiter"_s);
    QVERIFY(mEnableRateLimiter);
    QVERIFY(!mEnableRateLimiter->isChecked());
    QVERIFY(!mEnableRateLimiter->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableRateLimiter), u"API_Enable_Rate_Limiter"_s);

    auto ddpRateLimiterLabel = w.findChild<QLabel *>(u"ddpRateLimiterLabel"_s);
    QVERIFY(ddpRateLimiterLabel);
    QVERIFY(!ddpRateLimiterLabel->text().isEmpty());

    auto mLimitByIP = w.findChild<QCheckBox *>(u"mLimitByIP"_s);
    QVERIFY(mLimitByIP);
    QVERIFY(!mLimitByIP->isChecked());
    QVERIFY(!mLimitByIP->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByIP), u"DDP_Rate_Limit_IP_Enabled"_s);

    auto mLimitByUser = w.findChild<QCheckBox *>(u"mLimitByUser"_s);
    QVERIFY(mLimitByUser);
    QVERIFY(!mLimitByUser->isChecked());
    QVERIFY(!mLimitByUser->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByUser), u"DDP_Rate_Limit_User_Enabled"_s);

    auto mLimitByConnection = w.findChild<QCheckBox *>(u"mLimitByConnection"_s);
    QVERIFY(mLimitByConnection);
    QVERIFY(!mLimitByConnection->isChecked());
    QVERIFY(!mLimitByConnection->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByConnection), u"DDP_Rate_Limit_Connection_Enabled"_s);

    auto mLimiteByIpRequestsAllowed = w.findChild<QSpinBox *>(u"mLimiteByIpRequestsAllowed"_s);
    QVERIFY(mLimiteByIpRequestsAllowed);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByIpRequestsAllowed), u"DDP_Rate_Limit_IP_Requests_Allowed"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_IP_Requests_Allowed"_s);

    auto mLimiteByIpIntervalTime = w.findChild<QSpinBox *>(u"mLimiteByIpIntervalTime"_s);
    QVERIFY(mLimiteByIpIntervalTime);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByIpIntervalTime), u"DDP_Rate_Limit_IP_Interval_Time"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_IP_Interval_Time"_s);

    auto mLimiteByUserRequestsAllowed = w.findChild<QSpinBox *>(u"mLimiteByUserRequestsAllowed"_s);
    QVERIFY(mLimiteByUserRequestsAllowed);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserRequestsAllowed), u"DDP_Rate_Limit_User_Requests_Allowed"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_User_Requests_Allowed"_s);

    auto mLimiteByUserIntervalTime = w.findChild<QSpinBox *>(u"mLimiteByUserIntervalTime"_s);
    QVERIFY(mLimiteByUserIntervalTime);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserIntervalTime), u"DDP_Rate_Limit_User_Interval_Time"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_User_Interval_Time"_s);

    auto mLimiteByConnectionRequestsAllowed = w.findChild<QSpinBox *>(u"mLimiteByConnectionRequestsAllowed"_s);
    QVERIFY(mLimiteByConnectionRequestsAllowed);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionRequestsAllowed), u"DDP_Rate_Limit_Connection_Requests_Allowed"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_Connection_Requests_Allowed"_s);

    auto mLimiteByConnectionIntervalTime = w.findChild<QSpinBox *>(u"mLimiteByConnectionIntervalTime"_s);
    QVERIFY(mLimiteByConnectionIntervalTime);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionIntervalTime), u"DDP_Rate_Limit_Connection_Interval_Time"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_Connection_Interval_Time"_s);

    auto mLimitByUserPerMethod = w.findChild<QCheckBox *>(u"mLimitByUserPerMethod"_s);
    QVERIFY(mLimitByUserPerMethod);
    QVERIFY(!mLimitByUserPerMethod->isChecked());
    QVERIFY(!mLimitByUserPerMethod->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByUserPerMethod), u"DDP_Rate_Limit_User_By_Method_Enabled"_s);

    auto mLimiteByUserRequestsAllowedPerMethod = w.findChild<QSpinBox *>(u"mLimiteByUserRequestsAllowedPerMethod"_s);
    QVERIFY(mLimiteByUserRequestsAllowedPerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserRequestsAllowedPerMethod), u"DDP_Rate_Limit_User_By_Method_Requests_Allowed"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_User_By_Method_Requests_Allowed"_s);

    auto mLimiteByUserIntervalTimePerMethod = w.findChild<QSpinBox *>(u"mLimiteByUserIntervalTimePerMethod"_s);
    QVERIFY(mLimiteByUserIntervalTimePerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByUserIntervalTimePerMethod), u"DDP_Rate_Limit_User_By_Method_Interval_Time"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_User_By_Method_Interval_Time"_s);

    auto mLimitByConnectionPerMethod = w.findChild<QCheckBox *>(u"mLimitByConnectionPerMethod"_s);
    QVERIFY(mLimitByConnectionPerMethod);
    QVERIFY(!mLimitByConnectionPerMethod->isChecked());
    QVERIFY(!mLimitByConnectionPerMethod->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByConnectionPerMethod), u"DDP_Rate_Limit_Connection_By_Method_Enabled"_s);

    auto mLimiteByConnectionRequestsAllowedPerMethod = w.findChild<QSpinBox *>(u"mLimiteByConnectionRequestsAllowedPerMethod"_s);
    QVERIFY(mLimiteByConnectionRequestsAllowedPerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionRequestsAllowedPerMethod), u"DDP_Rate_Limit_Connection_By_Method_Requests_Allowed"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_Connection_By_Method_Requests_Allowed"_s);

    auto mLimiteByConnectionIntervalTimePerMethod = w.findChild<QSpinBox *>(u"mLimiteByConnectionIntervalTimePerMethod"_s);
    QVERIFY(mLimiteByConnectionIntervalTimePerMethod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimiteByConnectionIntervalTimePerMethod), u"DDP_Rate_Limit_Connection_By_Method_Interval_Time"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DDP_Rate_Limit_Connection_By_Method_Interval_Time"_s);
}

#include "moc_ratelimiterwidgettest.cpp"
