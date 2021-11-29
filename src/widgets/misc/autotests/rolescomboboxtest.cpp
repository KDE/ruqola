/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    // TODO
    QVERIFY(w.roles().isEmpty());
}
