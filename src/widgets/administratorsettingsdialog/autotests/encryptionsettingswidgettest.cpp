/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidgettest.h"
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

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEnableE2E = w.findChild<QCheckBox *>(QStringLiteral("mEnableE2E"));
    QVERIFY(mEnableE2E);
    QVERIFY(!mEnableE2E->isChecked());
    QVERIFY(!mEnableE2E->text().isEmpty());
    QVERIFY(!mEnableE2E->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableE2E), QStringLiteral("E2E_Enable"));

    auto mEnableEncryptionDirectRoomsByDefault = w.findChild<QCheckBox *>(QStringLiteral("mEnableEncryptionDirectRoomsByDefault"));
    QVERIFY(mEnableEncryptionDirectRoomsByDefault);
    QVERIFY(!mEnableEncryptionDirectRoomsByDefault->isChecked());
    QVERIFY(!mEnableEncryptionDirectRoomsByDefault->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableEncryptionDirectRoomsByDefault), QStringLiteral("E2E_Enabled_Default_DirectRooms"));

    auto mEnableEncryptionPrivateRoomsByDefault = w.findChild<QCheckBox *>(QStringLiteral("mEnableEncryptionPrivateRoomsByDefault"));
    QVERIFY(mEnableEncryptionPrivateRoomsByDefault);
    QVERIFY(!mEnableEncryptionPrivateRoomsByDefault->isChecked());
    QVERIFY(!mEnableEncryptionPrivateRoomsByDefault->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableEncryptionPrivateRoomsByDefault), QStringLiteral("E2E_Enabled_Default_PrivateRooms"));

    auto mEnableEncryptFiles = w.findChild<QCheckBox *>(QStringLiteral("mEnableEncryptFiles"));
    QVERIFY(mEnableEncryptFiles);
    QVERIFY(!mEnableEncryptFiles->isChecked());
    QVERIFY(!mEnableEncryptFiles->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableEncryptFiles), QStringLiteral("E2E_Enable_Encrypt_Files"));

    auto mAllowUnencryptedMessages = w.findChild<QCheckBox *>(QStringLiteral("mAllowUnencryptedMessages"));
    QVERIFY(mAllowUnencryptedMessages);
    QVERIFY(!mAllowUnencryptedMessages->isChecked());
    QVERIFY(!mAllowUnencryptedMessages->text().isEmpty());
    QVERIFY(!mAllowUnencryptedMessages->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUnencryptedMessages), QStringLiteral("E2E_Allow_Unencrypted_Messages"));

    auto mEnableOtr = w.findChild<QCheckBox *>(QStringLiteral("mEnableOtr"));
    QVERIFY(mEnableOtr);
    QVERIFY(!mEnableOtr->isChecked());
    QVERIFY(!mEnableOtr->text().isEmpty());
    QVERIFY(!mEnableOtr->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableOtr), QStringLiteral("OTR_Enabled"));
}

#include "moc_encryptionsettingswidgettest.cpp"
