/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroompendingtypedinfodatabasetest.h"
#include "localdatabase/localroompendingtypedinfodatabase.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(LocalRoomPendingTypedInfoDatabaseTest)
using namespace Qt::Literals::StringLiterals;
static QString accountName()
{
    return u"myAccount"_s;
}
LocalRoomPendingTypedInfoDatabaseTest::LocalRoomPendingTypedInfoDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalRoomPendingTypedInfoDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalRoomPendingTypedInfoDatabase roomPendingTypedInfoDataBase;
    QFile::remove(roomPendingTypedInfoDataBase.dbFileName(accountName()));
}

#include "moc_localroompendingtypedinfodatabasetest.cpp"
