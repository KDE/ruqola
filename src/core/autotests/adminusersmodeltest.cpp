/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminusersmodeltest.h"
#include "model/adminusersmodel.h"
#include <QTest>
QTEST_MAIN(AdminUsersModelTest)

AdminUsersModelTest::AdminUsersModelTest(QObject *parent)
    : QObject(parent)
{
}

void AdminUsersModelTest::shouldHaveDefaultValues()
{
    AdminUsersModel m;
    // TODO
}

#include "moc_adminusersmodeltest.cpp"
