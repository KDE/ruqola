/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolescopecomboboxtest.h"
#include "administratordialog/roles/rolescopecombobox.h"
#include <QTest>
QTEST_MAIN(RoleScopeComboBoxTest)
RoleScopeComboBoxTest::RoleScopeComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

void RoleScopeComboBoxTest::shouldHaveDefaultValues()
{
    RoleScopeComboBox w;
    QCOMPARE(w.count(), 2);
}

#include "moc_rolescopecomboboxtest.cpp"
