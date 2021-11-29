/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "rolesmodeltest.h"
#include "model/rolesmodel.h"
#include <QTest>
QTEST_MAIN(RolesModelTest)
RolesModelTest::RolesModelTest(QObject *parent)
    : QObject(parent)
{
}

void RolesModelTest::shouldHaveDefaultValues()
{
    RolesModel w;
    QCOMPARE(w.rowCount(), 0);
}
