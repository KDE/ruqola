/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
