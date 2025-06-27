/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/conferencecall/conferencecallsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QFormLayout>

QTEST_MAIN(ConferenceCallSettingsWidgetTest)
ConferenceCallSettingsWidgetTest::ConferenceCallSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceCallSettingsWidgetTest::shouldHaveDefaultValues()
{
    ConferenceCallSettingsWidget w(nullptr);
    auto mDefaultProvider = w.findChild<QComboBox *>(u"mDefaultProvider"_s);
    QVERIFY(mDefaultProvider);
    QVERIFY(mDefaultProvider->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDefaultProvider), u"VideoConf_Default_Provider"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"VideoConf_Default_Provider"_s);

    auto mEnableDirectMessage = w.findChild<QCheckBox *>(u"mEnableDirectMessage"_s);
    QVERIFY(mEnableDirectMessage);
    QVERIFY(!mEnableDirectMessage->isChecked());
    QVERIFY(!mEnableDirectMessage->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableDirectMessage), u"VideoConf_Enable_DMs"_s);

    auto mEnablePublicChannels = w.findChild<QCheckBox *>(u"mEnablePublicChannels"_s);
    QVERIFY(mEnablePublicChannels);
    QVERIFY(!mEnablePublicChannels->isChecked());
    QVERIFY(!mEnablePublicChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePublicChannels), u"VideoConf_Enable_Channels"_s);

    auto mEnablePrivateChannels = w.findChild<QCheckBox *>(u"mEnablePrivateChannels"_s);
    QVERIFY(mEnablePrivateChannels);
    QVERIFY(!mEnablePrivateChannels->isChecked());
    QVERIFY(!mEnablePrivateChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePrivateChannels), u"VideoConf_Enable_Groups"_s);

    auto mEnableTeams = w.findChild<QCheckBox *>(u"mEnableTeams"_s);
    QVERIFY(mEnableTeams);
    QVERIFY(!mEnableTeams->isChecked());
    QVERIFY(!mEnableTeams->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableTeams), u"VideoConf_Enable_Teams"_s);
}

#include "moc_conferencecallsettingswidgettest.cpp"
