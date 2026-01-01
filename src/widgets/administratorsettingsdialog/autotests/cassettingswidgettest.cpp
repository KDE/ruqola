/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cassettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/cas/cassettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
QTEST_MAIN(CasSettingsWidgetTest)
CasSettingsWidgetTest::CasSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CasSettingsWidgetTest::shouldHaveDefaultValues()
{
    CasSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(u"mEnabled"_s);
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), u"CAS_enabled"_s);

    auto mTrustCASUsername = w.findChild<QCheckBox *>(u"mTrustCASUsername"_s);
    QVERIFY(mTrustCASUsername);
    QVERIFY(!mTrustCASUsername->isChecked());
    QVERIFY(!mTrustCASUsername->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTrustCASUsername), u"CAS_trust_username"_s);

    auto mAllowUserCreation = w.findChild<QCheckBox *>(u"mAllowUserCreation"_s);
    QVERIFY(mAllowUserCreation);
    QVERIFY(!mAllowUserCreation->isChecked());
    QVERIFY(!mAllowUserCreation->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUserCreation), u"CAS_Creation_User_Enabled"_s);

    auto mSSOBaseURL = w.findChild<QLineEdit *>(u"mSSOBaseURL"_s);
    QVERIFY(mSSOBaseURL);
    QVERIFY(mSSOBaseURL->text().isEmpty());
    QVERIFY(!mSSOBaseURL->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSSOBaseURL), u"CAS_base_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"CAS_base_url"_s);

    auto mSSOLoginURL = w.findChild<QLineEdit *>(u"mSSOLoginURL"_s);
    QVERIFY(mSSOLoginURL);
    QVERIFY(mSSOLoginURL->text().isEmpty());
    QVERIFY(!mSSOLoginURL->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSSOLoginURL), u"CAS_login_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"CAS_login_url"_s);

    auto attributeHandlingLabel = w.findChild<QLabel *>(u"attributeHandlingLabel"_s);
    QVERIFY(attributeHandlingLabel);
    QVERIFY(!attributeHandlingLabel->text().isEmpty());

    auto mCASVersion = w.findChild<QComboBox *>(u"mCASVersion"_s);
    QVERIFY(mCASVersion);
    QCOMPARE(mCASVersion->count(), 2);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mCASVersion), u"CAS_version"_s);

    auto mAlwaysSyncUserData = w.findChild<QCheckBox *>(u"mAlwaysSyncUserData"_s);
    QVERIFY(mAlwaysSyncUserData);
    QVERIFY(!mAlwaysSyncUserData->isChecked());
    QVERIFY(!mAlwaysSyncUserData->text().isEmpty());
    QVERIFY(!mAlwaysSyncUserData->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAlwaysSyncUserData), u"CAS_Sync_User_Data_Enabled"_s);

    auto mAttributeMap = w.findChild<QLineEdit *>(u"mAttributeMap"_s);
    QVERIFY(mAttributeMap);
    QVERIFY(mAttributeMap->text().isEmpty());
    QVERIFY(!mAttributeMap->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAttributeMap), u"CAS_Sync_User_Data_FieldMap"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"CAS_Sync_User_Data_FieldMap"_s);
}

#include "moc_cassettingswidgettest.cpp"
