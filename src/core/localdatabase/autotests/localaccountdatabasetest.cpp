/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountdatabasetest.h"
#include "localdatabase/localaccountdatabase.h"
#include <QFile>
#include <QStandardPaths>
#include <QTest>
static QString accountName()
{
    return QStringLiteral("myAccount");
}
QTEST_MAIN(LocalAccountDatabaseTest)
LocalAccountDatabaseTest::LocalAccountDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalAccountDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalAccountDatabase accountDataBase;
    QFile::remove(accountDataBase.dbFileName(accountName()));
}
