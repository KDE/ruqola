/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "globaldatabasetest.h"
#include "localdatabase/globaldatabase.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(GlobalDatabaseTest)
static QString accountName()
{
    return QStringLiteral("myAccount");
}

static QString roomName()
{
    return QStringLiteral("myRoom");
}
static QString roomNameOther()
{
    return QStringLiteral("myOtherRoom");
}

enum class Fields {
    Identifier,
    TimeStamp,
}; // in the same order as the table

GlobalDatabaseTest::GlobalDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void GlobalDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    GlobalDatabase globalDataBase;
    QFile::remove(globalDataBase.dbFileName(accountName()));
}

void GlobalDatabaseTest::shouldStoreIdentifier()
{
    // GIVEN
    GlobalDatabase globalDataBase;
    int roomNameValue = 55;
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomName(), roomNameValue, GlobalDatabase::TimeStampType::MessageTimeStamp);
    int roomNameOtherValue = 12;
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomNameOther(), roomNameOtherValue, GlobalDatabase::TimeStampType::MessageTimeStamp);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomName(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameValue);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    // -1 if timeStamp doesn't exist
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::AccountTimeStamp), -1);
}

void GlobalDatabaseTest::shouldGenerateIdentifier()
{
    QFETCH(QString, accountName);
    QFETCH(QString, roomName);
    QFETCH(GlobalDatabase::TimeStampType, type);
    QFETCH(QString, result);

    GlobalDatabase globalDataBase;
    QCOMPARE(globalDataBase.generateIdentifier(accountName, roomName, type), result);
}

void GlobalDatabaseTest::shouldGenerateIdentifier_data()
{
    QTest::addColumn<QString>("accountName");
    QTest::addColumn<QString>("roomName");
    QTest::addColumn<GlobalDatabase::TimeStampType>("type");
    QTest::addColumn<QString>("result");

    QTest::addRow("empty") << QString() << QString() << GlobalDatabase::TimeStampType::MessageTimeStamp << QString();
    QTest::addRow("test1") << QStringLiteral("account1") << QStringLiteral("room1") << GlobalDatabase::TimeStampType::MessageTimeStamp
                           << QStringLiteral("messages-account1-room1");
    QTest::addRow("test2") << QStringLiteral("account2") << QStringLiteral("room2") << GlobalDatabase::TimeStampType::RoomTimeStamp
                           << QStringLiteral("rooms-account2-room2");
    QTest::addRow("test3") << QStringLiteral("account3") << QString() << GlobalDatabase::TimeStampType::AccountTimeStamp << QStringLiteral("account-account3");
}

void GlobalDatabaseTest::shouldVerifyDbFileName()
{
    GlobalDatabase accountDataBase;
    QCOMPARE(accountDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/database/global/myAccount/myAccount.sqlite"));
}

#include "moc_globaldatabasetest.cpp"
