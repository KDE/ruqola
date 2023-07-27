/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managelocaldatabasetest.h"
#include "managelocaldatabase.h"
#include <QTest>
QTEST_MAIN(ManageLocalDatabaseTest)
ManageLocalDatabaseTest::ManageLocalDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void ManageLocalDatabaseTest::shouldHaveDefaultValues()
{
    ManageLocalDatabase d(nullptr);
    // TODO
}

#include "moc_managelocaldatabasetest.cpp"
