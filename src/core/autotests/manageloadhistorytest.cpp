/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistorytest.h"
#include "manageloadhistory.h"
#include <QTest>
QTEST_MAIN(ManageLoadHistoryTest)
ManageLoadHistoryTest::ManageLoadHistoryTest(QObject *parent)
    : QObject{parent}
{
}

void ManageLoadHistoryTest::shouldHaveDefaultValues()
{
    ManageLoadHistory d(nullptr);
    // TODO
}

#include "moc_manageloadhistorytest.cpp"
