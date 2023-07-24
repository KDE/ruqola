/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistoryparsesyncmessagesutilstest.h"
#include "manageloadhistoryparsesyncmessagesutils.h"
#include <QTest>
QTEST_MAIN(ManageLoadHistoryParseSyncMessagesUtilsTest)
ManageLoadHistoryParseSyncMessagesUtilsTest::ManageLoadHistoryParseSyncMessagesUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ManageLoadHistoryParseSyncMessagesUtilsTest::shouldHaveDefaultValues()
{
    ManageLoadHistoryParseSyncMessagesUtils w(nullptr);
    QVERIFY(w.deletedMessages().isEmpty());
    QVERIFY(w.updatesMessages().isEmpty());
}

#include "moc_manageloadhistoryparsesyncmessagesutilstest.cpp"
