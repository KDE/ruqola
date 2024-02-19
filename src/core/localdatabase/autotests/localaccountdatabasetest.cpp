/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

void LocalAccountDatabaseTest::shouldHaveDefaultValues()
{
    LocalAccountDatabase accountDataBase;
    QCOMPARE(accountDataBase.schemaDatabaseStr(), QStringLiteral("CREATE TABLE ACCOUNT (accountName TEXT PRIMARY KEY NOT NULL, json TEXT)"));
}

void LocalAccountDatabaseTest::shouldVerifyDbFileName()
{
    LocalAccountDatabase accountDataBase;
    QCOMPARE(accountDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/database/account/myAccount/myAccount.sqlite"));
}

void LocalAccountDatabaseTest::shouldStoreAccountSettings()
{
    {
        LocalAccountDatabase accountDataBase;
        const QByteArray ba = "{}";
        accountDataBase.updateAccount(accountName(), ba);

        // WHEN
        const QByteArray getInfo = accountDataBase.jsonAccount(accountName());

        // THEN
        QCOMPARE(getInfo, ba);
    }
    {
        LocalAccountDatabase accountDataBase;
        const QByteArray ba = "{bla:\"bli\"}";
        accountDataBase.updateAccount(accountName(), ba);

        // WHEN
        const QByteArray getInfo = accountDataBase.jsonAccount(accountName());

        // THEN
        QCOMPARE(getInfo, ba);
    }
}

void LocalAccountDatabaseTest::shouldRemoveAccountSettings()
{
    {
        LocalAccountDatabase accountDataBase;
        const QByteArray ba = "{}";
        accountDataBase.updateAccount(accountName(), ba);
        // WHEN
        const QByteArray getInfo = accountDataBase.jsonAccount(accountName());

        // THEN
        QCOMPARE(getInfo, ba);

        accountDataBase.deleteAccount(accountName());

        QVERIFY(accountDataBase.jsonAccount(accountName()).isEmpty());
    }
}

#include "moc_localaccountdatabasetest.cpp"
