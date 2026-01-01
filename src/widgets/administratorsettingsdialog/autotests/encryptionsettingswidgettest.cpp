/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/encryption/encryptionsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>

QTEST_MAIN(EncryptionSettingsWidgetTest)
EncryptionSettingsWidgetTest::EncryptionSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EncryptionSettingsWidgetTest::shouldHaveDefaultValues()
{
    EncryptionSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mEnableE2E = w.findChild<QCheckBox *>(u"mEnableE2E"_s);
    QVERIFY(mEnableE2E);
    QVERIFY(!mEnableE2E->isChecked());
    QVERIFY(!mEnableE2E->text().isEmpty());
    QVERIFY(!mEnableE2E->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableE2E), u"E2E_Enable"_s);

    auto mEnableEncryptionDirectRoomsByDefault = w.findChild<QCheckBox *>(u"mEnableEncryptionDirectRoomsByDefault"_s);
    QVERIFY(mEnableEncryptionDirectRoomsByDefault);
    QVERIFY(!mEnableEncryptionDirectRoomsByDefault->isChecked());
    QVERIFY(!mEnableEncryptionDirectRoomsByDefault->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableEncryptionDirectRoomsByDefault), u"E2E_Enabled_Default_DirectRooms"_s);

    auto mEnableEncryptionPrivateRoomsByDefault = w.findChild<QCheckBox *>(u"mEnableEncryptionPrivateRoomsByDefault"_s);
    QVERIFY(mEnableEncryptionPrivateRoomsByDefault);
    QVERIFY(!mEnableEncryptionPrivateRoomsByDefault->isChecked());
    QVERIFY(!mEnableEncryptionPrivateRoomsByDefault->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableEncryptionPrivateRoomsByDefault), u"E2E_Enabled_Default_PrivateRooms"_s);

    auto mEnableEncryptFiles = w.findChild<QCheckBox *>(u"mEnableEncryptFiles"_s);
    QVERIFY(mEnableEncryptFiles);
    QVERIFY(!mEnableEncryptFiles->isChecked());
    QVERIFY(!mEnableEncryptFiles->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableEncryptFiles), u"E2E_Enable_Encrypt_Files"_s);

    auto mAllowUnencryptedMessages = w.findChild<QCheckBox *>(u"mAllowUnencryptedMessages"_s);
    QVERIFY(mAllowUnencryptedMessages);
    QVERIFY(!mAllowUnencryptedMessages->isChecked());
    QVERIFY(!mAllowUnencryptedMessages->text().isEmpty());
    QVERIFY(!mAllowUnencryptedMessages->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUnencryptedMessages), u"E2E_Allow_Unencrypted_Messages"_s);

    auto mEnableOtr = w.findChild<QCheckBox *>(u"mEnableOtr"_s);
    QVERIFY(mEnableOtr);
    QVERIFY(!mEnableOtr->isChecked());
    QVERIFY(!mEnableOtr->text().isEmpty());
    QVERIFY(!mEnableOtr->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableOtr), u"OTR_Enabled"_s);

    auto mEnableMentions = w.findChild<QCheckBox *>(u"mEnableMentions"_s);
    QVERIFY(mEnableMentions);
    QVERIFY(!mEnableMentions->isChecked());
    QVERIFY(!mEnableMentions->text().isEmpty());
    QVERIFY(!mEnableMentions->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableMentions), u"E2E_Enabled_Mentions"_s);
}

#include "moc_encryptionsettingswidgettest.cpp"
