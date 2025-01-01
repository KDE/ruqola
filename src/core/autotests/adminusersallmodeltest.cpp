/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "model/adminusersallmodel.h"
#include "adminusersallmodeltest.h"
#include <QTest>
QTEST_MAIN(AdminUsersAllModelTest)

AdminUsersAllModelTest::AdminUsersAllModelTest(QObject *parent)
    : QObject(parent)
{
}

void AdminUsersAllModelTest::shouldHaveDefaultValues()
{
    AdminUsersAllModel m;
    // TODO
}

#include "moc_adminusersallmodeltest.cpp"
