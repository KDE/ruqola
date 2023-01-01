/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagescomboboxtest.h"
#include "misc/systemmessagescombobox.h"
#include <QTest>
QTEST_MAIN(SystemMessagesComboBoxTest)

SystemMessagesComboBoxTest::SystemMessagesComboBoxTest(QObject *parent)
    : QObject(parent)
{
}

void SystemMessagesComboBoxTest::shouldHaveDefaultValue()
{
    SystemMessagesComboBox w;
    QVERIFY(w.count() > 0);
    QVERIFY(w.systemMessagesSelected().isEmpty());
}
