/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountdatabasetest.h"
#include <QStandardPaths>
#include <QTest>
QTEST_MAIN(LocalAccountDatabaseTest)
LocalAccountDatabaseTest::LocalAccountDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalAccountDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    // LocalGlobalDatabase globalDataBase;
    // QFile::remove(globalDataBase.dbFileName(accountName()));
}
