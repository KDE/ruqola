/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localglobaldatabasetest.h"
#include "localdatabase/localglobaldatabase.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(LocalGlobalDatabaseTest)
LocalGlobalDatabaseTest::LocalGlobalDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalGlobalDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

#if 0
    // Clean up after previous runs
    LocalMessageLogger logger;
    QFile::remove(logger.dbFileName(accountName(), roomName()));
    QFile::remove(logger.dbFileName(accountName(), otherRoomName()));
    QFile::remove(logger.dbFileName(accountName(), existingRoomName()));
#endif
}

void LocalGlobalDatabaseTest::shouldHaveDefaultValues()
{
    LocalGlobalDatabase w;
    // TODO
}
