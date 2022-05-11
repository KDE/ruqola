/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidgettest.h"
#include "administratorsettingsdialog/encryption/encryptionsettingswidget.h"
#include <QTest>

QTEST_MAIN(EncryptionSettingsWidgetTest)
EncryptionSettingsWidgetTest::EncryptionSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EncryptionSettingsWidgetTest::shouldHaveDefaultValues()
{
    EncryptionSettingsWidget w;
    // TODO
}
