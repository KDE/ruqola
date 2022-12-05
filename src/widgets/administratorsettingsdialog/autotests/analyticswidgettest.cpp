/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "analyticswidgettest.h"
#include "administratorsettingsdialog/analytics/analyticswidget.h"
#include "settingswidgetshelper.h"
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AnalyticsWidgetTest)
AnalyticsWidgetTest::AnalyticsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AnalyticsWidgetTest::shouldHaveDefaultValues()
{
    AnalyticsWidget w(nullptr);

    auto mMessages = w.findChild<QCheckBox *>(QStringLiteral("mMessages"));
    QVERIFY(mMessages);
    QVERIFY(!mMessages->isChecked());
    QVERIFY(!mMessages->text().isEmpty());
    QVERIFY(!mMessages->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMessages), QStringLiteral("Analytics_features_messages"));

    auto mRooms = w.findChild<QCheckBox *>(QStringLiteral("mRooms"));
    QVERIFY(mRooms);
    QVERIFY(!mRooms->isChecked());
    QVERIFY(!mRooms->text().isEmpty());
    QVERIFY(!mRooms->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRooms), QStringLiteral("Analytics_features_rooms"));

    auto mUsers = w.findChild<QCheckBox *>(QStringLiteral("mUsers"));
    QVERIFY(mUsers);
    QVERIFY(!mUsers->isChecked());
    QVERIFY(!mUsers->text().isEmpty());
    QVERIFY(!mUsers->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUsers), QStringLiteral("Analytics_features_users"));

    auto mGoogleEnabled = w.findChild<QCheckBox *>(QStringLiteral("mGoogleEnabled"));
    QVERIFY(mGoogleEnabled);
    QVERIFY(!mGoogleEnabled->isChecked());
    QVERIFY(!mGoogleEnabled->text().isEmpty());
    QVERIFY(mGoogleEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleEnabled), QStringLiteral("GoogleAnalytics_enabled"));

    auto mGoogleTrackingId = w.findChild<QLineEdit *>(QStringLiteral("mGoogleTrackingId"));
    QVERIFY(mGoogleTrackingId);
    QVERIFY(mGoogleTrackingId->text().isEmpty());
    QVERIFY(mGoogleTrackingId->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleTrackingId), QStringLiteral("GoogleAnalytics_ID"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("GoogleAnalytics_ID"));

    auto mPiwikEnabled = w.findChild<QCheckBox *>(QStringLiteral("mPiwikEnabled"));
    QVERIFY(mPiwikEnabled);
    QVERIFY(!mPiwikEnabled->isChecked());
    QVERIFY(!mPiwikEnabled->text().isEmpty());
    QVERIFY(mPiwikEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPiwikEnabled), QStringLiteral("PiwikAnalytics_enabled"));

    auto mPiwikUrl = w.findChild<QLineEdit *>(QStringLiteral("mPiwikUrl"));
    QVERIFY(mPiwikUrl);
    QVERIFY(mPiwikUrl->text().isEmpty());
    QVERIFY(!mPiwikUrl->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPiwikUrl), QStringLiteral("PiwikAnalytics_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("PiwikAnalytics_url"));

    auto mPiwikClientID = w.findChild<QLineEdit *>(QStringLiteral("mPiwikClientID"));
    QVERIFY(mPiwikClientID);
    QVERIFY(mPiwikClientID->text().isEmpty());
    QVERIFY(!mPiwikClientID->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPiwikClientID), QStringLiteral("PiwikAnalytics_siteId"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("PiwikAnalytics_siteId"));
}
