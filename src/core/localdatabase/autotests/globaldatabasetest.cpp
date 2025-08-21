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

static QByteArray roomId()
{
    return "myRoom"_ba;
}
static QByteArray roomIdOther()
{
    return "myOtherRoom"_ba;
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
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomId(), roomNameValue, GlobalDatabase::TimeStampType::MessageTimeStamp);
    const int roomNameOtherValue = 12;
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomIdOther(), roomNameOtherValue, GlobalDatabase::TimeStampType::MessageTimeStamp);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomId(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameValue);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    // -1 if timeStamp doesn't exist
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::AccountTimeStamp), -1);
}

void GlobalDatabaseTest::shouldGenerateIdentifier()
{
    QFETCH(QString, accountName);
    QFETCH(QByteArray, roomId);
    QFETCH(GlobalDatabase::TimeStampType, type);
    QFETCH(QString, result);

    GlobalDatabase globalDataBase;
    QCOMPARE(globalDataBase.generateIdentifier(accountName, roomId, type), result);
}

void GlobalDatabaseTest::shouldGenerateIdentifier_data()
{
    QTest::addColumn<QString>("accountName");
    QTest::addColumn<QByteArray>("roomId");
    QTest::addColumn<GlobalDatabase::TimeStampType>("type");
    QTest::addColumn<QString>("result");

    QTest::addRow("empty") << QString() << QByteArray() << GlobalDatabase::TimeStampType::MessageTimeStamp << QString();
    QTest::addRow("test1") << u"account1"_s << "room1"_ba << GlobalDatabase::TimeStampType::MessageTimeStamp << u"messages-account1-room1"_s;
    QTest::addRow("test2") << u"account2"_s << "room2"_ba << GlobalDatabase::TimeStampType::RoomTimeStamp << u"rooms-account2-room2"_s;
    QTest::addRow("test3") << u"account3"_s << QByteArray() << GlobalDatabase::TimeStampType::AccountTimeStamp << u"account-account3"_s;
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
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomId(), roomNameValue, GlobalDatabase::TimeStampType::MessageTimeStamp);
    const int roomNameOtherValue = 12;
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomIdOther(), roomNameOtherValue, GlobalDatabase::TimeStampType::MessageTimeStamp);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomId(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameValue);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    // Remove it.
    globalDataBase.removeTimeStamp(accountName(), roomId(), GlobalDatabase::TimeStampType::MessageTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomId(), GlobalDatabase::TimeStampType::MessageTimeStamp), -1);

    // OTher still exists
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    globalDataBase.removeTimeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::AccountTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);

    globalDataBase.removeTimeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::MessageTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomIdOther(), GlobalDatabase::TimeStampType::MessageTimeStamp), -1);

    // RoomTimeStamp
    globalDataBase.insertOrReplaceTimeStamp(accountName(), roomId(), roomNameValue, GlobalDatabase::TimeStampType::RoomTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomId(), GlobalDatabase::TimeStampType::RoomTimeStamp), roomNameValue);
    globalDataBase.removeTimeStamp(accountName(), roomId(), GlobalDatabase::TimeStampType::RoomTimeStamp);
    QCOMPARE(globalDataBase.timeStamp(accountName(), roomId(), GlobalDatabase::TimeStampType::RoomTimeStamp), -1);
}

#include "moc_globaldatabasetest.cpp"
