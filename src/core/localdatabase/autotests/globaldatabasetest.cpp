/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "globaldatabasetest.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabase/globaldatabase.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(GlobalDatabaseTest)
static QString accountName()
{
    return u"myAccount"_s;
}

static QString roomName()
{
    return u"myRoom"_s;
}
static QString roomNameOther()
{
    return u"myOtherRoom"_s;
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
    const int roomNameValue = 55;
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomName(), roomNameValue, GlobalDatabase::TimeStampType::MessageTimeStamp);
    const int roomNameOtherValue = 12;
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
    QTest::addRow("test1") << u"account1"_s << u"room1"_s << GlobalDatabase::TimeStampType::MessageTimeStamp << u"messages-account1-room1"_s;
    QTest::addRow("test2") << u"account2"_s << u"room2"_s << GlobalDatabase::TimeStampType::RoomTimeStamp << u"rooms-account2-room2"_s;
    QTest::addRow("test3") << u"account3"_s << QString() << GlobalDatabase::TimeStampType::AccountTimeStamp << u"account-account3"_s;
}

void GlobalDatabaseTest::shouldVerifyDbFileName()
{
    GlobalDatabase accountDataBase;
    QCOMPARE(accountDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/database/global/myAccount/myAccount.sqlite"_s);
}

void GlobalDatabaseTest::shouldRemoveTimeStamp()
{
    GlobalDatabase globalDataBase;
    const int roomNameValue = 55;
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomName(), roomNameValue, GlobalDatabase::TimeStampType::MessageTimeStamp);
    const int roomNameOtherValue = 12;
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomNameOther(), roomNameOtherValue, GlobalDatabase::TimeStampType::MessageTimeStamp);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomName(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameValue);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    // Remove it.
    globalDataBase.removeTimeStamp(accountName(), roomName(), GlobalDatabase::TimeStampType::MessageTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomName(), GlobalDatabase::TimeStampType::MessageTimeStamp), -1);

    // OTher still exists
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    globalDataBase.removeTimeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::AccountTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    globalDataBase.removeTimeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::MessageTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomNameOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), -1);

    // RoomTimeStamp
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomName(), roomNameValue, GlobalDatabase::TimeStampType::RoomTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomName(), GlobalDatabase::TimeStampType::RoomTimeStamp), roomNameValue);
    globalDataBase.removeTimeStamp(accountName(), roomName(), GlobalDatabase::TimeStampType::RoomTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomName(), GlobalDatabase::TimeStampType::RoomTimeStamp), -1);
}

#include "moc_globaldatabasetest.cpp"
