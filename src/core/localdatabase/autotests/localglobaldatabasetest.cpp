/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localglobaldatabasetest.h"
#include "localdatabase/localglobaldatabase.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(LocalGlobalDatabaseTest)
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

LocalGlobalDatabaseTest::LocalGlobalDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalGlobalDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalGlobalDatabase globalDataBase;
    QFile::remove(globalDataBase.dbFileName(accountName()));
}

void LocalGlobalDatabaseTest::shouldStoreIdentifier()
{
    // GIVEN
    LocalGlobalDatabase globalDataBase;
    int roomNameValue = 55;
    globalDataBase.updateTimeStamp(accountName(), roomName(), roomNameValue, LocalGlobalDatabase::TimeStampType::MessageTimeStamp);
    int roomNameOtherValue = 12;
    globalDataBase.updateTimeStamp(accountName(), roomNameOther(), roomNameOtherValue, LocalGlobalDatabase::TimeStampType::MessageTimeStamp);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomName(), LocalGlobalDatabase::TimeStampType::MessageTimeStamp), roomNameValue);

    QCOMPARE(globalDataBase.timeStamp(accountName(), roomNameOther(), LocalGlobalDatabase::TimeStampType::MessageTimeStamp), roomNameOtherValue);
}
