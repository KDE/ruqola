/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "analyticswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/analytics/analyticswidget.h"
#include "settingswidgetshelper.h"
#include <QLineEdit>
#include <QPlainTextEdit>
QTEST_MAIN(AnalyticsWidgetTest)
AnalyticsWidgetTest::AnalyticsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AnalyticsWidgetTest::shouldHaveDefaultValues()
{
    AnalyticsWidget w(nullptr);

    auto mMessages = w.findChild<QCheckBox *>(u"mMessages"_s);
    QVERIFY(mMessages);
    QVERIFY(!mMessages->isChecked());
    QVERIFY(!mMessages->text().isEmpty());
    QVERIFY(!mMessages->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMessages), u"Analytics_features_messages"_s);

    auto mRooms = w.findChild<QCheckBox *>(u"mRooms"_s);
    QVERIFY(mRooms);
    QVERIFY(!mRooms->isChecked());
    QVERIFY(!mRooms->text().isEmpty());
    QVERIFY(!mRooms->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRooms), u"Analytics_features_rooms"_s);

    auto mUsers = w.findChild<QCheckBox *>(u"mUsers"_s);
    QVERIFY(mUsers);
    QVERIFY(!mUsers->isChecked());
    QVERIFY(!mUsers->text().isEmpty());
    QVERIFY(!mUsers->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUsers), u"Analytics_features_users"_s);

    auto mGoogleEnabled = w.findChild<QCheckBox *>(u"mGoogleEnabled"_s);
    QVERIFY(mGoogleEnabled);
    QVERIFY(!mGoogleEnabled->isChecked());
    QVERIFY(!mGoogleEnabled->text().isEmpty());
    QVERIFY(mGoogleEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleEnabled), u"GoogleAnalytics_enabled"_s);

    auto mGoogleTrackingId = w.findChild<QLineEdit *>(u"mGoogleTrackingId"_s);
    QVERIFY(mGoogleTrackingId);
    QVERIFY(mGoogleTrackingId->text().isEmpty());
    QVERIFY(mGoogleTrackingId->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleTrackingId), u"GoogleAnalytics_ID"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"GoogleAnalytics_ID"_s);

    auto mPiwikEnabled = w.findChild<QCheckBox *>(u"mPiwikEnabled"_s);
    QVERIFY(mPiwikEnabled);
    QVERIFY(!mPiwikEnabled->isChecked());
    QVERIFY(!mPiwikEnabled->text().isEmpty());
    QVERIFY(mPiwikEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPiwikEnabled), u"PiwikAnalytics_enabled"_s);

    auto mPiwikUrl = w.findChild<QLineEdit *>(u"mPiwikUrl"_s);
    QVERIFY(mPiwikUrl);
    QVERIFY(mPiwikUrl->text().isEmpty());
    QVERIFY(!mPiwikUrl->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPiwikUrl), u"PiwikAnalytics_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"PiwikAnalytics_url"_s);

    auto mPiwikClientID = w.findChild<QLineEdit *>(u"mPiwikClientID"_s);
    QVERIFY(mPiwikClientID);
    QVERIFY(mPiwikClientID->text().isEmpty());
    QVERIFY(!mPiwikClientID->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPiwikClientID), u"PiwikAnalytics_siteId"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"PiwikAnalytics_siteId"_s);

    auto mPrependDomain = w.findChild<QCheckBox *>(u"mPrependDomain"_s);
    QVERIFY(mPrependDomain);
    QVERIFY(!mPrependDomain->isChecked());
    QVERIFY(!mPrependDomain->text().isEmpty());
    QVERIFY(!mPrependDomain->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPrependDomain), u"PiwikAnalytics_prependDomain"_s);

    auto mAllSubdomains = w.findChild<QCheckBox *>(u"mAllSubdomains"_s);
    QVERIFY(mAllSubdomains);
    QVERIFY(!mAllSubdomains->isChecked());
    QVERIFY(!mAllSubdomains->text().isEmpty());
    QVERIFY(!mAllSubdomains->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllSubdomains), u"PiwikAnalytics_cookieDomain"_s);

    auto mAdditionalPiwikSites = w.findChild<QPlainTextEdit *>(u"mAdditionalPiwikSites"_s);
    QVERIFY(mAdditionalPiwikSites);
    QVERIFY(mAdditionalPiwikSites->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAdditionalPiwikSites), u"PiwikAdditionalTrackers"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"PiwikAdditionalTrackers"_s);

    auto mHideOutgoingLinks = w.findChild<QPlainTextEdit *>(u"mHideOutgoingLinks"_s);
    QVERIFY(mHideOutgoingLinks);
    QVERIFY(mHideOutgoingLinks->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHideOutgoingLinks), u"PiwikAnalytics_domains"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"PiwikAnalytics_domains"_s);
}

#include "moc_analyticswidgettest.cpp"
