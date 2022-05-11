/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidgettest.h"
#include "administratorsettingsdialog/encryption/encryptionsettingswidget.h"
#include <QCheckBox>
#include <QTest>

QTEST_MAIN(EncryptionSettingsWidgetTest)
EncryptionSettingsWidgetTest::EncryptionSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EncryptionSettingsWidgetTest::shouldHaveDefaultValues()
{
    EncryptionSettingsWidget w(nullptr);
    auto mEnableE2E = w.findChild<QCheckBox *>(QStringLiteral("mEnableE2E"));
    QVERIFY(mEnableE2E);
    QVERIFY(!mEnableE2E->isChecked());
    QVERIFY(!mEnableE2E->text().isEmpty());
    QVERIFY(!mEnableE2E->toolTip().isEmpty());

    auto mEnableEncryptionDirectRoomsByDefault = w.findChild<QCheckBox *>(QStringLiteral("mEnableEncryptionDirectRoomsByDefault"));
    QVERIFY(mEnableEncryptionDirectRoomsByDefault);
    QVERIFY(!mEnableEncryptionDirectRoomsByDefault->isChecked());
    QVERIFY(!mEnableEncryptionDirectRoomsByDefault->text().isEmpty());

    auto mEnableEncryptionPrivateRoomsByDefault = w.findChild<QCheckBox *>(QStringLiteral("mEnableEncryptionPrivateRoomsByDefault"));
    QVERIFY(mEnableEncryptionPrivateRoomsByDefault);
    QVERIFY(!mEnableEncryptionPrivateRoomsByDefault->isChecked());
    QVERIFY(!mEnableEncryptionPrivateRoomsByDefault->text().isEmpty());

    auto mEnableOtr = w.findChild<QCheckBox *>(QStringLiteral("mEnableOtr"));
    QVERIFY(mEnableOtr);
    QVERIFY(!mEnableOtr->isChecked());
    QVERIFY(!mEnableOtr->text().isEmpty());
}
