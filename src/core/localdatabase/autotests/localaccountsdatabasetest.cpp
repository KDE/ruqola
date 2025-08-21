/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountsdatabasetest.h"

#include "localdatabase/localaccountsdatabase.h"
#include <QFile>
#include <QStandardPaths>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
enum class AccountFields {
    AccountName,
    Json,
}; // in the same order as the table

static QString accountName()
{
    return u"myAccount"_s;
}

QTEST_MAIN(LocalAccountsDatabaseTest)
LocalAccountsDatabaseTest::LocalAccountsDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalAccountsDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalAccountsDatabase accountDataBase;
    QFile::remove(accountDataBase.dbFileName(accountName()));
}

void LocalAccountsDatabaseTest::shouldHaveDefaultValues()
{
    LocalAccountsDatabase accountDataBase;
    QCOMPARE(accountDataBase.schemaDatabaseStr(), u"CREATE TABLE ACCOUNT (accountName TEXT PRIMARY KEY NOT NULL, json TEXT)"_s);
}

void LocalAccountsDatabaseTest::shouldVerifyDbFileName()
{
    LocalAccountsDatabase accountDataBase;
    QCOMPARE(accountDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/database/account/myAccount/myAccount.sqlite"_s);
}

void LocalAccountsDatabaseTest::shouldStoreAccountSettings()
{
    LocalAccountsDatabase accountDataBase;
    {
        const QByteArray ba = "{}";
        accountDataBase.updateAccount(accountName(), ba);

        // WHEN
        const QByteArray getInfo = accountDataBase.jsonAccount(accountName());

        // THEN
        QCOMPARE(getInfo, ba);
    }
    {
        const QByteArray ba = "{bla:\"bli\"}";
        accountDataBase.updateAccount(accountName(), ba);

        // WHEN
        const QByteArray getInfo = accountDataBase.jsonAccount(accountName());

        // THEN
        QCOMPARE(getInfo, ba);
    }
    {
        const QByteArray ba = "{}";
        accountDataBase.updateAccount(accountName(), ba);

        // WHEN
        const QByteArray getInfo = accountDataBase.jsonAccount(accountName());

        // THEN
        QCOMPARE(getInfo, ba);
    }
}

void LocalAccountsDatabaseTest::shouldRemoveAccountSettings()
{
    {
        LocalAccountsDatabase accountDataBase;
        const QByteArray ba = "{}";
        accountDataBase.updateAccount(accountName(), ba);

        // Verify that we have account info stored
        QVERIFY(!accountDataBase.jsonAccount(accountName()).isEmpty());

        // WHEN
        const QByteArray getInfo = accountDataBase.jsonAccount(accountName());

        // THEN
        QCOMPARE(getInfo, ba);

        accountDataBase.deleteAccount(accountName());

        QVERIFY(accountDataBase.jsonAccount(accountName()).isEmpty());
    }
}

#include "moc_localaccountsdatabasetest.cpp"
