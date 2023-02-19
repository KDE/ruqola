/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localglobaldatabasetest.h"
#include "localdatabase/localglobaldatabase.h"
#include <QTest>
QTEST_MAIN(LocalGlobalDatabaseTest)
LocalGlobalDatabaseTest::LocalGlobalDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalGlobalDatabaseTest::shouldHaveDefaultValues()
{
    LocalGlobalDatabase w;
    // TODO
}
