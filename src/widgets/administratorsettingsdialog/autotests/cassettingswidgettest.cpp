/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cassettingswidgettest.h"
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
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("CAS_enabled"));

    auto mTrustCASUsername = w.findChild<QCheckBox *>(QStringLiteral("mTrustCASUsername"));
    QVERIFY(mTrustCASUsername);
    QVERIFY(!mTrustCASUsername->isChecked());
    QVERIFY(!mTrustCASUsername->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTrustCASUsername), QStringLiteral("CAS_trust_username"));

    auto mAllowUserCreation = w.findChild<QCheckBox *>(QStringLiteral("mAllowUserCreation"));
    QVERIFY(mAllowUserCreation);
    QVERIFY(!mAllowUserCreation->isChecked());
    QVERIFY(!mAllowUserCreation->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUserCreation), QStringLiteral("CAS_Creation_User_Enabled"));

    auto mSSOBaseURL = w.findChild<QLineEdit *>(QStringLiteral("mSSOBaseURL"));
    QVERIFY(mSSOBaseURL);
    QVERIFY(mSSOBaseURL->text().isEmpty());
    QVERIFY(!mSSOBaseURL->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSSOBaseURL), QStringLiteral("CAS_base_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("CAS_base_url"));

    auto mSSOLoginURL = w.findChild<QLineEdit *>(QStringLiteral("mSSOLoginURL"));
    QVERIFY(mSSOLoginURL);
    QVERIFY(mSSOLoginURL->text().isEmpty());
    QVERIFY(!mSSOLoginURL->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSSOLoginURL), QStringLiteral("CAS_login_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("CAS_login_url"));

    auto attributeHandlingLabel = w.findChild<QLabel *>(QStringLiteral("attributeHandlingLabel"));
    QVERIFY(attributeHandlingLabel);
    QVERIFY(!attributeHandlingLabel->text().isEmpty());

    auto mCASVersion = w.findChild<QComboBox *>(QStringLiteral("mCASVersion"));
    QVERIFY(mCASVersion);
    QCOMPARE(mCASVersion->count(), 2);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mCASVersion), QStringLiteral("CAS_version"));

    auto mAlwaysSyncUserData = w.findChild<QCheckBox *>(QStringLiteral("mAlwaysSyncUserData"));
    QVERIFY(mAlwaysSyncUserData);
    QVERIFY(!mAlwaysSyncUserData->isChecked());
    QVERIFY(!mAlwaysSyncUserData->text().isEmpty());
    QVERIFY(!mAlwaysSyncUserData->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAlwaysSyncUserData), QStringLiteral("CAS_Sync_User_Data_Enabled"));

    auto mAttributeMap = w.findChild<QLineEdit *>(QStringLiteral("mAttributeMap"));
    QVERIFY(mAttributeMap);
    QVERIFY(mAttributeMap->text().isEmpty());
    QVERIFY(!mAttributeMap->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAttributeMap), QStringLiteral("CAS_Sync_User_Data_FieldMap"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("CAS_Sync_User_Data_FieldMap"));
}

#include "moc_cassettingswidgettest.cpp"
