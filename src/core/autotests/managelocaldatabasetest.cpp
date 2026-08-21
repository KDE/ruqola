/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managelocaldatabasetest.h"
#include "managelocaldatabase.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(ManageLocalDatabaseTest)
ManageLocalDatabaseTest::ManageLocalDatabaseTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ManageLocalDatabaseTest::shouldHaveDefaultValues()
{
    ManageLocalDatabase d(nullptr);
    // TODO
}

#include "moc_managelocaldatabasetest.cpp"
