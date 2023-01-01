/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolescomboboxtest.h"
#include "misc/rolescombobox.h"
#include <QTest>
QTEST_MAIN(RolesComboBoxTest)

RolesComboBoxTest::RolesComboBoxTest(QObject *parent)
    : QObject(parent)
{
}

void RolesComboBoxTest::shouldHaveDefaultValues()
{
    RolesComboBox w;
    QVERIFY(w.roles().isEmpty());
    QCOMPARE(w.sizeAdjustPolicy(), QComboBox::AdjustToContents);
}
