/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolesmodeltest.h"
#include "model/rolesmodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(RolesModelTest)
RolesModelTest::RolesModelTest(QObject *parent)
    : QObject(parent)
{
}

void RolesModelTest::shouldHaveDefaultValues()
{
    RolesModel w;
    QCOMPARE(w.rowCount(), 0);
}

#include "moc_rolesmodeltest.cpp"
